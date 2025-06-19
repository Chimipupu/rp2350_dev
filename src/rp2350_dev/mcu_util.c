/**
 * @file mcu_util.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief マイコン依存のユーティリティ
 * @version 0.1
 * @date 2025-06-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "mcu_util.h"

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