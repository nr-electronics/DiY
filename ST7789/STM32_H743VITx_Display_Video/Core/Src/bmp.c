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
//static uint8_t header[BMP_HEADER_SIZE];
void* header;
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

  void *bmp_file_bufer = malloc(img_file.obj.objsize);
  if(bmp_file_bufer == NULL)
    return NOT_ENOUGH_SPACE;

  res = f_read(&img_file, (void*)bmp_file_bufer, img_file.obj.objsize, &bytesRead);

  if (res != FR_OK || bytesRead != img_file.obj.objsize){
      f_close(&img_file);
      return IMG_FIL_ERR;
    }
  f_close(&img_file);

  BmpFileHaeder = bmp_file_bufer;
  BmpFileInfo   = bmp_file_bufer + 0x0E;

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

  //Выделяем буфер для картинки
  void *bmp_frame_bufer = malloc(imageWidth*imageHeight*sizeof(uint16_t));
  if(bmp_frame_bufer == NULL)
    return NOT_ENOUGH_SPACE;

  //передвинем указатель на начало битового поля
  uint8_t* pix_pointer = (uint8_t*) (bmp_file_bufer + BmpFileHaeder->bfOffBits);

  int32_t y_start;
  int32_t y_end;
  if(direction == -1){
    /* Рисуем строки снизу вверх */
    y_start = imageHeight;
    y_end = 0;
  }
  else{
    /* Рисуем строки сверху вниз вверх */
    y_start = 0;
    y_end = imageHeight;
  }

  /* Рисуем строки */
  for (int32_t y=y_start; y!=y_end; y+=direction)
  {
    //перепаковка из формата BMP24 бита в формат RGB565
    uint8_t* ppointer = (uint8_t*)pix_pointer;
    uint16_t* PixBuff = (uint16_t*)bmp_frame_bufer + y*imageWidth;
    uint16_t* PixBuff_end = PixBuff + imageWidth;
    while (PixBuff < PixBuff_end)
    {
      uint8_t b = *ppointer++;
      uint8_t g = *ppointer++;
      uint8_t r = *ppointer++;
      *PixBuff = RGB565(r, g, b);

      //свапаем пиксель
      *PixBuff = (*PixBuff>>8) | (*PixBuff<<8);

      PixBuff++;
    }

    pix_pointer += (imageWidth * 3 + 3) & ~3;
  }

  free(bmp_file_bufer);

  // Рисуем полученную картинку

  ST7789_DrawImage( x_pos, y_pos, imageWidth, imageHeight, (uint16_t*)bmp_frame_bufer );

  free(bmp_frame_bufer);

  return IMG_OK;
}
