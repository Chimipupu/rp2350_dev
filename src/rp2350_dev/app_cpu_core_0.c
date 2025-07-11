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
static neopixel_t s_neopixel;

/**
 * @brief CPU Core0のアプリメイン関数
 * 
 */
void app_core_0_main(void)
{
    uint8_t cnt;
    uint32_t core_num = get_core_num();

    s_neopixel.led_cnt = 64;
    s_neopixel.data_pin = 15;
    drv_neopixel_init(&s_neopixel);
    drv_neopixel_set_pixel_rgb(&s_neopixel, 0, 128, 0, 0);

    while(1)
    {
#if defined(MCU_BOARD_PICO2W)
    cyw43_led_tgl();
    sleep_ms(1000);
#endif
        for(cnt = 0; cnt <= s_neopixel.led_cnt; cnt++)
        {
            drv_neopixel_set_pixel_rgb(&s_neopixel, cnt, 128, 0, 0);
        }
        WDT_RST;
    }
}