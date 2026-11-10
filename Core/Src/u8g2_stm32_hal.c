#include "u8g2_stm32_hal.h"

uint8_t u8x8_byte_stm32_hw_i2c(u8x8_t *u8x8, uint8_t msg,
                               uint8_t arg_int, void *arg_ptr)
{
    static uint8_t buffer[32];
    static uint8_t buf_idx;
    uint8_t *data;

    switch (msg)
    {
    case U8X8_MSG_BYTE_START_TRANSFER:      // new message: start empty
        buf_idx = 0;
        break;
    case U8X8_MSG_BYTE_SEND:                // collect bytes
        data = (uint8_t *)arg_ptr;
        while (arg_int > 0 && buf_idx < sizeof(buffer))
        {
            buffer[buf_idx++] = *data++;
            arg_int--;
        }
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:        // send them with HAL
        HAL_I2C_Master_Transmit(&hi2c1, u8x8_GetI2CAddress(u8x8),
                                buffer, buf_idx, 100);
        break;
    case U8X8_MSG_BYTE_INIT:
    case U8X8_MSG_BYTE_SET_DC:
        break;
    default:
        return 0;
    }
    return 1;
}

uint8_t u8x8_gpio_and_delay_stm32(u8x8_t *u8x8, uint8_t msg,
                                  uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_DELAY_MILLI:              // u8g2 asks: wait some ms
        HAL_Delay(arg_int);
        break;
    default:
        break;
    }
    return 1;
}
