/*
 * status.c
 *
 *  Created on: Nov 17, 2026
 *      Author: Admin
 */
#include "status.h"
#include "encoder.h"
#include <stdint.h>
#include <stdio.h>
#include "motor.h"
#include "display.h"
#include "main.h"

typedef enum {
	SELECTING, EDIT_RPM, EDIT_DIR, RUNNING
} Statusstate;
static Statusstate state = SELECTING;
static uint16_t rpm= 150;
static uint8_t dir;
static int8_t laststate = -1;
static int8_t lastblink = -1;
static int16_t lastrpm = -1;
static int8_t lastdir = -1;
static int8_t lastselecting = -1;

static void draw(int32_t selecting, uint8_t blink) {
	char buf[20];
	snprintf(buf, sizeof(buf), "RPM:  %u", rpm);
	display_clear();
	switch (state) {
	case SELECTING:
		display_item(5, 15, buf, (selecting == 0) ? 1 : 0);
		display_item(5, 30, (dir == 1) ? "DIR:  CW" : "DIR:  CCW",
				(selecting == 1) ? 1 : 0);
		display_item(5, 55, "BACK", (selecting == 2) ? 1 : 0);
		display_item(75, 55, "START", (selecting == 3) ? 1 : 0);
		break;
	case EDIT_RPM:
		if (blink)
			display_item(5, 15, buf, 1);
		display_item(5, 30, (dir == 1) ? "DIR:  CW" : "DIR:  CCW", 0);
		display_item(5, 55, "BACK", 0);
		display_item(75, 55, "START", 0);
		break;
	case EDIT_DIR:
		display_item(5, 15, buf, 0);
		if (blink)
			display_item(5, 30, (dir == 1) ? "DIR:  CW" : "DIR:  CCW", 1);
		display_item(5, 55, "BACK", 0);
		display_item(75, 55, "START", 0);
		break;
	case RUNNING:
		display_item(5, 15, buf, 0);
		display_item(5, 30, (dir == 1) ? "DIR:  CW" : "DIR:  CCW", 0);
		if (blink)
			display_item(45, 55, "STOP", 1);
		break;
	}
	display_send();
}

uint8_t status_update(uint8_t click) {
	uint8_t blink = (HAL_GetTick() / 500) % 2;
	int32_t count = encoder_get_counter();
	int32_t selecting = count % 4;
	if (selecting < 0)
		selecting += 4;
	switch (state) {
	case SELECTING:
		if (click) {
			if (selecting == 0) {
				state = EDIT_RPM;
				encoder_set_counter(rpm);
			}
			if (selecting == 1) {
				state = EDIT_DIR;
				encoder_set_counter(dir);
			}
			if (selecting == 2)
				return 1; //back
			if (selecting == 3) {
				state = RUNNING;
				motor_set_dir(dir);
				motor_set_rpm(rpm);
				motor_start();
			}
		}

		break;
	case EDIT_RPM:
		if (count < 0) {
			count = 0;
			encoder_set_counter(0);
		}
		if (count > 300) {
			count = 300;
			encoder_set_counter(300);
		}
		rpm = count;
		if (click) {
			state = SELECTING;
			encoder_set_counter(0);
		}
		break;
	case EDIT_DIR:
		if (count < 0) {
			count = 0;
			encoder_set_counter(0);
		}
		if (count > 1) {
			count = 1;
			encoder_set_counter(1);
		}
		dir = count;
		if (click) {
			state = SELECTING;
			encoder_set_counter(1);
		}
		break;
	case RUNNING:
		if (click) {
			motor_stop();
			state = SELECTING;
			encoder_set_counter(3);
		}
		break;
	}
	if (state != laststate || blink != lastblink || rpm != lastrpm
			|| dir != lastdir || selecting != lastselecting) {
		draw(selecting, blink);
		laststate = state;
		lastblink = blink;
		lastrpm = rpm;
		lastdir = dir;
		lastselecting= selecting;
	}
	return 0;
}

void status_enter(void) {
	state = SELECTING;
	encoder_set_counter(0);
	laststate= -1;
}
