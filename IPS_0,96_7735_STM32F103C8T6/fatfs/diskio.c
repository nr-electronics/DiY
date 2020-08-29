/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <diskio.h>		/* FatFs lower layer API */
#include <spim.h>
#include <gpio.h>
#include <delay.h>


/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

/* Definitions for MMC/SDC command */
#define CMD0	(0)		/* GO_IDLE_STATE */
#define CMD1	(1)		/* SEND_OP_COND (MMC) */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)		/* SEND_IF_COND */
#define CMD9	(9)		/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */

/* Card type flags (CardType) */
#define CT_MMC		0x01		/* MMC ver 3 */
#define CT_SD1		0x02		/* SD ver 1 */
#define CT_SD2		0x04		/* SD ver 2 */
#define CT_SDC		(CT_SD1|CT_SD2)	/* SD */
#define CT_BLOCK	0x08		/* Block addressing */

static volatile DSTATUS Stat = STA_NOINIT;	/* Disk status */
static BYTE CardType;			/* Card type flags */
static uint32_t ReadNum = 0, WriteNum = 0;

#if (SDcard_CS_Used)
  #define SDcard_CS_HIGH()              GPIO_WriteBit(SDcard_CS_Port, SDcard_CS_Pin, Bit_SET) 
  #define SDcard_CS_LOW()               GPIO_WriteBit(SDcard_CS_Port, SDcard_CS_Pin, Bit_RESET)
#else
  #define SDcard_CS_HIGH()            
  #define SDcard_CS_LOW()            
#endif


//==============================================================================
//
//==============================================================================
static void SDcard_GPIO_init(void)
{
  // Настраиваем все используемые ноги как выходы
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  
#if (SDcard_CS_Used)
  // Сигнал CS (SPI)
  gpio_PortClockStart(SDcard_CS_Port);
  GPIO_InitStruct.GPIO_Pin = SDcard_CS_Pin;
  GPIO_Init(SDcard_CS_Port, &GPIO_InitStruct);
  SDcard_CS_HIGH();
#endif
}
//==============================================================================


//==============================================================================
//
//==============================================================================
static void SDcard_PowerOn(void)
{
  delay_ms(20);

  SDcard_GPIO_init();
  SDcard_CS_LOW();
}
//==============================================================================


//==============================================================================
//
//==============================================================================
static void SDcard_PowerOff(void)
{
  SDcard_CS_HIGH();
  
  Stat |= STA_NOINIT;
}
//==============================================================================


//==============================================================================
/* Wait for card ready                                                   */
//==============================================================================
static int wait_ready(void)	/* 1:OK, 0:Timeout */
{
  BYTE d = 0;
  uint32_t TOcntr = 500;

  do
  {
    delay_us(1);
    d = SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
    TOcntr--;
  }
  while(d != 0xFF && TOcntr);
  
  return (d == 0xFF) ? 1 : 0;
}
//==============================================================================


//==============================================================================
/* Deselect the card and release SPI bus                                 */
//==============================================================================
static void deselect (void)
{
  SDcard_CS_HIGH();
  SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);	/* Dummy clock (force DO hi-z for multiple slave SPI) */
}
//==============================================================================


//==============================================================================
/* Select the card and wait for ready                                    */
//==============================================================================
static int select(void)	/* 1:Successful, 0:Timeout */
{
  SDcard_CS_LOW();
  SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);	/* Dummy clock (force DO enabled) */
  
  if (wait_ready()) 
    return 1;	/* OK */
  
  deselect();
  
  return 0;	/* Timeout */
}
//==============================================================================


