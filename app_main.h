/**
 * @file app_main.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief アプリメインヘッダー
 * @version 0.1
 * @date 2026-09-06
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */
#ifndef APP_MAIN_HPP
#define APP_MAIN_HPP

#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "pico/version.h"
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"

void app_main_core_0_init(void);
void app_main_core_1_init(void);
void app_main_core_0(void);
void app_main_core_1(void);

#endif // APP_MAIN_HPP