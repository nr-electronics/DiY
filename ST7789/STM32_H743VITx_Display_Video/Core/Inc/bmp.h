/*
 * bmp.h
 *
 *  Created on: Nov 12, 2021
 *      Author: Pavel
 */

#ifndef __bmp_H
#define __bmp_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

 /* Error code */
typedef enum {
   IMG_OK = 0,         /* 0: Succeeded */
   IMG_FIL_ERR,        /* 1: Ошибка открытия файла */
   IMG_NOT_BMP,        /* 2: Это не BMP файл */
   IMG_UNKNOWN_FORMAT, /* 3: Неизвестный формат */
 } IMGRESULT;




 //#pragma pack(push,1)
 typedef  struct
 {
   uint16_t bfType;      // - Отметка для отличия формата от других (0x4D42) (Signature)
   uint32_t bfSize;      // - Размер файла в байтах
   uint16_t bfReserved1; // - зарезервированны =0
   uint16_t bfReserved2; // - зарезервированны =0
   uint32_t bfOffBits;   // - Положение пиксельных данных относительно начала данной структуры (в байтах).
 } __attribute__((packed)) BitMapFileHeader;//
 //#pragma pack(pop)

 //#pragma pack(push,1)
 typedef struct
 {
   uint32_t biSize;      // - Размер данной структуры в байтах, указывающий также на версию структуры
   uint32_t biWidth;     // - Ширина растра в пикселях. Указывается целым числом со знаком. Ноль и отрицательные не документированы.
   int32_t  biHeight;    // - Целое число со знаком, содержащее два параметра: высота растра в пикселях (абсолютное значение числа) и порядок следования строк в двумерных массивах (знак числа).
   uint16_t biPlanes;    // - В BMP допустимо только значение 1. Это поле используется в значках и курсорах Windows.
   uint16_t biBitCount;  // - Количество бит на пиксель
   uint32_t biCompression; // - Указывает на способ хранения пикселей
   uint32_t biSizeImage; // - Размер пиксельных данных в байтах. Может быть обнулено если хранение осуществляется двумерным массивом.
   uint32_t biXPelsPerMeter;// - Количество пикселей на метр по горизонтали
   uint32_t biYPelsPerMeter;// - по вертикали
   uint32_t biClrUsed;    // - Размер таблицы цветов в ячейках.
   uint32_t biClrImportant; // - Количество ячеек от начала таблицы цветов до последней используемой (включая её саму).
 } __attribute__((packed)) BitMapInfoHeader;
 //#pragma pack(pop)







// Показать картнку BMP из файла (указан верхний левый угол)
IMGRESULT DrawBMPImageFile(const char* fname, uint16_t x_pos, uint16_t y_pos);


#ifdef __cplusplus
}
#endif

#endif /* __bmp_H */
