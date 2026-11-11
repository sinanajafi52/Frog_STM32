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

uint8_t encoder_button_press(void)
{
	uint8_t SW;
	static uint8_t lastsw = 0;
	static uint32_t lastupdate = 0;
	uint32_t tick = HAL_GetTick();
	SW= (HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin)==0);
	if(SW!=lastsw && tick-lastupdate>50)
	{
		lastsw = SW;
		lastupdate= tick;
		if(SW)
		{
			return 1;
		}
	}
	return 0;
}

void encoder_set_counter(int32_t value)
{
	counter=value;
}
//callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
	{
		static uint32_t lasttime=0;
		if(GPIO_Pin==CLK_Pin)
		{
			uint32_t tick = HAL_GetTick();
			if(tick-lasttime >10)
			{
				lasttime=tick;
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
