/**
 * @file app_cpu_core_0.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief RP2350のCPU Core0のアプリ
 * @version 0.1
 * @date 2025-06-13
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#include "app_cpu_core_0.h"
#include "pcb_def.h"
#include "rp2350_util.h"
#include "drv_neopixel.h"

volatile uint32_t g_core_num_core_0 = 0xFF;
static uint32_t s_core_fifo_data = 0;
static uint8_t s_fade_pixel_flg = 0;

static void app_multicore_state_machine(uint32_t state);

static void app_multicore_state_machine(uint32_t state)
{
        switch (state)
        {
            case MULTI_CORE_TEST_DATA:
                printf("[Core 0] RX FIFO Data from Core 1 :  0x%08X\n", s_core_fifo_data);
                break;

            case PROC_NEOPIXEL_FADE:
                s_fade_pixel_flg = 1;
                break;

            default:
                NOP();NOP();NOP();
                break;
        }
}

/**
 * @brief CPU Core0のアプリメイン関数
 * 
 */
void app_core_0_main(void)
{
    g_core_num_core_0 = get_core_num();

    // Core 1 起動待ち（ブロッキングでFIFOを待つ）
    s_core_fifo_data = get_multicore_fifo();

    while(1)
    {
#if defined(PCB_PICO2W)
        cyw43_led_tgl();
        sleep_ms(1000);
#else
        s_core_fifo_data = get_multicore_fifo();
        app_multicore_state_machine(s_core_fifo_data);
        if (s_fade_pixel_flg != 0) {
            drv_neopixel_pixel_color_fade();
            sleep_ms(1);
        }
#endif
        WDT_RST;
    }
}