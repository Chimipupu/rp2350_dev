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
#include <string.h>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "hardware/sha256.h"

void trang_gen_rand_num_u32(uint32_t *p_rand_buf, uint32_t gen_num_cnt);
void sha256_padding(const uint8_t *p_src_buf, size_t len, uint8_t *p_dst_buf, size_t *p_out_len);
void hardware_calc_sha256(const uint8_t *p_data_buf, size_t len, uint8_t *p_hash_buf);

#endif // MCU_UTIL_H