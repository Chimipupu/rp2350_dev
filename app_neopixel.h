/**
 * @file app_neopixel.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief NeoPixelアプリ
 * @version 0.1
 * @date 2026-09-06
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */
#ifndef APP_NEOPIXEL_H
#define APP_NEOPIXEL_H

#include "pcb_def.h"

#if defined(RGBLED_PIN)
#include "stdint.h"
#include <Adafruit_NeoPixel.h>

// ---------------------------------------------------
// [コンパイルスイッチ]
#define DEBUG_RGB_LED

// ------------------------------------------------------------------------------
typedef union {
    uint32_t rgb;
    struct {
        uint32_t blue : 8;
        uint32_t green : 8;
        uint32_t red : 8;
        uint32_t reserved : 8;
    } para;
} led_color_t;

typedef struct {
    led_color_t rgb;
    const char *p_color_str;
} led_color_data_t;

extern const led_color_data_t g_led_color_tbl[];
extern const uint8_t RGBLED_COLOR_TBL_SIZE;
// ------------------------------------------------------------------------------
void app_neopixel_init(uint8_t led_pin, uint8_t led_num, uint8_t brightness);
void app_neopixel_set_rgb(uint8_t led_no, led_color_t *p_rgb);
void app_neopixel_rgb_illumination(uint8_t led_no);
void app_neopixel_set_brightness(uint8_t brightness);
// ---------------------------------------------------
#endif

#endif // APP_NEOPIXEL_H