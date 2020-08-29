//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "main.h"
#include <string.h>
#include <dispcolor.h>
#include <font.h>
#include <ff.h>
#include <diskio.h>
#include <timers.h>
#include <delay.h>
#include <button.h>
#include <test.h>


// Дисплей (160x80 или 240x240)
#define DISPLAY         DISPLAY_160x80  //DISPLAY_160x80 DISPLAY_240x240


#if (DISPLAY == DISPLAY_160x80)
  #include <test80x160.h>
#else
  #include <test240x240.h>
#endif

//==============================================================================
//
//==============================================================================
void main()
{
  SystemInit();
  
#if (DISPLAY == DISPLAY_160x80)
  // Инициализация дисплея
  dispcolor_Init(160, 80);
  // Инициализация входа для кнопки
  button_Init();
  // Тест вывода основных цветов
  Test_Colors();
  // Тест вывода приглашения с плавным затуханием
  Test_TextFading("Hello from\r\nNR.electronics", 30, 23);
  // Тест вывода текста разными шрифтами
  Test80x160_Text();
  // Тест вывода простой 2D-графики
  Test80x160_Graphics();

  while (1)
  {
    // Вывод картинок с SD-флешки
    Test80x160_Images();
  }
#else
  // Инициализация дисплея
  dispcolor_Init(240, 240);
  // Инициализация входа для кнопки
  button_Init();
  // Тест вывода основных цветов
  Test_Colors();
  // Тест вывода приглашения с плавным затуханием
  Test_TextFading("Hello from\r\nNR.electronics", 75, 100);
  // Тест вывода текста разными шрифтами
  Test240x240_Text();
  // Тест вывода простой 2D-графики
  Test240x240_Graphics();

  while (1)
  {
    // Вывод картинок с SD-флешки
    Test240x240_Images();
  }
#endif
}
//==============================================================================
