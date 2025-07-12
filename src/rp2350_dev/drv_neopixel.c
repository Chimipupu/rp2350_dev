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

static void pio_neopixel_begin(neopixel_t *p_neopixel, PIO pio, uint sm, uint offset,  uint pin);
static void set_pio_neopixel_show(neopixel_t *p_neopixel);

static PIO s_pio = pio1;
static uint s_sm = 0;
static uint s_offset = 0;

// PIO側に初期化を叩く
static void pio_neopixel_begin(neopixel_t *p_neopixel, PIO pio, uint sm, uint offset,  uint pin)
{
    pio_neopixel_init(pio, sm, offset, pin, 800000, false);
    drv_neopixel_clear(p_neopixel);
}

static void set_pio_neopixel_show(neopixel_t *p_neopixel)
{
    pio_sm_put_blocking(s_pio, s_sm, p_neopixel->p_pixel_rgb_buf->rgb_color.u32_rgb << 8u);
    // pio_sm_put_blocking(s_pio, s_sm, p_neopixel->p_pixel_rgb_buf->rgb_color.u32_rgb);
}

void drv_neopixel_init(neopixel_t *p_neopixel)
{
    bool ret = false;

    s_offset = pio_add_program(s_pio, &neopixel_program);
    ret = pio_claim_free_sm_and_add_program_for_gpio_range(&neopixel_program, &s_pio, &s_sm, &s_offset, p_neopixel->data_pin, 1, true);
    hard_assert(ret);
    pio_neopixel_begin(p_neopixel, s_pio, s_sm, s_offset, p_neopixel->data_pin);
}

void drv_neopixel_set_pixel_rgb(neopixel_t *p_neopixel, uint8_t led, uint8_t red, uint8_t green, uint8_t blue)
{
#if 1
    p_neopixel->p_pixel_rgb_buf[led].rgb_color.u32_rgb = ((uint32_t) (red) << 8) |
                                                        ((uint32_t) (green) << 16) |
                                                        (uint32_t) (blue);
#else
    p_neopixel->p_pixel_rgb_buf[led].rgb_color.rgb.red = red;
    p_neopixel->p_pixel_rgb_buf[led].rgb_color.rgb.green = green;
    p_neopixel->p_pixel_rgb_buf[led].rgb_color.rgb.blue = blue;
#endif
    set_pio_neopixel_show(p_neopixel);
}

void drv_neopixel_set_pixel_color(neopixel_t *p_neopixel, uint8_t led, uint32_t color)
{
    // TODO:色指定の状態遷移
}

// 全GRBカラーバッファを初期化
void drv_neopixel_clear(neopixel_t *p_neopixel)
{
    for (uint8_t i = 0; i < p_neopixel->led_cnt; i++)
    {
        p_neopixel->p_pixel_rgb_buf[i].rgb_color.rgb.red = 0;
        p_neopixel->p_pixel_rgb_buf[i].rgb_color.rgb.green = 0;
        p_neopixel->p_pixel_rgb_buf[i].rgb_color.rgb.blue = 0;
        set_pio_neopixel_show(p_neopixel);
        p_neopixel->p_pixel_rgb_buf++;
    }
}

void drv_neopixel_set_all_led_color(neopixel_t *p_neopixel, uint8_t red, uint8_t green, uint8_t blue)
{
    for (uint8_t i = 0; i < p_neopixel->led_cnt; i++)
    {
        p_neopixel->p_pixel_rgb_buf[i].rgb_color.rgb.red = red;
        p_neopixel->p_pixel_rgb_buf[i].rgb_color.rgb.green = green;
        p_neopixel->p_pixel_rgb_buf[i].rgb_color.rgb.blue = blue;
        set_pio_neopixel_show(p_neopixel);
        p_neopixel->p_pixel_rgb_buf++;
    }
}

uint32_t drv_neopixel_get_pixel_color(neopixel_t *p_neopixel, uint8_t led)
{
    uint32_t color = 0;
    // color = p_neopixel->p_pixel_rgb_buf->rgb_color.u32_grb;
    color = p_neopixel->p_pixel_rgb_buf->rgb_color.u32_rgb;
    return color;
}