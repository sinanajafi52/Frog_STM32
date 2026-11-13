/*
 * motor.h
 *
 *  Created on: Nov 13, 2026
 *      Author: Admin
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include <stdint.h>

void motor_init(void);//starter at begin 2
void motor_start(void);
void motor_stop(void);
void motor_set_dir(uint8_t dir);
void motor_set_rpm(uint16_t rpm);
void motor_update(void);
uint8_t motor_is_running(void);

#endif /* INC_MOTOR_H_ */
