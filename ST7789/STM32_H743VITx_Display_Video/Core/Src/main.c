/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "sdmmc.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7789.h"
#include "bitmap.h"
#include "fonts.h"
#include "bmp.h"

#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
FATFS fatfs;
DIR DirInfo;
FILINFO FileInfo;
char media_folder[] = "/media";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  // включаем подсветку дисплея
  HAL_GPIO_WritePin( GPIOB, GPIO_PIN_0, GPIO_PIN_SET );

  // Процедура инициализации дисплея ST7789 240x240 1.3"
  ST7789_Init();

  // установка ротации дисплея не обязательно так как по умолчанию стоит режим 1 ( всего 4 режима 1,2,3,4 )
  ST7789_rotation( 1 );


  // закрашиваем весь экран указаным цветом
  ST7789_FillScreen( RGB565(0, 10, 100) );


  // печатаем символ ( один ) параметры: х,  у,  цвет символа, цвет фона, вкл/выкл фон, размер шрифта, множитель шрифта (увеличивает в х раз шрифт ), сам символ
  // ST7789_DrawChar( 20, 20, RGB565( 255, 255, 255 ) , RGB565( 0, 10, 10 ) , 0, &Font_16x26, 3, 'F' );


  // печатаем строку параметры: х,  у,  цвет строки, цвет фона, вкл/выкл фон, размер шрифта, множитель шрифта (увеличивает в х раз шрифт ), сама строка
  // для кирилицы используем функцию utf8rus, если только латиница то можно без нее
  ST7789_print( 50, 20, RGB565(255, 255, 255) , RGB565(0, 10, 100) , 1, &Font_16x26, 1, utf8rus("STM32 TFT") );
  // незабываем каждый раз после вызова функции utf8rus() освобождать память
  // free( pText ); // освобождаем память выделенную в функции utf8rus() посредством malloc();
  free( pText );

  // печатаем строку параметры: х,  у,  цвет строки, цвет фона, вкл/выкл фон, размер шрифта, множитель шрифта (увеличивает в х раз шрифт ), сама строка
  // для кирилицы используем функцию utf8rus, если только латиница то можно без нее
  ST7789_print( 10, 160, RGB565(255, 0, 0) , RGB565(0, 10, 100) , 1, &Font_11x18, 1, utf8rus("Классный дисплей !!!") );
  // незабываем каждый раз после вызова функции utf8rus() освобождать память
  // free( pText ); // освобождаем память выделенную в функции utf8rus() посредством malloc();
  free( pText );

  // печатаем строку параметры: х,  у,  цвет строки, цвет фона, вкл/выкл фон, размер шрифта, множитель шрифта (увеличивает в х раз шрифт ), сама строка
  // для кирилицы используем функцию utf8rus, если только латиница то можно без нее
  ST7789_print( 8, 200, RGB565(0, 255, 0) , RGB565(0, 10, 100) , 1, &Font_7x9, 2, utf8rus("ST7789 : 240x240") );
  // незабываем каждый раз после вызова функции utf8rus() освобождать память
  // free( pText ); // освобождаем память выделенную в функции utf8rus() посредством malloc();
  free( pText );

  // очистка экрана - закрашивает экран цветом черный
  // ST7789_Clear();


  // рисуем монохромную иконку. параметры координаты х и у, имя иконки ( массив ), размер иконки шир и выс, цвет отображения
  // ST7789_DrawBitmap( 60, 120, logo, 128, 27, RGB565(255, 0, 0) );


  // рисуем цветную иконку. параметры координаты х и у, размер иконки шир и выс, имя иконки ( массив )
  ST7789_DrawImage( 80, 65, 85, 80, logoRGB );


  // круг пустотелый
  //ST7789_DrawCircle(100, 100, 50, RGB565(255, 0, 255));


  // круг закрашеный
  //ST7789_DrawCircleFilled(200, 200, 20, RGB565(255, 40, 255)) ;


  // линия
  ST7789_DrawLine(1, 238, 239, 238, RGB565(255, 255, 0));


  // прямоугольник закрашеный
  //ST7789_DrawRectangleFilled(70, 200, 100, 20, RGB565(0, 255, 0)) ;


  // прямоугольник пустотелый
  //ST7789_DrawRectangle(70, 200, 100, 20, RGB565(0, 255, 50)) ;



  // рисуем треугольник пустотелый
  //ST7789_DrawTriangle(20, 40, 150, 100, 200, 200, RGB565(100, 255, 150) );


  // рисуем треугольник закрашеный
  //ST7789_DrawFilledTriangle(20, 40, 150, 100, 200, 200, RGB565(100, 255, 150) );


  // переход в спящий режим
  //ST7789_SleepModeEnter();


  // выход их спящего режима
  //ST7789_SleepModeExit();



  // вкл/выкл дисплея 0-выкл 1- вкл
  //ST7789_DisplayPower( 1 );


  // инверсия цветов 0-вкл  1-выкл
  //ST7789_InversionMode(1);


  FRESULT res;

//--
  HAL_Delay(2000);
  ST7789_Clear();
  ST7789_print( 0, 0, RGB565(255, 255, 255) , RGB565(0, 0, 0) , 1, &Font_11x18, 1, "Mount SD .. " );
  res = f_mount(&fatfs, SDPath, 1);
  if(res == FR_OK){
    ST7789_print( 150, 0, RGB565(255, 255, 255) , RGB565(0, 0, 0) , 1, &Font_11x18, 1, "OK" );
  }
  else{
    ST7789_print( 150, 0, RGB565(255, 255, 255) , RGB565(0, 0, 0) , 1, &Font_11x18, 1, "fail" );
    while(1)
    {}
  }
//--
  ST7789_print( 0, 20, RGB565(255, 255, 255) , RGB565(0, 0, 0) , 1, &Font_11x18, 1, "Open dir:" );
  ST7789_print( 150, 20, RGB565(255, 255, 255) , RGB565(0, 0, 0) , 1, &Font_11x18, 1, media_folder);
  res = f_chdir(media_folder);
  if(res == FR_OK){
    ST7789_print( 0, 40, RGB565(255, 255, 255) , RGB565(0, 0, 0) , 1, &Font_11x18, 1, "OK" );
  }
  else{
    ST7789_print( 0, 40, RGB565(255, 255, 255) , RGB565(0, 0, 0) , 1, &Font_11x18, 1, "fail" );
    while(1)
    {}
  }
//--
  while (1)
    {
      /* Получаем информацию о содержимом каталога */
      res = f_opendir(&DirInfo, "");

      while (1)
      {
        res = f_readdir(&DirInfo, &FileInfo);
        if (FileInfo.fname[0] == 0)// если нет файлов
          break;
        /* Находим имя файла */
        char* dot; //указатель на точку в имени файла
        if((dot = strrchr(FileInfo.fname, '.')) == 0) //если нет точки в имени
          break;

        if(strncmp(dot,".bmp",4) == 0)
        {
          DrawBMPImageFile(FileInfo.fname, 0, 0);
          //ST7789_print( 150, 100, RGB565(255, 255, 255) , RGB565(0, 0, 0) , 1, &Font_11x18, 1, FileInfo.fname);
        }
        HAL_Delay(1000);
      }
      f_closedir(&DirInfo);
    }









  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
