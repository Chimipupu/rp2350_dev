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

// NeoPixelの色定義
typedef enum {
    NEOPIXEL_COLOR_BLACK = 0x00,   // 消灯
    NEOPIXEL_COLOR_RED,     // 赤
    NEOPIXEL_COLOR_GREEN,   // 緑
    NEOPIXEL_COLOR_BLUE,    // 青
    NEOPIXEL_COLOR_YELLOW,  // 黄
    NEOPIXEL_COLOR_CYAN,    // シアン
    NEOPIXEL_COLOR_MAGENTA, // マゼンタ
    NEOPIXEL_COLOR_ORANGE,  // オレンジ
    NEOPIXEL_COLOR_PURPLE,  // 紫
    NEOPIXEL_COLOR_PINK,    // ピンク
    NEOPIXEL_COLOR_WHITE    // 白
} e_neopixel_color;

#define RGB_TO_GRB(r, g, b) ((uint32_t)((g << 16) | (r << 8) | b))
#define GRB_TO_RGB(grb) ((uint32_t)(((grb & 0xFF0000) >> 8) | ((grb & 0xFF00) << 8) | (grb & 0xFF)))

typedef struct {
    union {
        uint32_t u32_grb;
        struct {
            uint32_t blue       : 8;
            uint32_t red        : 8;
            uint32_t green      : 8;
            uint32_t reserved   : 8;
        } grb_bit;
    } grb_color;
} rgb_color_t;

typedef struct {
    uint8_t data_pin;
    uint8_t led_cnt;
    rgb_color_t *p_pixel_grb_buf;
} neopixel_t;

void drv_neopixel_init(neopixel_t *p_neopixel);
void drv_neopixel_set_pixel_rgb(neopixel_t *p_neopixel, uint8_t led, uint8_t red, uint8_t green, uint8_t blue);
void drv_neopixel_set_pixel_color(neopixel_t *p_neopixel, uint8_t led, uint8_t color);
void drv_neopixel_clear(neopixel_t *p_neopixel);
void drv_neopixel_set_all_led_color(neopixel_t *p_neopixel, uint8_t red, uint8_t green, uint8_t blue);
uint32_t drv_neopixel_get_pixel_color(neopixel_t *p_neopixel, uint8_t led);
void drv_neopixel_pixel_color_fade(uint8_t *r, uint8_t *g, uint8_t *b);

#endif // DRV_NEOPIXEL_H