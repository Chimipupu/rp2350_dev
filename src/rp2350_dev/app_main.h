/**
 * @file app_main.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief アプリメインのヘッダ
 * @version 0.1
 * @date 2025-07-19
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#ifndef APP_MAIN_H
#define APP_MAIN_H

#include "rp2350_util.h"
#include "mcu_board_def.h"

#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "pico/version.h"
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"

#define FW_VERSION_MAJOR        0
#define FW_VERSION_MINOR        1
#define FW_VERSION_REVISION     0

void show_mem_dump(uint32_t dump_addr, uint32_t dump_size);
void core_0_main(void);
void core_1_main(void);
void i2c_slave_scan(uint8_t i2c_port);
void proc_exec_time(void (*p_func)(void), const char* p_func_name, ...);
#endif // APP_MAIN_H