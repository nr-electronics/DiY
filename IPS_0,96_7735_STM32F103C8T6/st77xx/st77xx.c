//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <delay.h>
#include <dispcolor.h>
#include <st77xx.h>
#include <gpio.h>
#include <spim.h>
#include <timers.h>


#if (ST77xx_CS_Used)
  #define ST77xx_CS_HIGH()              GPIO_WriteBit(ST77xx_CS_Port, ST77xx_CS_Pin, Bit_SET) 
  #define ST77xx_CS_LOW()               GPIO_WriteBit(ST77xx_CS_Port, ST77xx_CS_Pin, Bit_RESET)
#else
  #define ST77xx_CS_HIGH()            
  #define ST77xx_CS_LOW()            
#endif

#if (ST77xx_RESET_Used)
  #define ST77xx_RESET_HIGH()           GPIO_WriteBit(ST77xx_RESET_Port, ST77xx_RESET_Pin, Bit_SET) 
  #define ST77xx_RESET_LOW()            GPIO_WriteBit(ST77xx_RESET_Port, ST77xx_RESET_Pin, Bit_RESET)
#endif

#if (ST77xx_DC_Used)
  #define ST77xx_DC_HIGH()              GPIO_WriteBit(ST77xx_DC_Port, ST77xx_DC_Pin, Bit_SET) 
  #define ST77xx_DC_LOW()               GPIO_WriteBit(ST77xx_DC_Port, ST77xx_DC_Pin, Bit_RESET)
#endif

#if (ST77xx_BLK_PWM_Used)
#else
  #define ST77xx_BLK_HIGH()             GPIO_WriteBit(ST77xx_BLK_Port, ST77xx_BLK_Pin, Bit_SET) 
  #define ST77xx_BLK_LOW()              GPIO_WriteBit(ST77xx_BLK_Port, ST77xx_BLK_Pin, Bit_RESET)
#endif

uint8_t ST77xx_Width, ST77xx_Height;
uint8_t ST77xx_X_Start, ST77xx_Y_Start;

//==============================================================================
// Процедура отправки команды в дисплей
//==============================================================================
void ST77xx_SendCmd(uint8_t Cmd)
{
  ST77xx_DC_LOW();
  ST77xx_CS_LOW();

  SPI_send8b(ST77xx_SPI_periph, &Cmd, 1);

  ST77xx_CS_HIGH();
}
//==============================================================================


//==============================================================================
// Процедура отправки данных (параметров) в дисплей
//==============================================================================
void ST77xx_SendData(uint8_t Data)
{
  ST77xx_DC_HIGH();
  ST77xx_CS_LOW();
    
  SPI_send8b(ST77xx_SPI_periph, &Data, 1);

  ST77xx_CS_HIGH();
}
//==============================================================================


//==============================================================================
// Процедура программного сброса дисплея
//==============================================================================
void ST77xx_SoftReset(void)
{
  ST77xx_SendCmd(ST77xx_Cmd_SWRESET);
  delay_ms(130);
}
//==============================================================================


//==============================================================================
// Процедура аппаратного сброса дисплея (ножкой RESET)
//==============================================================================
void ST77xx_HardReset(void)
{
#if (ST77xx_RESET_Used)
  ST77xx_RESET_LOW();
  delay_ms(10);
  ST77xx_RESET_HIGH();
  delay_ms(150);
#endif
}
//==============================================================================


//==============================================================================
// Процедура включения/отключения режима сна
//==============================================================================
void ST77xx_SleepMode(uint8_t Mode)
{
  if (Mode)
    ST77xx_SendCmd(ST77xx_Cmd_SLPIN);
  else
    ST77xx_SendCmd(ST77xx_Cmd_SLPOUT);
  
  delay_ms(500);
}
//==============================================================================


//==============================================================================
// Процедура включения/отключения режима частичного заполнения экрана
//==============================================================================
void ST77xx_InversionMode(uint8_t Mode)
{
  if (Mode)
    ST77xx_SendCmd(ST77xx_Cmd_INVON);
  else
    ST77xx_SendCmd(ST77xx_Cmd_INVOFF);
}
//==============================================================================


//==============================================================================
// Процедура включения/отключения питания дисплея
//==============================================================================
void ST77xx_DisplayPower(uint8_t On)
{
  if (On)
    ST77xx_SendCmd(ST77xx_Cmd_DISPON);
  else
    ST77xx_SendCmd(ST77xx_Cmd_DISPOFF);
}
//==============================================================================


