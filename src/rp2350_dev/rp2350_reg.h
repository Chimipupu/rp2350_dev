/**
 * @file rp2350_reg.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief RP2350のレジスタ定義
 * @version 0.1
 * @date 2025-7-27
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#ifndef RP2350_REG_H
#define RP2350_REG_H

#include <stdint.h>

#define M33_REG_BASE                              0xE0000000
#define M33_REG_OFFSET                            0x0000ED00
#define M33_CPUID_REG_ADDR                       (M33_REG_BASE + M33_REG_OFFSET)

#define SIO_REG_BASE                              0xD0000000
#define SIO_REG_OFFSET                            0x00000000
#define SIO_CPUID_REG_ADDR                       (SIO_REG_BASE + SIO_REG_OFFSET)

#define SYSINFO_REG_BASE                          0x40000000
#define SYSINFO_CPUID_REG_OFFSET                  0x00000000
#define SYSINFO_PACKAGE_SEL_REG_OFFSET            0x00000004
#define SYSINFO_CPUID_REG_ADDR                   (SYSINFO_REG_BASE + SYSINFO_CPUID_REG_OFFSET)
#define SYSINFO_PACKAGE_SEL_REG_ADDR             (SYSINFO_REG_BASE + SYSINFO_PACKAGE_SEL_REG_OFFSET)
#define CHIP_PACKAGE_QFN60_RP2350A                1
#define CHIP_PACKAGE_QFN80_RP2350B                0

#define TRNG_REG_BASE                             0x400F0000
#define TRNG_VALID_OFFSET                         0x00000110
#define TRNG_RND_SOURCE_ENABLE_OFFSET             0x0000012C
#define TRNG_VALID_ADDR                          (TRNG_REG_BASE + TRNG_VALID_OFFSET)
#define TRNG_RND_SOURCE_ENABLE_ADDR              (TRNG_REG_BASE + TRNG_RND_SOURCE_ENABLE_OFFSET)

typedef union {
    uint32_t DWORD;
    struct {
        uint32_t REVISION:4;        // bit[3:0]
        uint32_t PARTNO:12;         // bit[15:4]
        uint32_t ARCHITECTURE:4;    // bit[19:16]
        uint32_t VARIANT:4;         // bit[23:20]
        uint32_t IMPLEMENTER:8;     // bit[31:24]
    }BIT;
} M33_CPUID;
typedef union {
    uint32_t DWORD;
} SIO_CPUID;

typedef union {
    uint32_t DWORD;
    struct {
        uint32_t STOP_BIT:1;         // bit[0]
        uint32_t MANUFACTURER:11;    // bit[11:1]
        uint32_t PART:16;            // bit[27:12]
        uint32_t REVISION:4;         // bit[31:28]
    }BIT;
} SYSINFO_CPUID;

typedef union {
    uint32_t DWORD;
    struct {
        uint32_t PACKAGE:1;         // bit[0] 0 = QFN80(RP2350B), 1 = QFN60(RP2350A)
        uint32_t Reserved:31;       // bit[31:1]
    }BIT;
} SYSINFO_PACKAGE_SEL;

typedef union {
    uint32_t DWORD;
    struct {
        uint32_t EHR_VALID:1;        // bit[0] ... 1で乱数生成完了
        uint32_t Reserved:31;        // bit[31:1]
    }BIT;
} TRNG_VALID;

typedef union {
    uint32_t DWORD;
    struct {
        uint32_t RND_SRC_EN:1;       // bit[0] ... 1で乱数生成開始
        uint32_t Reserved:31;        // bit[31:1]
    }BIT;
} TRNG_RND_SOURCE_ENABLE;
#endif /* RP2350_REG_H */