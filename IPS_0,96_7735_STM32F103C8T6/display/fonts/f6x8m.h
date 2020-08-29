//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _F6X8M_H
#define _F6X8M_H

#include <types.h>


// Шрифт моноширинный, 6х8 пикселей
#define f6x8_MONO_WIDTH         6
#define f6x8_MONO_HEIGHT        8

// Кол-во символов в таблице шрифта
#define f6x8m_NOFCHARS           256


// Функция возвращает указатель на подтаблицу символа Char
uint8_t *f6x8m_GetCharTable(uint8_t Char);

#endif 
