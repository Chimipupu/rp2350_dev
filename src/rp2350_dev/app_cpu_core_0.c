/**
 * @file app_cpu_core_0.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief RP2350のCPU Core0のアプリ
 * @version 0.1
 * @date 2025-06-13
 * 
 * @copyright Copyright (c) 2025 Chimipupu(https://github.com/Chimipupu)
 * 
 */
#include "app_cpu_core_0.h"
#include "mcu_board_def.h"

#include "drv_neopixel.h"
neopixel_t s_neopixel;
static rgb_color_t s_rgb_buf[NEOPIXEL_LED_CNT] = {0};

/**
 * @brief CPU Core0のアプリメイン関数
 * 
 */
void app_core_0_main(void)
{
    uint32_t core_num = get_core_num();

#if 1
    s_neopixel.led_cnt = NEOPIXEL_LED_CNT;
    s_neopixel.data_pin = MCU_BOARD_NEOPIXEL_PIN;
    memset(s_rgb_buf, 0, sizeof(s_rgb_buf));
    s_neopixel.p_pixel_grb_buf = &s_rgb_buf[0];
    drv_neopixel_init(&s_neopixel);
#endif // MCU_BOARD_NEOPIXEL

    while(1)
    {
#if defined(MCU_BOARD_PICO2W)
        cyw43_led_tgl();
        sleep_ms(1000);
#else
    #if 0
            uint8_t r, g, b;
            for (uint8_t i = 0; i < s_neopixel.led_cnt; i++)
            {
                drv_neopixel_pixel_color_fade(&r, &g, &b);
                drv_neopixel_set_pixel_rgb(&s_neopixel, i, r, g, b);
                WDT_RST;
            }
            sleep_ms(50);
    #else
            NOP();NOP();NOP();
    #endif
#endif
        WDT_RST;
    }
}