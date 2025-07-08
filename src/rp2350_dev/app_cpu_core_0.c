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

/**
 * @brief CPU Core0のアプリメイン関数
 * 
 */
void app_core_0_main(void)
{
    uint32_t core_num = get_core_num();

    while(1)
    {
#if 0
        printf("CPU Core: %d\n", core_num);
        sleep_ms(1000);
#else
        NOP();NOP();NOP();
#endif

#if defined(MCU_BOARD_PICO2W)
    cyw43_led_tgl();
    sleep_ms(1000);
#endif

        WDT_RST;
    }
}