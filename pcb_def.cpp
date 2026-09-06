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
// ---------------------------------------------------