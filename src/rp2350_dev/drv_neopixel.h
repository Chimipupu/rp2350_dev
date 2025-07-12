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

#define RGB_TO_GRB(r, g, b) ((uint32_t)((g << 16) | (r << 8) | b))
#define GRB_TO_RGB(grb) ((uint32_t)(((grb & 0xFF0000) >> 8) | ((grb & 0xFF00) << 8) | (grb & 0xFF)))

typedef struct {
    union {
        uint32_t u32_rgb;
        struct {
            uint32_t reserved   : 8;
            uint32_t blue       : 8;
            uint32_t red        : 8;
            uint32_t green      : 8;
        } rgb;
    } rgb_color;
} rgb_color_t;

typedef struct {
    uint8_t data_pin;
    uint8_t led_cnt;
    rgb_color_t *p_pixel_rgb_buf;
} neopixel_t;

void drv_neopixel_init(neopixel_t *p_neopixel);
void drv_neopixel_set_pixel_rgb(neopixel_t *p_neopixel, uint8_t led, uint8_t red, uint8_t green, uint8_t blue);
// void drv_neopixel_set_pixel_color(neopixel_t *p_neopixel, uint8_t led, uint32_t color);
void drv_neopixel_clear(neopixel_t *p_neopixel);
void drv_neopixel_set_all_led_color(neopixel_t *p_neopixel, uint8_t red, uint8_t green, uint8_t blue);
uint32_t drv_neopixel_get_pixel_color(neopixel_t *p_neopixel, uint8_t led);

#endif // DRV_NEOPIXEL_H