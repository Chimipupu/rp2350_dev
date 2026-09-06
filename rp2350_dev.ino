/**
 * @file rp2350_dev.ino
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief RP2350 Arduinoメイン
 * @version 0.1
 * @date 2026-09-06
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */
#include "app_main.h"
#include "multi_core_cpu.h"

// ---------------------------------------------------
// ********** [CPU Core 0] ***********
void setup()
{
    cpu_core_0_init();
}

void loop()
{
    cpu_core_0_main();
}

// ---------------------------------------------------
// ********** [CPU Core 1] ***********
void setup1()
{
    cpu_core_1_init();
}

void loop1()
{
    cpu_core_1_main();
}
// ---------------------------------------------------