/**
 * @file app_neopixel.cpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief NeoPixelアプリ
 * @version 0.1
 * @date 2026-09-06
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */

#include "app_neopixel.h"

#include "pcb_def.h"
#if defined(RGBLED_PIN)

// -----------------------------------------------------------
// RGBLEDカラーテーブル
const led_color_data_t g_led_color_tbl[] = {
    { { .rgb = 0xFF0000   }, "RED" },    // 赤   0xFF0000
    { { .rgb = 0xFFFF00   }, "YELLOW" }, // 黄   0xFFFF00
    { { .rgb = 0xFFA500   }, "ORANGE" }, // 橙   0xFFA500
    { { .rgb = 0x00FF00   }, "GREEN" },  // 緑   0x00FF00
    { { .rgb = 0x0000FF   }, "BLUE" },   // 青   0x0000FF
    { { .rgb = 0xFF00FF   }, "PURPLE" }, // 紫   0xFF00FF
    { { .rgb = 0xFFFFFF   }, "WHITE" },  // 白   0xFFFFFF
    { { .rgb = 0x00000000 }, "OFF" }     // 消灯 0x00000000
};
const uint8_t RGBLED_COLOR_TBL_SIZE = sizeof(g_led_color_tbl) / sizeof(g_led_color_tbl[0]);

static Adafruit_NeoPixel *s_p_pixel = NULL;
static uint8_t s_led_num = 0;
static uint8_t s_brightness = 0;
static led_color_t *s_p_rgb_buf = NULL;

static void _set_rgb(uint8_t led_no, led_color_t *rgb, uint8_t brightness);
// -----------------------------------------------------------
// [Static]

static void _set_rgb(uint8_t led_no, led_color_t *p_rgb, uint8_t brightness)
{
if (s_p_pixel == NULL) {
        return;
    }

    s_p_pixel->clear();
    s_p_pixel->setPixelColor(led_no, s_p_pixel->Color(p_rgb->para.red, p_rgb->para.green, p_rgb->para.blue));
    s_p_pixel->setBrightness(brightness);
    s_p_pixel->show();
}
// -----------------------------------------------------------
// [API]

/**
 * @brief RGBLED(Neopixel)の初期化
 * @param led_pin RGBLED(Neopixel)のデータピンとの接続GPIO
 * @param led_num LEDの数
 * @param brightness 最大輝度
 */
void app_neopixel_init(uint8_t led_pin, uint8_t led_num, uint8_t brightness)
{
    s_led_num = led_num;
    s_brightness = brightness;

    if (s_p_pixel != NULL) {
        delete s_p_pixel;
    }

    // NeoPixelオブジェクトを生成
    s_p_pixel = new Adafruit_NeoPixel(s_led_num, led_pin, NEO_GRB + NEO_KHZ800);

    // バッファを確保
#if (RGBLED_NUM > 1)
        static led_color_t led_color_buf[RGBLED_NUM];
        s_p_rgb_buf = &led_color_buf[0];
#else
        static led_color_t led_color_buf;
        s_p_rgb_buf = &led_color_buf;
#endif

    s_p_pixel->begin();
    s_p_pixel->clear();
    s_p_pixel->show();
}

/**
 * @brief 指定のLEDの色をRGBで設定
 * @param led_no LEDの指定
 * @param p_rgb RGB構造体ポインタ
 */
void app_neopixel_set_rgb(uint8_t led_no, led_color_t *p_rgb)
{
    if(p_rgb == NULL) {
        return;
    }

    _set_rgb(led_no, p_rgb, s_brightness);
}

/**
 * @brief 指定のLEDの色をフルカラーで回転させる
 * @param led_no LEDの指定
 */
void app_neopixel_rgb_illumination(uint8_t led_no)
{
    static uint8_t s_idx = 0;

    if(((led_no + 1) > s_led_num) || (s_p_rgb_buf == NULL)) {
        return;
    }

#ifdef DEBUG_RGB_LED
    Serial.printf("[DEBUG] RGB_LED: Color = %s\n", g_led_color_tbl[s_idx].p_color_str);
#endif

    s_p_rgb_buf[led_no] = g_led_color_tbl[s_idx].rgb;
    app_neopixel_set_rgb(led_no, &s_p_rgb_buf[led_no]);
    s_idx = (s_idx + 1) % (RGBLED_COLOR_TBL_SIZE - 1);
}

/**
 * @brief LEDの輝度変更
 * @param brightness 輝度(0~255)
 */
void app_neopixel_set_brightness(uint8_t brightness)
{
    s_brightness = brightness;
}

// ---------------------------------------------------
#endif // RGBLED_PIN