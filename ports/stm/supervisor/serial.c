/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017, 2018 Scott Shawcroft for Adafruit Industries
 * Copyright (c) 2019 Lucian Copeland for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/mphal.h"
#include <string.h>
#include "supervisor/shared/serial.h"
#if CPY_STM32F4
#include "stm32f4xx_hal.h"
#include "stm32f4/gpio.h"
// TODO: Switch this to using DEBUG_UART.

UART_HandleTypeDef huart2;
#endif

void port_serial_init(void) {
    #if CPY_STM32F4
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) == HAL_OK) {
        stm32f4_peripherals_status_led(1, 1);
    }
    #endif
}

bool port_serial_connected(void) {
    return true;
}

char port_serial_read(void) {
    #if CPY_STM32F4
    uint8_t data;
    HAL_UART_Receive(&huart2, &data, 1, 500);
    return data;
    #else
    return -1;
    #endif
}

// There is no easy way to find the number of pending characters, so just say there's 1.
uint32_t port_serial_bytes_available(void) {
    #if CPY_STM32F4
    return __HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) ? 1 : 0;
    #else
    return 0;
    #endif
}

void port_serial_write_substring(const char *text, uint32_t len) {
    #if CPY_STM32F4
    HAL_UART_Transmit(&huart2, (uint8_t *)text, len, 5000);
    #endif
}
