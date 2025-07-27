/**
 * @file rp2xxx.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief RP2350の固有機能関連
 * @version 0.1
 * @date 2025-7-27
 * 
 * @copyright Copyright (c) 2025 Chimipupu
 * 
 */

#ifndef RP2XXX_H
#define RP2XXX_H

#define MCU_RP2350
#include "rp2350_reg.h"

uint8_t rp2xxx_read_reg_chip_rev(void);
uint8_t rp2xxx_get_cpu_core_num(void);
void pico_sdk_version_print(void);
void rp2xxx_chip_package_print(void);
void rp2xxx_chip_rev_print(void);
void rp2xxx_reg_info(void);

#endif /* RP2XXX_H */