//==============================================================================
/* Send a command packet to MMC                                           */
/* Returns R1 resp (bit7==1:Send failed)                                  */
/* cmd - Command index                                                    */
/* arg - Argument                                                         */
//==============================================================================
static BYTE send_cmd(BYTE cmd, DWORD arg)
{
  BYTE n, res;
  
  /* ACMD<n> is the command sequense of CMD55-CMD<n> */
  if (cmd & 0x80) 
  {
    cmd &= 0x7F;
    res = send_cmd(CMD55, 0);
    if (res > 1)
      return res;
  }
  
  /* Select the card and wait for ready */
  deselect();
  if (!select())
    return 0xFF;
  
  /* Send command packet */
  SPI_SendRecvByte(SDcard_SPI_periph, 0x40 | cmd);			/* Start + Command index */
  SPI_SendRecvByte(SDcard_SPI_periph, (BYTE)(arg >> 24));		/* Argument[31..24] */
  SPI_SendRecvByte(SDcard_SPI_periph, (BYTE)(arg >> 16));		/* Argument[23..16] */
  SPI_SendRecvByte(SDcard_SPI_periph, (BYTE)(arg >> 8));		/* Argument[15..8] */
  SPI_SendRecvByte(SDcard_SPI_periph, (BYTE)arg);			/* Argument[7..0] */
  n = 0x01;				/* Dummy CRC + Stop */
  
  if (cmd == CMD0)
    n = 0x95;			/* Valid CRC for CMD0(0) */
  if (cmd == CMD8)
    n = 0x87;			/* Valid CRC for CMD8(0x1AA) */
  
  SPI_SendRecvByte(SDcard_SPI_periph, n);

  /* Receive command response */
  if (cmd == CMD12)
    SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);		/* Skip a stuff byte when stop reading */
  
  n = 10;								/* Wait for a valid response in timeout of 10 attempts */
  do
  {
    res = SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
  }
  while ((res & 0x80) && --n);

  return res;			/* Return with the response value */
}
//==============================================================================


//==============================================================================
/* Get Drive Status                                                      */
/* pdrv - Physical drive nmuber to identify the drive                    */
//==============================================================================
DSTATUS disk_status(BYTE pdrv)
{
  if (pdrv)
    return STA_NOINIT;		/* Supports only single drive */

  return Stat;
}
//==============================================================================


//==============================================================================
/* Receive a data packet from MMC                                         */
/* buff	- Data buffer to store received data                              */
/* btr  - Byte count (must be multiple of 4)                              */
//==============================================================================
static int rcvr_datablock(BYTE *buff, UINT btr)
{
  BYTE token;
  uint32_t TOcntr = 200;

  /* Wait for data packet in timeout of 200ms */
  do 
  {
    delay_us(1);
    token = SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
    TOcntr--;
  } 
  while ((token == 0xFF) && TOcntr);
  
  /* If not valid data token, retutn with error */
  if (token != 0xFE) 
    return 0;

  ReadNum++;
    /*
  if (SD_SPIDMA_ReadBlock(buff, btr) == rDMAfunc_w_SEM)
  {
    xStatus = xSemaphoreTake(xSDSPI_RxDMA_FreeShandle, 50);
    if (xStatus != pdPASS)
      return 0;
  }
  else
  {
    Timer1 = 10;
    while(SPI_DMA_bisy && Timer1) ;
    if (!Timer1)
      return 0;
  }
  */
  /* Receive the data block into buffer */
  
  do 
  {
    *(buff++) = SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
    *(buff++) = SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
    *(buff++) = SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
    *(buff++) = SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
  } while (btr -= 4);
  
  
  SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);						/* Discard CRC */
  SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
  
  return 1;						/* Return with success */
}
//==============================================================================


