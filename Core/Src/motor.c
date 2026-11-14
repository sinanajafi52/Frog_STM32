/*
 * motor.c
 *
 *  Created on: Nov 13, 2026
 *      Author: Admin
 */
#include "motor.h"
#include "main.h"
#include "tim.h"

#define MAX_RPM 300
#define RPM_LIMIT 10

static uint32_t now_rpm;
static uint16_t target_rpm;
static uint8_t en;
static uint8_t dir;
static uint8_t running;


static void delay_us(uint16_t us)//microsec
{
	__HAL_TIM_SET_COUNTER(&htim3,0);
	while(__HAL_TIM_GET_COUNTER(&htim3)<us)
	{
		//wait
	}
}

void motor_init(void)
{
	HAL_TIM_Base_Start(&htim3);
	HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_SET); //DRIVER OFF
	HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_RESET); //CW
}

void motor_start(void)
{
	en=1;
}

void motor_stop(void)
{
	en=0;
}

void motor_set_dir(uint8_t new_dir)
{
	if(en||running) return;
	dir = new_dir;
	HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,dir ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void motor_set_rpm(uint16_t rpm)
{
	target_rpm = rpm;
	if(target_rpm > MAX_RPM) target_rpm = MAX_RPM;
}

void motor_update(void)
{
	static uint32_t lasttime = 0;
	uint32_t tick = HAL_GetTick();
	uint16_t wanted;
	if(tick - lasttime >=5 )
	{
		lasttime = tick;
		wanted= (en==1 ) ? target_rpm: 0;
		if(now_rpm> wanted)
		{
			if(wanted<RPM_LIMIT && now_rpm<RPM_LIMIT) now_rpm= wanted;
			else
			{
				now_rpm--;
			}
		}
		else if(now_rpm<wanted)
		{
			if(wanted<RPM_LIMIT && now_rpm<RPM_LIMIT) now_rpm= wanted;
			else if(now_rpm<RPM_LIMIT && wanted >=RPM_LIMIT)
			{
				now_rpm=RPM_LIMIT;
			}
			else
			{
				now_rpm++;
			}
		}
		if(now_rpm==0)
		{
			if(running)
			{
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0);
				HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_3);
				HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_SET);
				running =0;
			}
		}
		else
		{
			uint32_t steps = now_rpm *3200/60;
			__HAL_TIM_SET_AUTORELOAD(&htim2,1000000/steps-1);
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,10);
			if(running==0)
			{
				HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_RESET);
				delay_us(5);
				__HAL_TIM_SET_COUNTER(&htim2,0);
				HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
				running=1;
			}
		}
	}
}

uint8_t motor_is_running(void)
{
	return running;
}
