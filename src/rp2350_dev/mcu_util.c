/**
 * @file mcu_util.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief マイコン依存(RP2350)のユーティリティ
 * @version 0.1
 * @date 2025-06-20
 * 
 * @copyright Copyright (c) 2025 Chimipupu(https://github.com/Chimipupu)
 * 
 */
#include "mcu_util.h"
#include "mcu_board_def.h"

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
    size_t total_len = len + 1 + 8;
    size_t rem = (len + 1 + 8) % 64;
    size_t pad_len = (rem > 0) ? (64 - rem) : 0;
    size_t padded_len = len + 1 + pad_len + 8;
    uint64_t bit_len = (uint64_t)len * 8;

    memcpy(p_dst_buf, p_src_buf, len);
    p_dst_buf[len] = 0x80;
    memset(p_dst_buf + len + 1, 0, pad_len);

    size_t bit_len_pos = len + 1 + pad_len;
    for (int i = 0; i < 8; i++)
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