/**
 * @file app_cpu_core_1.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief RP2350のCPU Core1のアプリ
 * @version 0.1
 * @date 2025-06-13
 * 
 * @copyright Copyright (c) 2025 Chimipupu(https://github.com/Chimipupu)
 * 
 */
#include "app_cpu_core_1.h"
#include "app_main.h"
#include "dbg_com.h"

#include "drv_neopixel.h"
neopixel_t s_neopixel;
static rgb_color_t s_rgb_buf[NEOPIXEL_LED_CNT] = {0};

/**
 * @brief CPU Core1のアプリメイン関数
 * 
 */
void app_core_1_main(void)
{
    // uint32_t core_num = get_core_num();

    sleep_ms(300); // Core0の初期化待ち

    // NeoPixel初期化(PIOで並列処理)
    s_neopixel.led_cnt = NEOPIXEL_LED_CNT;
    s_neopixel.data_pin = MCU_BOARD_NEOPIXEL_PIN;
    memset(s_rgb_buf, 0, sizeof(s_rgb_buf));
    s_neopixel.p_pixel_grb_buf = &s_rgb_buf[0];
    drv_neopixel_init(&s_neopixel);

    pico_sdk_version_print();

#if defined(PICO_RP2040) && !defined(PICO_RP2350)
    printf("MCU:\tRP2040\n");
#elif !defined(PICO_RP2040) && defined(PICO_RP2350)
    printf("MCU:\tRP2350\n");
#endif

    printf("System Clock:\t%d MHz\n", clock_get_hz(clk_sys) / 1000000);
    printf("USB Clock:\t%d MHz\n", clock_get_hz(clk_usb) / 1000000);

    // デバッグモニタ初期化
    dbg_com_init();

    while(1)
    {
        dbg_com_main();
        WDT_RST();
    }
}