/*
 * reset_reason.c
 *
 *  Created on: Nov 24, 2026
 *      Author: Admin
 */
#include "iwdg.h"
#include "reset_reason.h"
#include "main.h"

static const char *reset_reason = "UNKNOWN";

void reset_reason_init(void) {
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) {
		reset_reason = "WATCHDOG";
	} else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)) {
		reset_reason = "SOFTWARE";
	} else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST)) {
		reset_reason = "POWER";
	} else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)) {
		reset_reason = "BUTTON";
	} else {
		reset_reason = "UNKNOWN";
	}
	__HAL_RCC_CLEAR_RESET_FLAGS();
}
const char *reset_reason_get(void) {
	return reset_reason;
}