//==============================================================================
// Процедура установки начального и конечного адресов колонок
//==============================================================================
static void ST77xx_ColumnSet(uint16_t ColumnStart, uint16_t ColumnEnd)
{
  if (ColumnStart > ColumnEnd)
    return;
  if (ColumnEnd > ST77xx_Width)
    return;
  
  ColumnStart += ST77xx_X_Start;
  ColumnEnd += ST77xx_X_Start;
  
  ST77xx_SendCmd(ST77xx_Cmd_CASET);
  ST77xx_SendData(ColumnStart >> 8);  
  ST77xx_SendData(ColumnStart & 0xFF);  
  ST77xx_SendData(ColumnEnd >> 8);  
  ST77xx_SendData(ColumnEnd & 0xFF);  
}
//==============================================================================


//==============================================================================
// Процедура установки начального и конечного адресов строк
//==============================================================================
static void ST77xx_RowSet(uint16_t RowStart, uint16_t RowEnd)
{
  if (RowStart > RowEnd)
    return;
  if (RowEnd > ST77xx_Height)
    return;
  
  RowStart += ST77xx_Y_Start;
  RowEnd += ST77xx_Y_Start;
  
  ST77xx_SendCmd(ST77xx_Cmd_RASET);
  ST77xx_SendData(RowStart >> 8);  
  ST77xx_SendData(RowStart & 0xFF);  
  ST77xx_SendData(RowEnd >> 8);  
  ST77xx_SendData(RowEnd & 0xFF);  
}
//==============================================================================


//==============================================================================
// Процедура установка границ экрана для заполнения
//==============================================================================
void ST77xx_SetWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  ST77xx_ColumnSet(x0, x1);
  ST77xx_RowSet(y0, y1);
  
  ST77xx_SendCmd(ST77xx_Cmd_RAMWR);
}
//==============================================================================


//==============================================================================
// Процедура записи данных в дисплей
//==============================================================================
void ST77xx_RamWrite(uint16_t *pBuff, uint16_t Len)
{
  while (Len--)
  {
    ST77xx_SendData(*pBuff >> 8);  
    ST77xx_SendData(*pBuff & 0xFF);
  }  
}
//==============================================================================


//==============================================================================
// Процедура выбора кривой гамма-коррекции
//==============================================================================
void ST77xx_GammaSet(uint8_t CurveNum)
{
  if (CurveNum > 4)
    return;
  
  ST77xx_SendCmd(ST77xx_Cmd_GAMSET);
  ST77xx_SendData(1 << (CurveNum - 1));  
}
//==============================================================================


//==============================================================================
// Процедура настройки формата цвета
//==============================================================================
void ST77xx_ColorModeSet(uint8_t ColorMode)
{
  ST77xx_SendCmd(ST77xx_Cmd_COLMOD);
  ST77xx_SendData(ColorMode & 0x77);  
}
//==============================================================================


//==============================================================================
// Процедура настройки отображения  
//==============================================================================
void ST77xx_MemAccessModeSet(uint8_t Rotation, uint8_t VertMirror, uint8_t HorizMirror, uint8_t IsBGR)
{
  uint8_t Value;
  Rotation &= 7; 

  ST77xx_SendCmd(ST77xx_Cmd_MADCTL);
  
  // Настройка направления заполнения экрана
  switch (Rotation)
  {
  case 0:
    Value = 0;
    break;
  case 1:
    Value = ST77xx_MADCTL_MX;
    break;
  case 2:
    Value = ST77xx_MADCTL_MY;
    break;
  case 3:
    Value = ST77xx_MADCTL_MX | ST77xx_MADCTL_MY;
    break;
  case 4:
    Value = ST77xx_MADCTL_MV;
    break;
  case 5:
    Value = ST77xx_MADCTL_MV | ST77xx_MADCTL_MX;
    break;
  case 6:
    Value = ST77xx_MADCTL_MV | ST77xx_MADCTL_MY;
    break;
  case 7:
    Value = ST77xx_MADCTL_MV | ST77xx_MADCTL_MX | ST77xx_MADCTL_MY;
    break;
  }
  
  if (VertMirror)
    Value = ST77xx_MADCTL_ML;
  if (HorizMirror)
    Value = ST77xx_MADCTL_MH;
  
  // Использование порядка цветов BGR вместо RGB
  if (IsBGR)
    Value |= ST77xx_MADCTL_BGR;
  
  ST77xx_SendData(Value);
}
//==============================================================================


//==============================================================================
// Процедура управления подсветкой (без поддержки ШИМ)
//==============================================================================
void st77xx_SetBL(uint8_t Value)
{
  if (Value > 100)
    Value = 100;
  
#if (ST77xx_BLK_PWM_Used)
  tmr2_PWM_set(ST77xx_PWM_TMR2_Chan, Value);
#else
  if (Value)
    ST77xx_BLK_HIGH();    
  else
    ST77xx_BLK_LOW();
#endif
}
//==============================================================================


