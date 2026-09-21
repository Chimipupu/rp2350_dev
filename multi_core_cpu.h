/**
 * @file multi_core_cpu.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief マルチコアCPU
 * @note 詳細設計書:「RP2350評価FW_詳細設計_初版」のシート「CPU_FIFOデータフォーマット」
 * @version 0.1
 * @date 2026-09-06
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */
#ifndef MULTI_CORE_CPU_H
#define MULTI_CORE_CPU_H

#include <stdint.h>

// ---------------------------------------------------
#define CPU_FIFO_DATA_SIZE           4 // CPU FIFOのデータ長
#define CPU_FIFO_BUF_SIZE            8 // CPU FIFOのバッファサイズ
#define CPU_FIFO_DATA_INIT_MSG       0xFFFFFFFF
#define CPU_FIFO_DATA_NONE           0

#define CPU_FIFO_DATA_BIT            0x00FFFFFF
#define CPU_FIFO_DATA_TYPE_BIT       0xFF000000

typedef enum {
    CPU_FIFO_DATA_TYPE_CONFIG = 0,    // 設定データ
    CPU_FIFO_DATA_TYPE_REQUEST,       // データ要求
    CPU_FIFO_DATA_TYPE_RESULT,        // 設定/要求結果
    CPU_FIFO_DATA_TYPE_MSG_ASCII,     // ASCII
    CPU_FIFO_DATA_TYPE_RGBLED,        // RGBLEDデータ
    CPU_FIFO_DATA_TYPE_DEBUG,         // デバッグデータ
} E_CPU_FIFO_DATA_TYPE;

typedef enum {
    CPU_FIFO_RESULT_NONE = 0,         // 未処理状態
    CPU_FIFO_RESULT_BUSY,             // 処理中
    CPU_FIFO_RESULT_OK,               // 処理完了
    CPU_FIFO_RESULT_ERROR_CRC32,      // CRC32エラー
    CPU_FIFO_RESULT_ERROR = 0xFF,     // 不明なエラー
} E_CPU_FIFO_RESULT;

typedef struct {
    uint32_t fifo[CPU_FIFO_BUF_SIZE];
    uint8_t idx;
    uint8_t cnt;
} fifo_t;

typedef struct {
    fifo_t tx;
    fifo_t rx;
} cpu_fifo_t;

// ---------------------------------------------------
bool cpu_fifo_tx_data(uint32_t data);
bool cpu_fifo_rx_data(uint32_t *p_data);
void set_cpu_fifo_tx_data(uint32_t data);
void dump_cpu_fifo_buf(void);
void cpu_core_0_init(void);
void cpu_core_1_init(void);
void cpu_core_0_main(void);
void cpu_core_1_main(void);

// ---------------------------------------------------

#endif // MULTI_CORE_CPU_H