/*
 * display.c
 *
 *  Created on: Nov 10, 2026
 *      Author: Admin
 */

#include "display.h"
#include "u8g2.h"
#include "u8g2_stm32_hal.h"

static u8g2_t u8g2;

void display_init(void) //starter in begin 2
{
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0,
				u8x8_byte_stm32_hw_i2c, u8x8_gpio_and_delay_stm32);

		u8g2_InitDisplay(&u8g2);
		u8g2_SetPowerSave(&u8g2, 0);
}
void display_clear(void)
{
	u8g2_ClearBuffer(&u8g2);
}
void display_send(void)
{
	u8g2_SendBuffer(&u8g2);
}
void display_item(uint8_t x,uint8_t y,const char *text, uint8_t selected)
{
	u8g2_SetFont(&u8g2, selected ? u8g2_font_ncenB08_tr:u8g2_font_ncenR08_tr);
	u8g2_DrawStr(&u8g2, x, y, text);
}
