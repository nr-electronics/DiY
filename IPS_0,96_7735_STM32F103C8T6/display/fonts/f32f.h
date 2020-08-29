//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _F32F_H
#define _F32F_H

#include <types.h>


// Шрифт пропорциональный, высота 32
#define f32_FLOAT_HEIGHT        32

// Кол-во символов в таблице шрифта
#define f32f_NOFCHARS           10


// Функция возвращает указатель на подтаблицу символа Char
uint8_t *f32f_GetCharTable(uint8_t Char);

#endif 
