//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _DISP_COMMON_H
#define _DISP_COMMON_H

#include <types.h>

// Процедура инициализирует цветной дисплей
void disp_Init(uint8_t Width, uint8_t Height);
// Процедура устанавливает яркость дисплея
void disp_SetBrightness(uint8_t Value);
// Процедура очищает экран (закрашивает чёрным)
void disp_ClearScreen(void);
// Процедура окрашивает 1 пиксель дисплея
void disp_DrawPixel(int16_t x, int16_t y, uint16_t color);
// Процедура заполнения прямоугольника цветом color
void disp_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
// Процедура закрашивает экран цветом color
void disp_FillScreen(uint16_t color);
// Процедура заполнения прямоугольной области из буфера (порядок заполнения сектора Y, X
void disp_DrawPartYX(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff);
// Процедура заполнения прямоугольной области из буфера (порядок заполнения сектора X, Y
void disp_DrawPartXY(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pBuff);


#endif