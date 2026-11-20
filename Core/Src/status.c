/*
 * status.c
 *
 *  Created on: Nov 17, 2026
 *      Author: Admin
 */
#include "status.h"
#include "encoder.h"
#include <stdint.h>
#include "motor.h"

typedef enum {
	SELECTING, EDIT_RPM, EDIT_DIR, RUNNING
} Statusstate;
static Statusstate state = SELECTING;
static uint16_t rpm;
static uint8_t dir;

uint8_t status_update(uint8_t click) {
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
				return 1;
			if (selecting == 3) {
				state = RUNNING;
				motor_start();
				motor_set_dir(dir);
				motor_set_rpm(rpm);

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

		break;
	}
	return 0;
}

void status_enter(void) {
	state = SELECTING;
	encoder_set_counter(0);
}
