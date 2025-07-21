/**
 * @file mcu_util.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief マイコン依存(RP2350)のユーティリティヘッダ
 * @version 0.1
 * @date 2025-06-20
 * 
 * @copyright Copyright (c) 2025 Chimipupu(https://github.com/Chimipupu)
 * 
 */
#ifndef MCU_UTIL_H
#define MCU_UTIL_H

#include "mcu_util.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "hardware/sha256.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/interp.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"

// レジスタを8/16/32bitでR/Wするマクロ
#define REG_READ_BYTE(base, offset)         (*(volatile uint8_t  *)((base) + (offset)))
#define REG_READ_WORD(base, offset)         (*(volatile uint16_t *)((base) + (offset)))
#define REG_READ_DWORD(base, offset)        (*(volatile uint32_t *)((base) + (offset)))
#define REG_WRITE_BYTE(base, offset, val)   (*(volatile uint8_t  *)((base) + (offset)) = (val))
#define REG_WRITE_WORD(base, offset, val)   (*(volatile uint16_t *)((base) + (offset)) = (val))
#define REG_WRITE_DWORD(base, offset, val)  (*(volatile uint32_t *)((base) + (offset)) = (val))

// レジスタビット操作
#define REG_BIT_SET(reg, bit)               ((reg) |=  (1UL << (bit))) // レジスタのビットをセット
#define REG_BIT_CLR(reg, bit)               ((reg) &= ~(1UL << (bit))) // レジスタのビットをクリア
#define REG_BIT_TGL(reg, bit)               ((reg) ^=  (1UL << (bit))) // レジスタのビットをトグル
#define REG_BIT_CHK(reg, bit)               ((reg) &   (1UL << (bit))) // レジスタのビットチェック

// NOP
__attribute__( ( always_inline ) ) static inline void NOP(void)
{
    __asm__ __volatile__("nop");
}

 // 割り込み禁止
__attribute__( ( always_inline ) ) static inline void _DI(void)
{
    __asm__ __volatile__("cpsid i");
}

// 割り込み許可
__attribute__( ( always_inline ) ) static inline void _EI(void)
{
    __asm__ __volatile__("cpsie i");
}

// WDTをなでるマクロ
static inline void WDT_RST(void)
{
#ifdef _WDT_ENABLE_
    watchdog_update();
#else
    NOP();
#endif // _WDT_ENABLE_
}

void trang_gen_rand_num_u32(uint32_t *p_rand_buf, uint32_t gen_num_cnt);
void sha256_padding(const uint8_t *p_src_buf, size_t len, uint8_t *p_dst_buf, size_t *p_out_len);
void hardware_calc_sha256(const uint8_t *p_data_buf, size_t len, uint8_t *p_hash_buf);

#endif // MCU_UTIL_H