//==============================================================================
/* Send a data packet to MMC                                              */
/* buff  - 512 byte data block to be transmitted                          */
/* token - Data/Stop token                                                */
//==============================================================================
static int xmit_datablock(const BYTE *buff, BYTE token)
{
  //BYTE resp;
  BYTE wc;
  
  if (!wait_ready())
    return 0;
  
  SPI_SendRecvByte(SDcard_SPI_periph, token);					/* Xmit data token */
  
  /* Is data token */
  if (token != 0xFD) 
  {
    wc = 0;
    
    WriteNum++;
    //GPIOE->BSRRH = (1<<6);
    /*
    if (SD_SPIDMA_WriteBlock((uint8_t*)buff, 513) == rDMAfunc_w_SEM)
    {
      xStatus = xSemaphoreTake(xSDSPI_TxDMA_FreeShandle, 50);
      if (xStatus != pdPASS)
        return 0;
    }
    else
    {
      Timer1 = 10;
      while(SPI_DMA_bisy && Timer1) ;
      if (!Timer1)
        return 0;
    }    
*/    
    /* Xmit the 512 byte data block to MMC */
    do 
    {
      SPI_SendRecvByte(SDcard_SPI_periph, *buff++);
      SPI_SendRecvByte(SDcard_SPI_periph, *buff++);
    } 
    while (--wc);
    
    //GPIOE->BSRRL = (1<<6);
    
    SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);				/* CRC (Dummy) */
    SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
    /*resp = */SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);		/* Reveive data response */

    //if ((resp & 0x1F) != 0x05)		/* If not accepted, return with error */
    //  return 0;
  }
  return 1;
}
//==============================================================================


//==============================================================================
/* Inidialize a Drive                                                    */
/* pdrv - Physical drive nmuber to identify the drive                    */
//==============================================================================
DSTATUS disk_initialize(BYTE pdrv)
{
  BYTE n, cmd, ty, ocr[4];
  
  if (pdrv)
    return STA_NOINIT;		/* Supports only single drive */
  if (Stat & STA_NODISK)
    return Stat;	        /* No card in the socket */
  
  SDcard_PowerOn();			/* Force socket power on */
  
  spim_init(SDcard_SPI_periph, 0, 8);

  // 80 dummy clocks
  for (n = 10; n; n--)
    SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
      
  ty = 0;
      
  if (send_cmd(CMD0, 0) == 1)			/* Enter Idle state */
  {
    uint32_t TOcntr = 1000;			/* Initialization timeout of 1000 msec */
    if (send_cmd(CMD8, 0x1AA) == 1)	        /* SDv2? */
    {
      for (n = 0; n < 4; n++)
        ocr[n] = SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);		        /* Get trailing return value of R7 resp */
      if (ocr[2] == 0x01 && ocr[3] == 0xAA) 	/* The card can work at vdd range of 2.7-3.6V */
      {
	/* Wait for leaving idle state (ACMD41 with HCS bit) */
        while (TOcntr && send_cmd(ACMD41, 1UL << 30))
        {
          delay_us(1);
          TOcntr--;
        }
        
        if (TOcntr && send_cmd(CMD58, 0) == 0)		/* Check CCS bit in the OCR */
        {
          for (n = 0; n < 4; n++)
            ocr[n] = SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
          
          ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* SDv2 */
        }
      }
    }
    else							/* SDv1 or MMCv3 */
    {
      if (send_cmd(ACMD41, 0) <= 1)
      {
        ty = CT_SD1;	/* SDv1 */
        cmd = ACMD41;
      } 
      else
      {
        ty = CT_MMC;	/* MMCv3 */
        cmd = CMD1;
      }
      
      /* Wait for leaving idle state */
      while (TOcntr && send_cmd(cmd, 0))
      {
        delay_us(1);
        TOcntr--;
      }
      
      if (!TOcntr || send_cmd(CMD16, 512) != 0)	/* Set R/W block length to 512 */
        ty = 0;
    }
  }
  
  CardType = ty;
  deselect();
      
  if (ty)		  /* Initialization succeded */
  {
    Stat &= ~STA_NOINIT;  /* Clear STA_NOINIT */
    spim_init(SDcard_SPI_periph, 1, 8);
  }
  else			  /* Initialization failed */
    SDcard_PowerOff();
  
  return Stat;
}
//==============================================================================


