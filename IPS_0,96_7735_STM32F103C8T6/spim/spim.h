//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _SPIM_H
#define _SPIM_H

#include "../types.h"


// Тип указатель на функцию, вызываемую по окончании приёмо-передачи по SPI
typedef void (*spi_endhandler)(void);


// Процедура инициализации spi (SPI1 или SPI2) в режиме master
void spim_init(SPI_TypeDef* SPIx, uint8_t FastSpeed, uint8_t WordLen);
// Процедура инициализации spi в режиме master с пересылкой с помощью DMA
void spim_init_dma(SPI_TypeDef* SPIx, uint8_t WordLen, void (*func)(void));
// Процедура отправляет массив 16-битных слов
void SPI_send16b(SPI_TypeDef* SPIx, uint16_t *pBuff, uint16_t Len);
// Процедура отправляет массив 8-битных слов
void SPI_send8b(SPI_TypeDef* SPIx, uint8_t *pBuff, uint16_t Len);
// Процедура отправляет массив 8-битных слов с использованием прерываний SPI
void SPI_send8b_irq(SPI_TypeDef* SPIx, uint8_t *pBuff, uint16_t Len, void (*func)(void));
// Процедура отправляет массив 8-битных слов с помощью DMA
void SPI_send8b_dma(SPI_TypeDef* SPIx, uint8_t *pBuff, uint16_t Len, void (*func)(void));
// Процедура отправляет массив 16-битных слов
void SPI_recv16b(SPI_TypeDef* SPIx, uint16_t *pBuff, uint16_t Len);
// Процедура отправляет массив 8-битных слов
void SPI_recv8b(SPI_TypeDef* SPIx, uint8_t *pBuff, uint16_t Len);
// Процедура принимает массив 8-битных слов с использованием прерываний SPI
void SPI_recv8b_irq(SPI_TypeDef* SPIx, uint8_t *pBuff, uint16_t Len, void (*func)(void));
// Процедура принимает массив 8-битных слов с помощью DMA
void SPI_recv8b_dma(SPI_TypeDef* SPIx, uint8_t *pBuff, uint16_t Len, void (*func)(void));
// Процедура отправляет 1 байт и возвращает принятый байт
uint8_t SPI_SendRecvByte(SPI_TypeDef* SPIx, uint8_t TxByte);
// Процедура отправляет/принимает массив 8-битных слов с использованием прерываний
void SPI_SendRecv_irq(SPI_TypeDef* SPIx, uint8_t *pTxBuff, uint8_t *pRxBuff, uint16_t Len, void (*func)(void));
// Процедура отправляет/принимает массив 8-битных слов c помощью DMA
void SPI_SendRecv_dma(SPI_TypeDef* SPIx, uint8_t *pTxBuff, uint8_t *pRxBuff, uint16_t Len, void (*func)(void));

#endif