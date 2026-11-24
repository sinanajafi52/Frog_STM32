/*
 * chip_temp.h
 *
 *  Created on: Nov 24, 2026
 *      Author: Admin
 */

#ifndef INC_CHIP_TEMP_H_
#define INC_CHIP_TEMP_H_

void chip_temp_init(void);
void chip_temp_update(void);
int32_t chip_temp_get_t10(void);
uint32_t chip_temp_get_vdda(void);


#endif /* INC_CHIP_TEMP_H_ */
