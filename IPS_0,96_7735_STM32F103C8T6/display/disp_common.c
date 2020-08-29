//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include <dispcolor.h>
#include <font.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#if (DISPCOLOR_type == DISPTYPE_st7789)
  #include <st77xx.h>
  #include <st7789.h>
#elif (DISPCOLOR_type == DISPTYPE_st7735)
  #include <st77xx.h>
  #include <st7735.h>
#else
  #error Работа c выбранным типом дисплея не поддерживается, попробуйте обновить модуль disp1color
#endif


//==============================================================================
// Процедура инициализирует цветной дисплей
//==============================================================================
void disp_Init(uint8_t Width, uint8_t Height)
{
#if (DISPCOLOR_type == DISPTYPE_st7789)
  // Инициализация дисплея
  ST7789_Init(Width, Height);
#elif (DISPCOLOR_type == DISPTYPE_st7735)
  // Инициализация дисплея
  ST7735_Init(Width, Height);
#endif
}
//==============================================================================


//==============================================================================
// Процедура устанавливает яркость дисплея
//==============================================================================
void disp_SetBrightness(uint8_t Value)
{
#if (DISPCOLOR_type == DISPTYPE_st7789)
  st77xx_SetBL(Value);
#elif (DISPCOLOR_type == DISPTYPE_st7735)
  st77xx_SetBL(Value);
#endif
}
//==============================================================================


//==============================================================================
// Процедура закрашивает экран цветом color
//==============================================================================
void disp_FillScreen(uint16_t color)
{
#if (DISPCOLOR_type == DISPTYPE_st7789)
  ST77xx_FillScreen(color);
#elif (DISPCOLOR_type == DISPTYPE_st7735)
  ST77xx_FillScreen(color);
#endif
}
//==============================================================================


//==============================================================================
// Процедура окрашивает 1 пиксель дисплея
//==============================================================================
void disp_DrawPixel(int16_t x, int16_t y, uint16_t color)
{
#if (DISPCOLOR_type == DISPTYPE_st7789)
  ST77xx_DrawPixel(x, y, color);
#elif (DISPCOLOR_type == DISPTYPE_st7735)
  ST77xx_DrawPixel(x, y, color);
#endif
}
//==============================================================================


//==============================================================================
// Процедура заполнения прямоугольника цветом color
//==============================================================================
void disp_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
#if (DISPCOLOR_type == DISPTYPE_st7789)
  ST77xx_FillRect(x, y, w, h, color);
#elif (DISPCOLOR_type == DISPTYPE_st7735)
  ST77xx_FillRect(x, y, w, h, color);
#endif
}
//==============================================================================


//==============================================================================
// Процедура заполнения прямоугольной области из буфера (порядок заполнения сектора Y, X
//==============================================================================
void disp_DrawPartYX(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff)
{
#if (DISPCOLOR_type == DISPTYPE_st7789)
  ST77xx_DrawPartYX(x, y, w, h, pBuff);
#elif (DISPCOLOR_type == DISPTYPE_st7735)
  ST77xx_DrawPartYX(x, y, w, h, pBuff);
#endif
}
//==============================================================================


//==============================================================================
// Процедура заполнения прямоугольной области из буфера (порядок заполнения сектора X, Y
//==============================================================================
void disp_DrawPartXY(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff)
{
#if (DISPCOLOR_type == DISPTYPE_st7789)
  ST77xx_DrawPartXY(x, y, w, h, pBuff);
#elif (DISPCOLOR_type == DISPTYPE_st7735)
  ST77xx_DrawPartXY(x, y, w, h, pBuff);
#endif
}
//==============================================================================