//==============================================================================
// Процедура окрашивает 1 пиксель дисплея
//==============================================================================
void ST77xx_DrawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) ||(x >= ST77xx_Width) || (y < 0) || (y >= ST77xx_Height))
    return;

  ST77xx_SetWindow(x, y, x, y);
  ST77xx_RamWrite(&color, 1);
}
//==============================================================================


//==============================================================================
// Процедура заполнения прямоугольника цветом color
//==============================================================================
void ST77xx_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  if ((x >= ST77xx_Width) || (y >= ST77xx_Height))
    return;
  
  if ((x + w) > ST77xx_Width)
    w = ST77xx_Width - x;
  
  if ((y + h) > ST77xx_Height)
    h = ST77xx_Height - y;

  ST77xx_SetWindow(x, y, x + w - 1, y + h - 1);

  for (uint32_t i = 0; i < (h * w); i++)
    ST77xx_RamWrite(&color, 1);
}
//==============================================================================


//==============================================================================
// Процедура заполнения прямоугольной области из буфера. Порядок заполнения экрана Y - X
//==============================================================================
void ST77xx_DrawPartYX(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff)
{
  if ((x >= ST77xx_Width) || (y >= ST77xx_Height))
    return;
  
  if ((x + w - 1) >= ST77xx_Width)
    w = ST77xx_Width - x;
  
  if ((y + h - 1) >= ST77xx_Height)
    h = ST77xx_Height - y;

  ST77xx_SetWindow(x, y, x + w - 1, y + h - 1);

  for (uint32_t i = 0; i < (h * w); i++)
    ST77xx_RamWrite(pBuff++, 1);
}
//==============================================================================


//==============================================================================
// Процедура заполнения прямоугольной области из буфера. Порядок заполнения экрана X - Y
//==============================================================================
void ST77xx_DrawPartXY(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff)
{
  if ((x >= ST77xx_Width) || (y >= ST77xx_Height))
    return;
  
  if ((x + w - 1) >= ST77xx_Width)
    w = ST77xx_Width - x;
  
  if ((y + h - 1) >= ST77xx_Height)
    h = ST77xx_Height - y;

  for (uint16_t iy = y; iy < y + h; iy++)
  {
    ST77xx_SetWindow(x, iy, x + w - 1, iy + 1);
    for (x = w; x > 0; x--)
      ST77xx_RamWrite(pBuff++, 1);
  }
}
//==============================================================================


//==============================================================================
// Процедура закрашивает экран цветом color
//==============================================================================
void ST77xx_FillScreen(uint16_t color)
{
  ST77xx_FillRect(0, 0,  ST77xx_Width, ST77xx_Height, color);
}
//==============================================================================


//==============================================================================
// Процедура настройки ножек микроконтроллера для обмена с ST77xx
//==============================================================================
void ST77xx_GPIO_init(void)
{
  // Настраиваем все используемые ноги как выходы
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  
#if (ST77xx_RESET_Used)
  // Сигнал RESET
  gpio_PortClockStart(ST77xx_RESET_Port);
  GPIO_InitStruct.GPIO_Pin = ST77xx_RESET_Pin;
  GPIO_Init(ST77xx_RESET_Port, &GPIO_InitStruct);
  ST77xx_RESET_HIGH();
#endif
  
#if (ST77xx_CS_Used)
  // Сигнал CS (SPI)
  gpio_PortClockStart(ST77xx_CS_Port);
  GPIO_InitStruct.GPIO_Pin = ST77xx_CS_Pin;
  GPIO_Init(ST77xx_CS_Port, &GPIO_InitStruct);
  ST77xx_CS_HIGH();
#endif
  
#if (ST77xx_DC_Used)
  // Сигнал DC (Data/Command)
  gpio_PortClockStart(ST77xx_DC_Port);
  GPIO_InitStruct.GPIO_Pin = ST77xx_DC_Pin;
  GPIO_Init(ST77xx_DC_Port, &GPIO_InitStruct);
  ST77xx_DC_HIGH();
#endif

#if (ST77xx_BLK_PWM_Used)
  tmr2_PWM_init(ST77xx_PWM_TMR2_Chan, 100, 0);
#else
  // Сигнал BLK
  gpio_PortClockStart(ST77xx_BLK_Port);
  GPIO_InitStruct.GPIO_Pin = ST77xx_BLK_Pin;
  GPIO_Init(ST77xx_BLK_Port, &GPIO_InitStruct);
  ST77xx_BLK_LOW();
#endif
}
//==============================================================================
