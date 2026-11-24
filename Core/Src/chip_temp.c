/*
 * chip_temp.c
 *
 *  Created on: Nov 24, 2026
 *      Author: Admin
 */
#include "main.h"
#include "adc.h"
#include "chip_temp.h"

static int32_t t10;
static uint32_t vdda;

void chip_temp_init(void) {
	HAL_ADCEx_Calibration_Start(&hadc1);
}
void chip_temp_update(void) {
	static uint32_t last_print = 0;

	if (HAL_GetTick() - last_print >= 1000) {
		last_print = HAL_GetTick();
		int32_t vref_raw = 0;
		uint32_t temp_raw = 0;

		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
			temp_raw = HAL_ADC_GetValue(&hadc1);
		}
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
			vref_raw = HAL_ADC_GetValue(&hadc1);
		}
		HAL_ADC_Stop(&hadc1);
		if(vref_raw>0)
		{
		vdda = 1200 * 4095 / vref_raw;
		uint32_t sense_mv = temp_raw * vdda / 4095;
		t10 = (1430 - (int32_t) sense_mv) * 100 / 43 + 250;
		}
	}

}
int32_t chip_temp_get_t10(void) {
	return t10;
}
uint32_t chip_temp_get_vdda(void) {
	return vdda;
}
