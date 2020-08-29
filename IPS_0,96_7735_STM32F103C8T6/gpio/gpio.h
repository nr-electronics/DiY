//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _GPIO_H
#define _GPIO_H

#include "../types.h"

#define gpio_NoPull     0
#define gpio_PullUp     1
#define gpio_PullDown   2


void gpio_SetGPIOmode_In(GPIO_TypeDef* GPIOx, uint16_t Mask, uint8_t PullMode);
void gpio_SetGPIOmode_Out(GPIO_TypeDef* GPIOx, uint16_t Mask);
void gpio_PortClockStart(GPIO_TypeDef *GPIOx);

#endif