/**
 * @file app_cpu_core_0.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief RP2350のCPU Core0のアプリ
 * @version 0.1
 * @date 2025-06-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "app_cpu_core_0.h"


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
        WDT_RST;
    }
}