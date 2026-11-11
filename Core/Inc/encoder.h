/*
 * encoder.h
 *
 *  Created on: Nov 10, 2026
 *      Author: Admin
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_
#include <stdint.h>

void encoder_set_counter(int32_t value);
uint8_t encoder_button_press(void);
int32_t encoder_get_counter(void);

#endif /* INC_ENCODER_H_ */
