/*
 * dispenser.c
 *
 *  Created on: Nov 20, 2026
 *      Author: Admin
 */
#include "motor.h"
#include "main.h"
#include "display.h"
#include "dispenser.h"
#include "encoder.h"
#include <stdint.h>
#include <stdio.h>

typedef enum {
	SELECTING, EDIT_RPM, EDIT_ON, EDIT_OFF, RUN_OFF, RUN_ON
} Dispenserstate;
static Dispenserstate state = SELECTING;
static uint16_t rpm = 150;
static uint8_t on_time = 1;
static uint8_t off_time = 1;
static int8_t laston = -1; //
static int8_t lastoff = -1; //
static int16_t lastrpm = -1; //
static int8_t laststate = -1; //
static int8_t lastblink = -1; //
static int8_t lastselecting = -1; //
static uint32_t state_start;

static void draw(int32_t selecting, uint8_t blink) {
	display_clear();
	char buf_rpm[20], buf_on[20], buf_off[20];
	snprintf(buf_rpm, sizeof(buf_rpm), "RPM:  %u", rpm);
	snprintf(buf_on, sizeof(buf_on), "ON-TIME:  %u", on_time);
	snprintf(buf_off, sizeof(buf_off), "OFF-TIME:  %u", off_time);
	switch (state) {
	case SELECTING:
		display_item(5, 12, buf_rpm, selecting == 0);
		display_item(5, 26, buf_on, selecting == 1);
		display_item(5, 40, buf_off, selecting == 2);
		display_item(5, 58, "BACK", selecting == 3);
		display_item(75, 58, "START", selecting == 4);
		break;
	case EDIT_RPM:
		if (blink)
			display_item(5, 12, buf_rpm, 1);
		display_item(5, 26, buf_on, 0);
		display_item(5, 40, buf_off, 0);
		display_item(5, 58, "BACK", 0);
		display_item(75, 58, "START", 0);
		break;
	case EDIT_ON:
		display_item(5, 12, buf_rpm, 0);
		if (blink)
			display_item(5, 26, buf_on, 1);
		display_item(5, 40, buf_off, 0);
		display_item(5, 58, "BACK", 0);
		display_item(75, 58, "START", 0);
		break;
	case EDIT_OFF:
		display_item(5, 12, buf_rpm, 0);
		display_item(5, 26, buf_on, 0);
		if (blink)
			display_item(5, 40, buf_off, 1);
		display_item(5, 58, "BACK", 0);
		display_item(75, 58, "START", 0);
		break;
	case RUN_ON:
	case RUN_OFF:
		display_item(5, 12, buf_rpm, 0);
		display_item(5, 26, buf_on, 0);
		display_item(5, 40, buf_off, 0);
		if (blink)
			display_item(45, 58, "STOP", 1);
		break;
	}
	display_send();
}
void dispensr_enter(void) {
	state = SELECTING;
	encoder_set_counter(0);
	laststate = -1;
}
uint8_t dispenser_update(uint8_t click) {
	int32_t count = encoder_get_counter();
	int32_t selecting = count % 5;
	if (selecting < 0)
		selecting += 5;
	uint8_t blink = (HAL_GetTick() / 500) % 2;
	switch (state) {
	case SELECTING:
		if (click) {
			if (selecting == 0) {
				state = EDIT_RPM;
				encoder_set_counter(rpm);
			}
			if (selecting == 1) {
				state = EDIT_ON;
				encoder_set_counter(on_time);
			}
			if (selecting == 2) {
				state = EDIT_OFF;
				encoder_set_counter(off_time);
			}
			if (selecting == 3) {
				return 1;
			}
			if (selecting == 4) {
				motor_set_rpm(rpm);
				motor_start();
				state = RUN_ON;
				state_start = HAL_GetTick();
			}
		}
		break;
	case EDIT_RPM:
		if (count > 300) {
			count = 300;
			encoder_set_counter(300);
		}
		if (count < 0) {
			count = 0;
			encoder_set_counter(0);
		}

		rpm = count;
		if (click) {
			state = SELECTING;
			encoder_set_counter(0);
		}
		break;
	case EDIT_ON:
		if (count > 10) {
			count = 10;
			encoder_set_counter(10);
		}
		if (count < 1) {
			count = 1;
			encoder_set_counter(1);
		}
		on_time = count;
		if (click) {
			state = SELECTING;
			encoder_set_counter(1);
		}
		break;
	case EDIT_OFF:
		if (count > 10) {
			count = 10;
			encoder_set_counter(10);
		}
		if (count < 1) {
			count = 1;
			encoder_set_counter(1);
		}
		off_time = count;
		if (click) {
			state = SELECTING;
			encoder_set_counter(2);
		}
		break;
	case RUN_ON:
		if (HAL_GetTick() - state_start >= on_time * 1000) {
			motor_stop();
			state = RUN_OFF;
			state_start = HAL_GetTick();
		}
		if (click) {
			state = SELECTING;
			motor_stop();
			encoder_set_counter(4);
		}
		break;
	case RUN_OFF:
		if (HAL_GetTick() - state_start >= off_time * 1000) {
			motor_set_rpm(rpm);
			motor_start();
			state = RUN_ON;
			state_start = HAL_GetTick();
		}
		if (click) {
			state = SELECTING;
			motor_stop();
			encoder_set_counter(4);
		}
		break;
	}
	if ((state != laststate || blink != lastblink || rpm != lastrpm
			|| on_time != laston || off_time != lastoff
			|| selecting != lastselecting)) {
		draw(selecting, blink);
		laststate = state;
		lastblink = blink;
		lastrpm = rpm;
		laston = on_time;
		lastoff = off_time;
		lastselecting = selecting;
	}
	return 0;
}

