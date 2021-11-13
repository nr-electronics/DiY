/*
 * bmp.c
 *
 *  Created on: Nov 12, 2021
 *      Author: Ulmer Pavel
 */
#include "bmp.h"
#include "fatfs.h"
#include "st7789.h"

#define BMP_HEADER_SIZE     34

FIL img_file;

//Массив байт заголовка BMP файла
static uint8_t header[BMP_HEADER_SIZE];
static BitMapFileHeader* BmpFileHaeder;
static BitMapInfoHeader* BmpFileInfo;

IMGRESULT DrawBMPImageFile(const char* fname, uint16_t x_pos, uint16_t y_pos)
{
  FRESULT res;
  UINT bytesRead;
  res = f_open(&img_file, fname, FA_READ);
  if (res != FR_OK){
    f_close(&img_file);
    return IMG_FIL_ERR;
  }

  res = f_read(&img_file, (void*)header, (UINT)BMP_HEADER_SIZE, &bytesRead);
  if (res != FR_OK){
    f_close(&img_file);
    return IMG_FIL_ERR;
  }

  BmpFileHaeder = (void*)header;
  BmpFileInfo   = (void*)(&header[0x0E]);

  if ( BmpFileHaeder->bfType != 0x4D42 ){
    f_close(&img_file);
    return IMG_NOT_BMP;
  }

  //Проверка, подходит ли нам такой формат компрессии
  if((BmpFileInfo->biPlanes != 1) || (BmpFileInfo->biBitCount != 24) || (BmpFileInfo->biCompression != 0)){
    f_close(&img_file);
    return IMG_UNKNOWN_FORMAT;
  }

  //сохраняем значения ширины и высоты изображения
  uint32_t imageWidth = BmpFileInfo->biWidth;
  int32_t  imageHeight = BmpFileInfo->biHeight;
  int8_t direction = -1;
  if(imageHeight<0){
    imageHeight *= -1;
    direction = 1;
  }

  //передвинем указатель на начало битового поля
  res = f_lseek(&img_file, BmpFileHaeder->bfOffBits);//imageOffset);
  if(res != FR_OK){
    f_close(&img_file);
    return IMG_FIL_ERR;
  }


  //Массив байт для чтения из флеши
  static uint8_t imageRow[(240 * 3 + 3) & ~3];

  //Массив байт для одной горизонтальной линии, для загрузки в дисплей
  static uint16_t PixBuff[240];


  uint32_t y_start;
  uint32_t y_end;
  if(direction == -1){
    /* Рисуем строки снизу вверх */
    y_start = imageHeight+y_pos-1;
    y_end = y_pos-1;
  }
  else{
    /* Рисуем строки сверху вниз вверх */
    y_start = y_pos-1;
    y_end = imageHeight+y_pos-1;
  }

  /* Рисуем строки */
  for (uint32_t y=y_start; y!=y_end; y+=direction)
  {
    res = f_read(&img_file, imageRow, (imageWidth * 3 + 3) & ~3, &bytesRead);
    if (res != FR_OK)
    {
      f_close(&img_file);
      return IMG_FIL_ERR;
    }

    //Подготавливаем буфер строки картинки для вывода
    //перепаковка из формата BMP24 бита в формат RGB565
    //imageRow и PixBuff ссылаются на один буфер, но так как результат перепаковки уменьшается,
    //то так поступить можно, уменьшая затраченную памть
    uint32_t rowIdx = 0;
    for (uint32_t x = 0; x < imageWidth; x++)
    {
      uint8_t b = imageRow[rowIdx++];
      uint8_t g = imageRow[rowIdx++];
      uint8_t r = imageRow[rowIdx++];
      PixBuff[x] = RGB565(r, g, b);

      //свапаем пиксель
      uint16_t temp = PixBuff[x];
      PixBuff[x] = PixBuff[x] >> 8;
      PixBuff[x] |= (temp << 8);
    }
    // Рисуем полученную строку
    ST7789_DrawImage( x_pos, y, imageWidth, 1, PixBuff );
  }

  f_close(&img_file);
  return IMG_OK;
}
