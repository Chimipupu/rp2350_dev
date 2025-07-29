/**
 * @file rp2040_reg.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief RP2040のレジスタ定義
 * @version 0.1
 * @date 2025-7-27
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#ifndef RP2040_REG_H
#define RP2040_REG_H

#include <stdint.h>

#define SYSINFO_REG_BASE            0x40000000
#define SYSINFO_CPUID_REG_OFFSET    0x00000000
#define SIO_REG_BASE                0xD0000000
#define SIO_REG_OFFSET              0x00000000
#define M0PLUS_REG_BASE             0xE0000000
#define M0PLUS_REG_OFFSET           0x0000ED00

#define SYSINFO_CPUID_REG_ADDR  (SYSINFO_REG_BASE + SYSINFO_CPUID_REG_OFFSET)
#define SIO_CPUID_REG_ADDR      (SIO_REG_BASE + SIO_REG_OFFSET)
#define M0PLUS_CPUID_REG_ADDR   (M0PLUS_REG_BASE + M0PLUS_REG_OFFSET)

typedef union {
    uint32_t DWORD;
    struct {
        uint32_t REVISION:4;        // bit[3:0]
        uint32_t PARTNO:12;         // bit[15:4]
        uint32_t ARCHITECTURE:4;    // bit[19:16]
        uint32_t VARIANT:4;         // bit[23:20]
        uint32_t IMPLEMENTER:8;     // bit[31:24]
    }BIT;
} M0PLUS_CPUID;

typedef union {
    uint32_t DWORD;
} SIO_CPUID;

// 0x40000000 ... CHIP_ID Reg
typedef union {
    uint32_t DWORD;
    struct {
        uint32_t MANUFACTURER:12;    // bit[11:0]
        uint32_t PART:16;            // bit[27:12]
        uint32_t REVISION:4;         // bit[31:28]
    }BIT;
} SYSINFO_CPUID;
#endif /* RP2040_REG_H */