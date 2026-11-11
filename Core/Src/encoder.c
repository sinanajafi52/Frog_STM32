/*
 * encoder.c
 *
 *  Created on: Nov 10, 2026
 *      Author: Admin
 */
#include "encoder.h"
#include "main.h"
static volatile int32_t counter=0;

int32_t encoder_get_counter(void)
{
	return counter;
}

uint8_t encoder_get_sw(void)
{
	return (HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin)==0);
}

void encoder_set_counter(uint32_t value)
{
	counter=value;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
	{
		static uint32_t lasttime=0;
		if(GPIO_Pin==CLK_Pin)
		{
			if(HAL_GetTick()-lasttime >10)
			{
				lasttime=HAL_GetTick();
				if(HAL_GPIO_ReadPin(DT_GPIO_Port ,DT_Pin))
				{
					counter--;
				}
				else{
					counter++;
				}
			}
		}
	}
