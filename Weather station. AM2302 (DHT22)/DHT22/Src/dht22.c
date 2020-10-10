#include "dht22.h"
//--------------------------------------------------
__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
micros *= (SystemCoreClock / 1000000) / 9;
/* Wait till done */
while (micros--) ;
}
//--------------------------------------------------
void port_init(void)
{
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);
  GPIOB->CRH |= GPIO_CRH_MODE11;
  GPIOB->CRH |= GPIO_CRH_CNF11_0;
  GPIOB->CRH &= ~GPIO_CRH_CNF11_1;
}
//--------------------------------------------------
uint8_t dht22_init(void)
{
	HAL_Delay(2000);
  GPIOB->ODR |= GPIO_ODR_ODR11;//высокий уровень
  return 0;
}
//----------------------------------------------------------
uint8_t dht22_GetData(uint8_t *data)
{
	uint8_t i, j = 0;
	//reset port
	GPIOB->ODR &= ~GPIO_ODR_ODR11;//низкий уровень
	GPIOB->ODR |= GPIO_ODR_ODR11;//высокий уровень
	DelayMicro(100000);
	GPIOB->ODR &= ~GPIO_ODR_ODR11;//низкий уровень
	DelayMicro(18000);
	GPIOB->ODR |= GPIO_ODR_ODR11;//высокий уровень
	//дождемс€ ответа датчика
	DelayMicro(39);//20-40 мкс
	//если датчик нам не ответил прит€гиванием шины, то ошибка
	if(GPIOB->IDR & GPIO_IDR_IDR11) {
		return 0;
	}
	DelayMicro(80);
	//если датчик не отпустил шину, то ошибка
	if(!(GPIOB->IDR & GPIO_IDR_IDR11)) {
		return 0;
	}
	DelayMicro(80);
	//читаем данные (записываем байты в массив наоборот,
	//так как сначала передаЄтс€ старший, чтобы потом
	//не переворачивать двухбайтовый результат)
	for (j=0; j<5; j++)
	{
		data[4-j]=0;
		for(i=0; i<8; i++)
		{
			while(!(GPIOB->IDR & GPIO_IDR_IDR11)); //ждЄм отпускани€ шины
			DelayMicro(30);
			if(GPIOB->IDR & GPIO_IDR_IDR11) //читаем результат по прошествии 30 микросекунд
				//если шина за это врем€ не прит€нулась к земле, то значит это единица, а если прит€нулась, то ноль
				data[4-j] |= (1<<(7-i));
			while(GPIOB->IDR & GPIO_IDR_IDR11); //ждЄм, пока датчик прит€нет шину (в случае единицы)
		}
	}
	return 1;	
}
//----------------------------------------------------------