//==============================================================================
/* Read Sector(s)                                                        */
/* pdrv,	Physical drive nmuber to identify the drive */
/* *buff,	Data buffer to store read data */
/* sector,	Start sector in LBA */
/* count	Number of sectors to read */
//==============================================================================
DRESULT disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
//  char cmd_ok=0, rcv=0;
  
  if (pdrv || !count)
    return RES_PARERR;
  if (Stat & STA_NOINIT)
    return RES_NOTRDY;
  
  /* Convert to byte address if needed */
  if (!(CardType & CT_BLOCK))
    sector *= 512;
  
  if (count == 1) 	/* Single block read */
  {
    if ((send_cmd(CMD17, sector) == 0) && rcvr_datablock(buff, 512))	/* READ_SINGLE_BLOCK */
      count = 0;
/*
    cmd_ok = send_cmd(CMD17, sector);
    rcv = rcvr_datablock(buff, 512);
    
    if ((cmd_ok == 0) && (rcv))
      count = 0;
    else
      count = 1;
*/
  }
  else 			/* Multiple block read */
  {
    if (send_cmd(CMD18, sector) == 0)	                                /* READ_MULTIPLE_BLOCK */
    {
      do
      {
        if (!rcvr_datablock(buff, 512))
          break;
        buff += 512;
      }
      while (--count);
      send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */
    }
  }
  
  deselect();

  return count ? RES_ERROR : RES_OK;
}
//==============================================================================


//==============================================================================
/* Write Sector(s)                                                       */
/* pdrv,	Physical drive nmuber to identify the drive */
/* *buff,	Data to be written */
/* sector,	Start sector in LBA */
/* count	Number of sectors to write */
//==============================================================================
DRESULT disk_write(BYTE pdrv, const BYTE *buff,	DWORD sector, UINT count)
{
//  char cmd_ok=0, rcv=0;

  if (pdrv || !count)
    return RES_PARERR;
  if (Stat & STA_NOINIT)
    return RES_NOTRDY;
  if (Stat & STA_PROTECT)
    return RES_WRPRT;
  
  /* Convert to byte address if needed */
  if (!(CardType & CT_BLOCK))
    sector *= 512;
  
  if (count == 1)	/* Single block write */
  {
    if ((send_cmd(CMD24, sector) == 0) && xmit_datablock(buff, 0xFE))	/* WRITE_BLOCK */
      count = 0;
/*    
    cmd_ok = send_cmd(CMD24, sector);
    rcv = xmit_datablock(buff, 0xFE);
    
    if ((cmd_ok == 0) && (rcv))
      count = 0;
    else
      count = 1;
*/    
  }
  else			              /* Multiple block write */
  {
    if (CardType & CT_SDC)
      send_cmd(ACMD23, count);
    if (send_cmd(CMD25, sector) == 0) /* WRITE_MULTIPLE_BLOCK */
    {
      do 
      {
        if (!xmit_datablock(buff, 0xFC))
          break;
        buff += 512;
      } 
      while (--count);
      
      if (!xmit_datablock(0, 0xFD))	/* STOP_TRAN token */
        count = 1;
    }
  }
  
  deselect();
  
  return count ? RES_ERROR : RES_OK;
}
//==============================================================================
/*

//==============================================================================
// Miscellaneous Functions                                               
// pdrv,	Physical drive nmuber (0..)                              
// cmd,		Control code                                             
// *buff	Buffer to send/receive control data                      
//==============================================================================
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
  if (cmd == CTRL_SYNC) 
  {
     return RES_OK;
  }
  else 
  {
    // should never be called
    return RES_ERROR;
  }
}
//==============================================================================
*/

/*-----------------------------------------------------------------------*/
/* Power Control  (Platform dependent)                                   */
/*-----------------------------------------------------------------------*/
/* When the target system does not support socket power control, there   */
/* is nothing to do in these functions and chk_power always returns 1.   */
static int power_status(void)		/* Socket power state: 0=off, 1=on */
{
//  return (PORTE & 0x80) ? 0 : 1;
  return 1;
}

