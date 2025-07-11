/**
 * @file drv_neopixel.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief NeoPixelドライバ
 * @version 0.1
 * @date 2025-07-12
 * 
 * @copyright Copyright (c) 2025 Chimipupu(https://github.com/Chimipupu)
 * 
 */
#include "drv_neopixel.h"

static void pio_neopixel_begin(PIO pio, uint sm, uint offset,  uint pin);
static void set_pio_neopixel_show(neopixel_t *p_neopixel);

static PIO s_pio = pio1;
static uint s_sm = 0;
static uint s_offset = 0;

// PIO側に初期化を叩く
static void pio_neopixel_begin(PIO pio, uint sm, uint offset,  uint pin)
{
    pio_neopixel_init(pio, sm, offset, pin, 800000, false);
    pio_sm_set_enabled(pio, sm, true);
}

void set_pio_neopixel_show(neopixel_t *p_neopixel)
{
    for (uint8_t i = 0; i < p_neopixel->led_cnt; i++)
    {
        uint8_t red_ptr = p_neopixel->p_pixels[i * 3];
        uint8_t green_ptr = p_neopixel->p_pixels[(i * 3) + 1];
        uint8_t blue_ptr = p_neopixel->p_pixels[(i * 3) + 2];

        uint32_t color_data = ((uint32_t)(red_ptr) << 8) |
                            ((uint32_t)(green_ptr) << 16) |
                            (uint32_t)(blue_ptr);

        pio_sm_put_blocking(s_pio, s_sm, color_data << 8u);
    }
}

void drv_neopixel_init(neopixel_t *p_neopixel)
{
    s_offset = pio_add_program(s_pio, &neopixel_program);
    pio_neopixel_begin(s_pio, 0, s_offset, p_neopixel->data_pin);
}

void drv_neopixel_set_pixel_rgb(neopixel_t *p_neopixel, uint8_t led, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t *p = &p_neopixel->p_pixels[led * 3];
    *p++ = red;
    *p++ = green;
    *p++ = blue;
}

void drv_neopixel_set_pixel_color(neopixel_t *p_neopixel, uint8_t led, uint32_t color)
{
    // TODO:色指定の状態遷移

    // uint8_t *p = &p_neopixel->p_pixels[led * 3];
    // *p++ = (color >> 16) & 0xFF; // Red
    // *p++ = (color >> 8) & 0xFF;  // Green
    // *p++ = color & 0xFF;         // Blue
}

void drv_neopixel_clear(neopixel_t *p_neopixel)
{
    // TODO:Neopixelの消灯
}

void drv_neopixel_set_all_led_color(neopixel_t *p_neopixel, uint8_t red, uint8_t green, uint8_t blue)
{
    for (uint8_t i = 0; i < p_neopixel->led_cnt; i++)
    {
        uint8_t *p = &p_neopixel->p_pixels[i * 3];
        *p++ = red;
        *p++ = green;
        *p++ = blue;
    }
}

uint32_t drv_neopixel_get_pixel_color(neopixel_t *p_neopixel, uint8_t led)
{
    uint8_t ofs = led * 3;
    return ((uint32_t)p_neopixel->p_pixels[ofs] << 16) |
            ((uint8_t)p_neopixel->p_pixels[ofs + 1] << 8) |
            ((uint8_t)p_neopixel->p_pixels[ofs + 2]);
}