//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _TMR2_H
#define _TMR2_H

#include <stdint.h>


// Процедура настраивает таймер
void tmr2_init(uint32_t Freq, void (*func)(void));
// Процедура стартует таймер
void tmr2_start(void);
// Процедура останавливает таймер
void tmr2_stop(void);
// Процедура настройки канала 1-4 таймера 2 в режиме ШИМ
void tmr2_PWM_init(uint8_t Channel, uint16_t MaxValue, uint16_t Value);
// Процедура установки значения ШИМ для канала 1-4 таймера 2
void tmr2_PWM_set(uint8_t Channel, uint16_t Value);

#endif