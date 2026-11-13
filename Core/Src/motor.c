/*
 * motor.c
 *
 *  Created on: Nov 13, 2026
 *      Author: Admin
 */
#include "motor.h"
#include "main.h"
#include "tim.h"

static uint32_t now_rpm;
static uint16_t target_rpm;
static uint8_t en;
static uint8_t dir;
static uint8_t flipping;
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
	flipping = (new_dir==dir) ? 0:1;
}

void motor_set_rpm(uint16_t rpm)
{
	target_rpm = rpm;
	if(target_rpm > 300) target_rpm = 300;
}

void motor_update(void)
{
	static uint32_t lasttime = 0;
}

uint8_t motor_is_running(void)
{
	return running;
}
