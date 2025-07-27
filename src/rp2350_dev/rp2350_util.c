/**
 * @file rp2350_util.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief RP2350固有機能ユーティリティ
 * @version 0.1
 * @date 2025-07-27
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#include "rp2350_util.h"
#include "pcb_def.h"

#include "hardware/adc.h"

const char *p_cpu_name_str = "M33";

/**
 * @brief 真性乱数をH/WのTRANGで生成(u32)
 * 
 * @param p_rand_buf バッファポインタ
 * @param gen_num_cnt 生成個数
 */
void trang_gen_rand_num_u32(uint32_t *p_rand_buf, uint32_t gen_num_cnt)
{
    if (gen_num_cnt > 0)
    {
        for (uint32_t i = 0; i < gen_num_cnt; i++)
        {
            p_rand_buf[i] = get_rand_32();
        }
    }
}

/**
 * @brief SHA-256のパディング処理(FIPS 180-4準拠)
 * 
 * @param p_src_buf 入力データのポインタ
 * @param len 入力データの長さ（バイト単位）
 * @param p_dst_buf 出力バッファのポインタ（最低 len + 64 バイト以上必要）
 * @param p_out_len パディング後の長さを格納するポインタ
 */
void sha256_padding(const uint8_t *p_src_buf, size_t len, uint8_t *p_dst_buf, size_t *p_out_len)
{
    uint8_t i;
    size_t total_len = len + 1 + 8;
    size_t rem = (len + 1 + 8) % 64;
    size_t pad_len = (rem > 0) ? (64 - rem) : 0;
    size_t padded_len = len + 1 + pad_len + 8;
    uint64_t bit_len = (uint64_t)len * 8;

    memcpy(p_dst_buf, p_src_buf, len);
    p_dst_buf[len] = 0x80;
    memset(p_dst_buf + len + 1, 0, pad_len);

    size_t bit_len_pos = len + 1 + pad_len;
    for (i = 0; i < 8; i++)
    {
        p_dst_buf[bit_len_pos + 7 - i] = (uint8_t)(bit_len >> (i * 8));
    }

    *p_out_len = padded_len;
}

/**
 * @brief H/WでSHA-256のハッシュ値を計算
 * 
 * @param p_data_buf 入力データバッファのポインタ
 * @param len 入力データ長
 * @param p_hash_buf ハッシュ値の格納先バッファのポインタ
 */
void hardware_calc_sha256(const uint8_t *p_data_buf, size_t len, uint8_t *p_hash_buf)
{
    volatile uint32_t word_val;
    volatile uint32_t *p_write_addr;

    sha256_set_dma_size(4);
    sha256_set_bswap(true); // H/Wでエンディング変換
    sha256_start();

    // リトルエンディアンで書き込み
    for (size_t i = 0; i < len; i += 4)
    {
        sha256_wait_ready_blocking();
        p_write_addr = (volatile uint32_t *)sha256_get_write_addr();

        word_val = 0;
        for (size_t j = 0; j < 4; j++)
        {
            if (i + j < len) {
                word_val |= ((uint32_t)p_data_buf[i + j]) << (8 * j);
            }
        }

        *p_write_addr = word_val;
    }

    sha256_wait_valid_blocking();

    // (DEBUG)デバッグしてわかったこと
    // リザルトが「0x6A09E667BB67AE853C6EF372A54FF53A510E527F9B05688C1F83D9AB5BE0CD19」
    // -> これはSHA-256の初期化ベクトル（IV）で
    // リザルトこれがだとまだH/Wがハッシュを計算できてない！
    sha256_result_t result;
    sha256_get_result(&result, SHA256_BIG_ENDIAN);
    memcpy(p_hash_buf, result.bytes, 32);
}

uint32_t get_multicore_fifo(void)
{
    uint32_t data = 0;

    // ブロッキング
    // data = multicore_fifo_pop_blocking();

    // ブロッキング(タイムアウト=100us)
    multicore_fifo_pop_timeout_us(100, &data);

    return data;
}

void set_multicore_fifo(uint32_t data)
{
    multicore_fifo_push_blocking(data);
}

/**
 * @brief 内蔵CPU温度センサの取得
 * 
 * @return float 内蔵CPU温度センサの温度
 */
float get_cpu_temp_from_adc(void)
{
    uint16_t raw_adc = adc_read();
    float voltage = (raw_adc * VREF_VOLTAGE) / (1 << 12);
    // データシート(12.4.6 . Temperature Sensor)の計算式
    float cpu_temp = 27.0f - ((voltage - 0.706f) / 0.001721f);

    return cpu_temp;
}

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