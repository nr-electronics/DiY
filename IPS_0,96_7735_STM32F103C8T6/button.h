//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef BUTTON
#define BUTTON


// Нога, к которой подключена кнопка
#define TestButton_Port       GPIOB
#define TestButton_Pin        GPIO_Pin_13


// Процедура инициализации ноги GPIO для подключения кнопки
void button_Init();
// Функция чтения состояния кнопки
uint8_t button_GetState(void);
// Процедура ожидания надатия (и отпускания) кнопки
void button_WaitPress(void);

#endif