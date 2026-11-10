/*
 * display.h
 *
 *  Created on: Nov 10, 2026
 *      Author: Admin
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include <stdint.h>

void display_init(void);
void display_clear(void);
void display_item(uint8_t x,uint8_t y,const char *text, uint8_t selected);
void display_send(void);


#endif /* INC_DISPLAY_H_ */
