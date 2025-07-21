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
#include "mcu_util.h"

volatile uint32_t g_core_num_core_0 = 0xFF;
static uint32_t s_core_fifo_data = 0;

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
#if defined(MCU_BOARD_PICO2W)
        cyw43_led_tgl();
        sleep_ms(1000);
#else
        s_core_fifo_data = get_multicore_fifo();
        if (s_core_fifo_data == 0x97654321) {
            printf("[Core 0] RX FIFO Data from Core 1 :  0x%08X\n", s_core_fifo_data);
        }

        NOP();NOP();NOP();
#endif
        WDT_RST;
    }
}