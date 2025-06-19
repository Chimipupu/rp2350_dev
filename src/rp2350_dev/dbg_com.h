/**
 * @file dbg_com.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief デバッグモニタのヘッダ
 * @version 0.1
 * @date 2025-06-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef DBG_COM_H
#define DBG_COM_H

#include "common.h"
#include "pcb_def.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "pico/version.h"
#include "hardware/clocks.h"
#include "hardware/watchdog.h"
#include "hardware/timer.h"
#include "pico/time.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

// #define DEBUG_DBG_COM      // デバッグ用

// コマンド関連のマクロ
#define DBG_CMD_MAX_LEN 32 // コマンドの最大長
#define DBG_CMD_MAX_ARGS 5 // コマンドの最大引数数
#define CMD_HISTORY_MAX 16 // コマンド履歴の最大数

// GPIOの最大ピン番号（RP2350）
#define GPIO_PIN_NUM_MAX 29

// 期待値: tan(355/226)
#define TAN_355_226_EXPECTED -7497258.18532

// タイマー関連の定数
#define TIMER_MAX_SECONDS 3600  // 最大1時間
#define TIMER_MAX_ALARMS 4      // RP2350のH/Wタイマー数

// キーボードのコード定義
#define KEY_ESC         27    // ESCキー
#define KEY_BACKSPACE   127   // バックスペースキー
#define KEY_ANSI_ESC    '['   // ANSIエスケープシーケンス
#define KEY_UP          'A'   // 十字キーの矢印上
#define KEY_DOWN        'B'   // 十字キーの矢印下

// 【メモリダンプコマンド】
// 例) mem_dump #00000000 #100

// コマンドの種類
typedef enum {
    CMD_HELP,       // ヘルプ表示
    CMD_VER,        // バージョン表示
    CMD_SYSTEM,     // システム情報表示
    CMD_RND,        // 真性乱数をH/WのTRANGで生成,
    CMD_AT_TEST,    // int/float/double四則演算テスト
    CMD_PI_CALC,    // 円周率計算
    CMD_TRIG,       // 三角関数テスト
    CMD_ATAN2,      // atan2テスト
    CMD_TAN355,     // tan(355/226)テスト
    CMD_ISQRT,      // 逆平方根テスト
    CMD_TIMER,      // タイマーコマンド
    CMD_GPIO,       // GPIO制御
    CMD_I2C,        // I2C制御
    CMD_MEM_DUMP,   // メモリダンプ
    CMD_REG,        // レジスタ操作8/16/32bit
    CMD_RST,        // リセット
    CMD_UNKNOWN     // 不明なコマンド
} dbg_cmd_t;

// コマンド構造体
typedef struct {
    const char* p_cmd_str;     // コマンド文字列
    dbg_cmd_t cmd_type;        // コマンド種類
    const char* p_description; // コマンドの説明
    int32_t min_args;          // 最小引数数
    int32_t max_args;          // 最大引数数
} dbg_cmd_info_t;

// コマンド引数構造体
typedef struct {
    int32_t argc;                    // 引数の数
    char* p_argv[DBG_CMD_MAX_ARGS]; // 引数の配列
} dbg_cmd_args_t;

// タイマー状態
typedef struct {
    bool is_running;      // タイマー実行中フラグ
    uint32_t start_time;  // 開始時間
    uint32_t duration;    // 設定時間（us）
    alarm_id_t alarm_id;  // アラームID
    uint8_t reg_order;    // 登録順序（1から開始）
} timer_state_t;

// 関数プロトタイプ
void dbg_com_init(void);
void dbg_com_process(void);

#endif // DBG_COM_H