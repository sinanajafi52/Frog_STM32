/*
 * menu.c
 *
 *  Created on: Nov 14, 2026
 *      Author: Admin
 */
#include "menu.h"
#include "encoder.h"
#include "display.h"
#include <stdint.h>
#include "main.h"
#include "status.h"
#include "dispenser.h"

typedef enum {
	SCR_MAIN, SCR_STATUS, SCR_DISPENSER, SCR_INFO
} screen_t;
static screen_t screen = SCR_MAIN;
static int32_t lastcount = -1;
static uint8_t redraw = 1;
static uint8_t wrap(int32_t count, uint8_t n) {
	int32_t p = count % n;
	if (p < 0)
		p = p + n;
	return p;
}

void menu_init(void) {
	display_init();
	encoder_set_counter(0);
}

void menu_update(void) {
	int32_t count = encoder_get_counter();
	uint8_t click = encoder_button_press();

	if (count != lastcount) {
		lastcount = count;
		redraw = 1;
	}
	switch (screen) {
	case SCR_MAIN: {
		uint8_t page = wrap(count, 3);
		if (click) {
			if (page == 0) {
				screen = SCR_STATUS;
				status_enter();
			}
			if (page == 1) {
				screen = SCR_DISPENSER;
				dispensr_enter();
			}
			if (page == 2)
				screen = SCR_INFO;
			encoder_set_counter(0);
			redraw = 1;
			break;
		}
		if (redraw) {
			display_clear();
			display_item(5, 15, "STATUS", page == 0);
			display_item(5, 35, "DISPENSER", page == 1);
			display_item(5, 55, "INFO", page == 2);
			display_send();
			redraw = 0;
		}
		break;
	}
	case SCR_STATUS: {
		if (status_update(click)) {
			screen = SCR_MAIN;
			encoder_set_counter(0);
			redraw = 1;
		}

		break;
	}
	case SCR_DISPENSER: {
		if (dispenser_update(click)) {
			screen = SCR_MAIN;
			encoder_set_counter(0);
			redraw = 1;
		}
		break;
	}
	case SCR_INFO: {
		uint8_t page = wrap(count, 2);
		if (click) {
			if (page == 0)
				screen = SCR_MAIN;
			if (page == 1)
				NVIC_SystemReset();
			encoder_set_counter(0);
			redraw = 1;
			break;
		}
		if (redraw) {
			display_clear();
			display_item(35, 15, "VERSION 1.0", 1);
			display_item(5, 55, "BACK", page == 0);
			display_item(85, 55, "RESET", page == 1);
			display_send();
			redraw = 0;
		}
		break;
	}
	}
}
