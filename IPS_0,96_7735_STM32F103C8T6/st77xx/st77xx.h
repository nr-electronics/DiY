//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _ST77XX_H
#define _ST77XX_H

#include <types.h>


// Используемый SPI (SPI1, SPI2)
#define ST77xx_SPI_periph       SPI1
#define ST77xx_RESET_Used       1       // Используется ножка RESET дисплея
#define ST77xx_CS_Used          1       // Используется ножка CS дисплея. Можно установить в 0 если дисплей один и на CS лог. 1
#define ST77xx_DC_Used          1       // Используется ножка DC (Data/Command) дисплея. 0 если используется 3-проводной режим подключения по SPI
#define ST77xx_BLK_PWM_Used     1       // Используется плавное управление яркостью подсветки используя ШИМ (иначе управление дискретное - ВКЛ/ВЫКЛ)

//==============================================================================
// Настройки подключения к интерфейсу дисплея
//==============================================================================
// Сигнал RESET
#if (ST77xx_RESET_Used)
  #define ST77xx_RESET_Port     GPIOA
  #define ST77xx_RESET_Pin      GPIO_Pin_3
#endif
// Сигнал CS
#if (ST77xx_CS_Used)
  #define ST77xx_CS_Port        GPIOB
  #define ST77xx_CS_Pin         GPIO_Pin_10
#endif
// Сигнал DC
#if (ST77xx_DC_Used)
  #define ST77xx_DC_Port        GPIOA
  #define ST77xx_DC_Pin         GPIO_Pin_2
#endif

#if (ST77xx_BLK_PWM_Used)
// Используется 1 канал таймера 2 для управления с помощью ШИМ
  #define ST77xx_PWM_TMR2_Chan  1       // Номер канала (1-4) таймера 2, используемый для ШИМ
#else
  // Сигнал BLK (для управления подсветкой)
  #define ST77xx_BLK_Port       GPIOA
  #define ST77xx_BLK_Pin        GPIO_Pin_1
#endif
//==============================================================================


// Набор команд
//#define ST7735_Cmd_NOP          0x00
#define ST77xx_Cmd_SWRESET      0x01
//#define ST7735_Cmd_RDDID        0x04
//#define ST7735_Cmd_RDDST        0x09
#define ST77xx_Cmd_SLPIN        0x10
#define ST77xx_Cmd_SLPOUT       0x11
#define ST77xx_Cmd_PTLON        0x12
#define ST77xx_Cmd_NORON        0x13
#define ST77xx_Cmd_INVOFF       0x20
#define ST77xx_Cmd_INVON        0x21
#define ST77xx_Cmd_GAMSET       0x26
#define ST77xx_Cmd_DISPOFF      0x28
#define ST77xx_Cmd_DISPON       0x29
#define ST77xx_Cmd_CASET        0x2A
#define ST77xx_Cmd_RASET        0x2B
#define ST77xx_Cmd_RAMWR        0x2C
//#define ST7735_Cmd_RAMRD        0x2E
#define ST77xx_Cmd_PTLAR        0x30
#define ST77xx_Cmd_COLMOD       0x3A
#define ST77xx_Cmd_MADCTL       0x36    // Memory data access control 

#define ST7735_Cmd_FRMCTR1      0xB1    // Frame Rate Control in normal mode
#define ST7735_Cmd_FRMCTR2      0xB2    // Frame Rate Control in idle mode
#define ST7735_Cmd_FRMCTR3      0xB3    // Frame Rate Control in partial mode
#define ST7735_Cmd_INVCTR       0xB4
#define ST7735_Cmd_DISSET5      0xB6    // Display Function set 5 
#define ST7735_Cmd_PWCTR1       0xC0    // Power control 1 
#define ST7735_Cmd_PWCTR2       0xC1    // Power control 2 
#define ST7735_Cmd_PWCTR3       0xC2    // Power control 3 
#define ST7735_Cmd_PWCTR4       0xC3    // Power control 4 
#define ST7735_Cmd_PWCTR5       0xC4    // Power control 5 
#define ST7735_Cmd_VMCTR1       0xC5    // VCOM Control 1

#define ST7789_Cmd_MADCTL_MY    0x80
#define ST7789_Cmd_MADCTL_MX    0x40
#define ST7789_Cmd_MADCTL_MV    0x20
#define ST7789_Cmd_MADCTL_ML    0x10
#define ST7789_Cmd_MADCTL_RGB   0x00

#define ST7789_Cmd_RDID1        0xDA
#define ST7789_Cmd_RDID2        0xDB
#define ST7789_Cmd_RDID3        0xDC
#define ST7789_Cmd_RDID4        0xDD
//==============================================================================
#define ST7735_ColorMode_12bit  0x03
#define ST7735_ColorMode_16bit  0x05
#define ST7735_ColorMode_18bit  0x06

#define ST77xx_MADCTL_MY        0x80
#define ST77xx_MADCTL_MX        0x40
#define ST77xx_MADCTL_MV        0x20
#define ST77xx_MADCTL_ML        0x10
#define ST77xx_MADCTL_BGR       0x08
#define ST77xx_MADCTL_MH        0x04



extern uint8_t ST77xx_Width, ST77xx_Height;
extern uint8_t ST77xx_X_Start, ST77xx_Y_Start;

// Процедура настройки ножек микроконтроллера для обмена с ST77xx
void ST77xx_GPIO_init(void);
// Процедура отправки команды в дисплей
void ST77xx_SendCmd(uint8_t Cmd);
// Процедура отправки данных (параметров) в дисплей
void ST77xx_SendData(uint8_t Data);
// Процедура аппаратного сброса дисплея (ножкой RESET)
void ST77xx_HardReset(void);
// Процедура программного сброса дисплея
void ST77xx_SoftReset(void);
// Процедура включения/отключения режима сна
void ST77xx_SleepMode(uint8_t Mode);
// Процедура включения/отключения режима частичного заполнения экрана
void ST77xx_InversionMode(uint8_t Mode);
// Процедура включения/отключения питания дисплея
void ST77xx_DisplayPower(uint8_t On);
// Процедура выбора кривой гамма-коррекции
void ST77xx_GammaSet(uint8_t CurveNum);
// Процедура настройки формата цвета
void ST77xx_ColorModeSet(uint8_t ColorMode);
// Процедура настройки отображения  
void ST77xx_MemAccessModeSet(uint8_t Rotation, uint8_t VertMirror, uint8_t HorizMirror, uint8_t IsBGR);
// Процедура управления подсветкой (без поддержки ШИМ)
void st77xx_SetBL(uint8_t Value);
// Процедура установка границ экрана для заполнения
void ST77xx_SetWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
// Процедура закрашивает экран цветом color
void ST77xx_FillScreen(uint16_t color);
// Процедура окрашивает 1 пиксель дисплея
void ST77xx_DrawPixel(int16_t x, int16_t y, uint16_t color);
// Процедура заполнения прямоугольника цветом color
void ST77xx_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
// Процедура заполнения прямоугольной области из буфера. Порядок заполнения экрана Y - X
void ST77xx_DrawPartYX(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff);
// Процедура заполнения прямоугольной области из буфера. Порядок заполнения экрана X - Y
void ST77xx_DrawPartXY(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff);


#endif
