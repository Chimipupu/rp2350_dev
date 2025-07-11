/**
 * @file drv_neopixel.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief NeoPixelドライバヘッダ
 * @version 0.1
 * @date 2025-07-12
 * 
 * @copyright Copyright (c) 2025 Chimipupu(https://github.com/Chimipupu)
 * 
 */
#ifndef DRV_NEOPIXEL_H
#define DRV_NEOPIXEL_H

// #pragma once

#include "hardware/pio.h"
#include "pico/stdlib.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "neopixel.pio.h"

typedef struct {
    uint8_t data_pin;
    uint8_t led_cnt;
    uint8_t *p_pixels;
} neopixel_t;

void drv_neopixel_init(neopixel_t *p_neopixel);
void drv_neopixel_set_pixel_rgb(neopixel_t *p_neopixel, uint8_t led, uint8_t red, uint8_t green, uint8_t blue);
void drv_neopixel_set_pixel_color(neopixel_t *p_neopixel, uint8_t led, uint32_t color);
void drv_neopixel_clear(neopixel_t *p_neopixel);
void drv_neopixel_set_all_led_color(neopixel_t *p_neopixel, uint8_t red, uint8_t green, uint8_t blue);
uint32_t drv_neopixel_get_pixel_color(neopixel_t *p_neopixel, uint8_t led);

#endif // DRV_NEOPIXEL_H