/*------------------------------------------------------------------------*/
/* Miscellaneous Functions                                                */
/* drv   - Physical drive nmuber (0)                                      */
/* ctrl  - Control code                                                   */
/* *buff - Buffer to send/receive control data                            */
/*------------------------------------------------------------------------*/
DRESULT disk_ioctl(BYTE drv, BYTE ctrl,	void *buff)
{
  DRESULT res;
  BYTE n, csd[16], *ptr = buff;
  WORD csize;
  
  if (drv)
    return RES_PARERR;
  
  //__watchdog_reset();

  res = RES_ERROR;
  
  if (ctrl == CTRL_POWER)
  {
    switch (ptr[0])
    {
    case 0:		/* Sub control code (POWER_OFF) */
      SDcard_PowerOff();		/* Power off */
      res = RES_OK;
      break;
    case 1:		/* Sub control code (POWER_GET) */
      ptr[1] = (BYTE)power_status();
      res = RES_OK;
      break;
    default :
      res = RES_PARERR;
    }
  }
  else
  {
    if (Stat & STA_NOINIT)
      return RES_NOTRDY;
    
    switch (ctrl)
    {
    case CTRL_SYNC :		/* Make sure that no pending write process. Do not remove this or written sector might not left updated. */
      if (select())
      {
        deselect();
        res = RES_OK;
      }
      break;
    case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
      if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16))
      {
        if ((csd[0] >> 6) == 1)	/* SDC ver 2.00 */
        {
          csize = csd[9] + ((WORD)csd[8] << 8) + 1;
          *(DWORD*)buff = (DWORD)csize << 10;
        } 
        else			/* SDC ver 1.XX or MMC*/
        {
          n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
          csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
          *(DWORD*)buff = (DWORD)csize << (n - 9);
        }
        res = RES_OK;
      }
      break;
    case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
      *(WORD*)buff = 512;
      res = RES_OK;
      break;
    case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
      if (CardType & CT_SD2)	/* SDv2? */
      {
        if (send_cmd(ACMD13, 0) == 0) 	/* Read SD status */
        {
          SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
          if (rcvr_datablock(csd, 16))	/* Read partial block */
          {
            /* Purge trailing data */
            for (n = 64 - 16; n; n--) 
              SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
            
            *(DWORD*)buff = 16UL << (csd[10] >> 4);
            res = RES_OK;
          }
        }
      } 
      else					/* SDv1 or MMCv3 */
      {
        if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) 	/* Read CSD */
        {
          if (CardType & CT_SD1)	/* SDv1 */
            *(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
          else				/* MMCv3 */
            *(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);

          res = RES_OK;
        }
      }
      break;
    case MMC_GET_TYPE :		/* Get card type flags (1 byte) */
      *ptr = CardType;
      res = RES_OK;
      break;
    case MMC_GET_CSD :		/* Receive CSD as a data block (16 bytes) */
      if (send_cmd(CMD9, 0) == 0 && rcvr_datablock(ptr, 16))    /* READ_CSD */
        res = RES_OK;
      break;
    case MMC_GET_CID :		/* Receive CID as a data block (16 bytes) */
      if (send_cmd(CMD10, 0) == 0 && rcvr_datablock(ptr, 16))	/* READ_CID */
        res = RES_OK;
      break;
    case MMC_GET_OCR :		/* Receive OCR as an R3 resp (4 bytes) */
      if (send_cmd(CMD58, 0) == 0) 	                        /* READ_OCR */
      {
        for (n = 4; n; n--)
          *ptr++ = SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
        res = RES_OK;
      }
      break;
    case MMC_GET_SDSTAT :	/* Receive SD statsu as a data block (64 bytes) */
      if (send_cmd(ACMD13, 0) == 0) 	/* SD_STATUS */
      {
        SPI_SendRecvByte(SDcard_SPI_periph, 0xFF);
        if (rcvr_datablock(ptr, 64))
          res = RES_OK;
      }
      break;
    default:
      res = RES_PARERR;
    }
    deselect();
  }
  return res;
}
