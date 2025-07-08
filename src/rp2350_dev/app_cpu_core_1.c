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

/**
 * @brief CPU Core1のアプリメイン関数
 * 
 */
void app_core_1_main(void)
{
    pico_sdk_version_print();

#if defined(PICO_RP2040) && !defined(PICO_RP2350)
    printf("MCU:\tRP2040\n");
#elif !defined(PICO_RP2040) && defined(PICO_RP2350)
    printf("MCU:\tRP2350\n");
#endif

    printf("System Clock:\t%d MHz\n", clock_get_hz(clk_sys) / 1000000);
    printf("USB Clock:\t%d MHz\n", clock_get_hz(clk_usb) / 1000000);

    dbg_com_init();

    // uint32_t core_num = get_core_num();

    while(1)
    {
        dbg_com_process();
#if 0
        printf("CPU Core: %d\n", core_num);
        sleep_ms(2000);
#endif
        WDT_RST();
    }
}