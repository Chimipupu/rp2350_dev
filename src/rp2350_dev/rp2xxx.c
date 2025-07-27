/**
 * @file rp2xxx.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief RP2350の固有機能関連
 * @version 0.1
 * @date 2025-7-27
 * 
 * @copyright Copyright (c) 2025 Chimipupu
 * 
 */

#include "rp2xxx.h"
#include "mcu_util.h"

const char *p_cpu_name_str = "M33";

/**
 * @brief RP2040/RP2350のCPUのコア番号の取得
 * 
 * @return uint8_t 0=CPU0(コア0), 1=CPU1(コア1)
 */
uint8_t rp2xxx_get_cpu_core_num(void)
{
    uint8_t core_num = 0;
    uint32_t cpu_core = REG_READ_DWORD(SIO_REG_BASE, SIO_REG_OFFSET);

    if(cpu_core != 0) {
        core_num = 1;
    }

    return core_num;
}

/**
 * @brief RP2040/RP2350のチップRevisonの取得
 * 
 * @return uint8_t RP2040 ... B0/B1/B2, RP2350 ... A2
 */
uint8_t rp2xxx_read_reg_chip_rev(void)
{
    uint8_t chip_rev = 0;
    SYSINFO_CPUID SYSINFO_CPUID_REG;

    SYSINFO_CPUID_REG.DWORD = (*(volatile uint32_t *)SYSINFO_CPUID_REG_ADDR);
    chip_rev = SYSINFO_CPUID_REG.BIT.REVISION;

    return chip_rev;
}

/**
 * @brief Pico SDKのバージョン表示関数
 * 
 */
void pico_sdk_version_print(void)
{
    printf("Pico SDK Ver.%s\n", PICO_SDK_VERSION_STRING);
}

/**
 * @brief RP2350のチップパッケージの表示
 * 
 */
void rp2xxx_chip_package_print(void)
{
    SYSINFO_PACKAGE_SEL SYSINFO_PACKAGE_SEL_REG;    // SYSINFO PACKAGE_SELレジスタ

    SYSINFO_PACKAGE_SEL_REG.DWORD = (*(volatile uint32_t *)SYSINFO_PACKAGE_SEL_REG_ADDR);

    if(SYSINFO_PACKAGE_SEL_REG.BIT.PACKAGE != CHIP_PACKAGE_QFN80_RP2350B) {
        printf("Chip Package QFN-60, RP2350A Chip!\n");
    } else {
        printf("Chip Package QFN-80, RP2350B Chip!\n");
    }
}

/**
 * @brief RP2350のチップRevisonの表示
 * 
 */
void rp2xxx_chip_rev_print(void)
{
    printf("RP2350 Chip Rev = %d\n", rp2xxx_read_reg_chip_rev());
}

/**
 * @brief コアレジスタを読み出し
 * 
 */
void rp2xxx_reg_info(void)
{
    uint32_t cpu_core = 0xFF;
    SYSINFO_CPUID SYSINFO_CPUID_REG;                // SYSINFO CPUIDレジスタ
    SYSINFO_PACKAGE_SEL SYSINFO_PACKAGE_SEL_REG;    // SYSINFO PACKAGE_SELレジスタ
    SIO_CPUID SIO_CPUID_REG;                        // SIO CPUIDレジスタ

    SIO_CPUID_REG.DWORD = (*(volatile uint32_t *)SIO_CPUID_REG_ADDR);
    SYSINFO_CPUID_REG.DWORD = (*(volatile uint32_t *)SYSINFO_CPUID_REG_ADDR);
    SYSINFO_PACKAGE_SEL_REG.DWORD = (*(volatile uint32_t *)SYSINFO_PACKAGE_SEL_REG_ADDR);
    cpu_core = SIO_CPUID_REG.DWORD;

    printf("**************************************************************************\n");
    printf("[Core%X] ... RP2350 Reg Info\n", cpu_core);
    M33_CPUID M33_CPUID_REG;   // M33 CPUIDレジスタ
    M33_CPUID_REG.DWORD = (*(volatile uint32_t *)M33_CPUID_REG_ADDR);
    printf("**************************************************************************\n");

    // SIO CPUIDレジスタ(Core0 or Core1のチェック)
    printf("SIO CPUID Reg(0x%02X)\n", SIO_CPUID_REG_ADDR);
    printf("SIO CPUID Reg Val = 0x%08X\n", SIO_CPUID_REG.DWORD);
    printf("--------------------------------------------------------------------------\n");

    // SYSINFO CPUIDレジスタ
    printf("SYSINFO CPUID Reg(0x%02X)\n", SYSINFO_CPUID_REG);
    printf("SYSINFO CPUID Reg Val = 0x%02X\n", SYSINFO_CPUID_REG.DWORD);
    printf("SYSINFO CPUID Reg MANUFACTURER[11:0] = 0x%02X\n", SYSINFO_CPUID_REG.BIT.MANUFACTURER);
    printf("SYSINFO CPUID Reg PART[27:12] = 0x%02X\n", SYSINFO_CPUID_REG.BIT.PART);
    printf("SYSINFO CPUID Reg REVISION[31:28] = 0x%02X\n", SYSINFO_CPUID_REG.BIT.REVISION);

    printf("--------------------------------------------------------------------------\n");

    printf("SYSINFO PACKAGE_SEL Reg(0x%02X)\n", SYSINFO_PACKAGE_SEL_REG);
    printf("SYSINFO PACKAGE_SEL Reg Val = 0x%02X\n", SYSINFO_PACKAGE_SEL_REG.DWORD);
    printf("SYSINFO PACKAGE_SEL Reg Reserved[31:1] \n");
    printf("SYSINFO PACKAGE_SEL Reg PACKAGE[0] = 0x%02X (0 = QFN80, 1 = QFN60)\n", SYSINFO_PACKAGE_SEL_REG.BIT.PACKAGE);

    printf("--------------------------------------------------------------------------\n");

    // M33 CPUIDレジスタ
    printf("%s CPUID Reg(0x%02X)\n", p_cpu_name_str, M33_CPUID_REG_ADDR);
    printf("%s CPUID Reg Val = 0x%02X\n", p_cpu_name_str, M33_CPUID_REG.DWORD);
    printf("%s CPUID Reg REVISION[3:0] = 0x%02X\n", p_cpu_name_str, M33_CPUID_REG.BIT.REVISION);
    printf("%s CPUID Reg PARTNO[15:4] = 0x%02X\n", p_cpu_name_str, M33_CPUID_REG.BIT.PARTNO);
    printf("%s CPUID Reg ARCHITECTURE[19:16] = 0x%02X\n", p_cpu_name_str, M33_CPUID_REG.BIT.ARCHITECTURE);
    printf("%s CPUID Reg VARIANT[23:20] = 0x%02X\n", p_cpu_name_str, M33_CPUID_REG.BIT.VARIANT);
    printf("%s CPUID Reg IMPLEMENTER[31:24] = 0x%02X\n", p_cpu_name_str, M33_CPUID_REG.BIT.IMPLEMENTER);
    printf("**************************************************************************\n");
}