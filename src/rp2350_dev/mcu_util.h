/**
 * @file mcu_util.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief マイコン依存のユーティリティヘッダ
 * @version 0.1
 * @date 2025-06-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef MCU_UTIL_H
#define MCU_UTIL_H

#include "common.h"
#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "pico/rand.h"

void trang_gen_rand_num_u32(uint32_t *p_rand_buf, uint32_t gen_num_cnt);

#endif // MCU_UTIL_H