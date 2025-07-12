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

#ifdef MCU_BOARD_NEOPIXEL
#include "drv_neopixel.h"
static neopixel_t s_neopixel;
#endif // MCU_BOARD_NEOPIXEL

/**
 * @brief CPU Core0のアプリメイン関数
 * 
 */
void app_core_0_main(void)
{
    uint32_t core_num = get_core_num();

#ifdef MCU_BOARD_NEOPIXEL
    s_neopixel.led_cnt = NEOPIXEL_LED_CNT;
    s_neopixel.data_pin = MCU_BOARD_NEOPIXEL_PIN;
    drv_neopixel_init(&s_neopixel);
    drv_neopixel_set_pixel_rgb(&s_neopixel, 0, 32, 32, 32);
#endif // MCU_BOARD_NEOPIXEL

    while(1)
    {
#if defined(MCU_BOARD_PICO2W)
        cyw43_led_tgl();
        sleep_ms(1000);
#endif

#if 0
        drv_neopixel_set_pixel_rgb(&s_neopixel, 0, 32, 32, 32);
#else
        NOP();NOP();NOP();
#endif // MCU_BOARD_NEOPIXEL
        WDT_RST;
    }
}