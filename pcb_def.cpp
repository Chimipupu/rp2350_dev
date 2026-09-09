/**
 * @file pcb_def.cpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief RP2350基板固有処理
 * @version 0.1
 * @date 2026-09-06
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */
#include "pcb_def.h"
#include <Arduino.h>
#include <Wire.h>

#include "pico/platform.h"
#include "hardware/clocks.h"
#include "hardware/structs/sysinfo.h"

// ---------------------------------------------------
void pcb_gpio_init(void)
{
#ifdef PCB_RPI_PICO_2
    pinMode(OB_LED_PIN, OUTPUT);
    digitalWrite(OB_LED_PIN, LOW);
#endif
}

void pcb_uart_init(void)
{
    Serial.begin(115200); // UART初期化（115200bps 8N1）
}

void pcb_i2c_init(void)
{
    Wire.setSDA(I2C_SDA_PIN);
    Wire.setSCL(I2C_SCL_PIN);
    Wire.begin();
}

unsigned int DBG_PRINTF(const char *p_fmt, ...)
{
    char buf[256];
    va_list args;

    va_start(args, p_fmt);
    unsigned int ret = vsnprintf(buf, sizeof(buf), p_fmt, args);
    Serial.print(buf);
    va_end(args);

    return ret;
}

void pcb_info(void)
{
    uint32_t chip_rev;
    uint32_t cpu_freq_Mhz;
    uint32_t core_num;

    DBG_PRINTF("RP2350: ARM Cortex-M33 x2 Core\n");

    cpu_freq_Mhz = clock_get_hz(clk_sys) / 1000000;
    DBG_PRINTF("CPU Clock: %dMHz\n", cpu_freq_Mhz);

    core_num = get_core_num();
    DBG_PRINTF("Running CPU: Core %u\n", core_num);

    // チップリビジョンRP2350: 1=A2, 3=A4、RP204: B0=1, B1=2, B2=3
    chip_rev = (sysinfo_hw->chip_id & SYSINFO_CHIP_ID_REVISION_BITS) >> SYSINFO_CHIP_ID_REVISION_LSB;
    if (chip_rev == 1) {
        DBG_PRINTF("Chip Rev: A2\n");
    } else if (chip_rev == 3) {
        DBG_PRINTF("Chip Rev: A4\n");
    } else {
        DBG_PRINTF("Chip Rev: Unknown\n");
    }
}
// ---------------------------------------------------