/**
 * @file dbg_com.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief デバッグモニタのヘッダ
 * @version 0.1
 * @date 2025-06-13
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#ifndef DBG_COM_H
#define DBG_COM_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "rp2350_util.h"
#include "pcb_def.h"

#include "pico/version.h"
#include "hardware/clocks.h"
#include "hardware/watchdog.h"
#include "hardware/timer.h"
#include "pico/time.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

// #define DEBUG_DBG_COM      // デバッグ用

// コマンド関連のマクロ
#define DBG_CMD_MAX_LEN         32 // コマンドの最大長
#define DBG_CMD_MAX_ARGS        5 // コマンドの最大引数数
#define CMD_HISTORY_MAX         16 // コマンド履歴の最大数

// GPIOの最大本数
#if defined(PCB_WEACT_RP2350A_V10) || defined(PCB_WEACT_RP2350B)
// RP2350B (QFN-80)
#define GPIO_MAX_PIN_NUM        48
#else
// RP2350A (QFN-60)
#define GPIO_MAX_PIN_NUM        30
#endif

// 期待値: tan(355/226)
#define TAN_355_226_EXPECTED -7497258.18532

// [タイマー関連定義]
#define TIMER_MAX_SECONDS 3600   // 最大1時間
// タイマーの最大アラーム数
#define TIMER_MAX_ALARMS      PICO_TIME_DEFAULT_ALARM_POOL_MAX_TIMERS

// [キーボード関連定義]
#define KEY_ESC         27    // ESCキー
#define KEY_BACKSPACE   127   // バックスペースキー
#define KEY_ANSI_ESC    '['   // ANSIエスケープシーケンス
#define KEY_UP          'A'   // 十字キーの矢印上
#define KEY_DOWN        'B'   // 十字キーの矢印下

// ANSI ESC(エスケープシーケンス)
#define ANSI_ESC_CLS            "\033[2J\033[H" // ANSI ESC 画面クリア
#define ANSI_TXT_COLOR_RESET    "\e[0m"         // ANSI ESC 文字色 リセット
#define ANSI_TXT_COLOR_WHITE    "\e[37sm"       // ANSI ESC 文字色 白
#define ANSI_TXT_COLOR_RED      "\e[31m"        // ANSI ESC 文字色 赤
#define ANSI_TXT_COLOR_GREEN    "\e[32m"        // ANSI ESC 文字色 緑
#define ANSI_TXT_COLOR_YELLOW   "\e[33m"        // ANSI ESC 文字色 黄
#define ANSI_TXT_COLOR_BLUE     "\e[34m"        // ANSI ESC 文字色 青
#define ANSI_TXT_COLOR_PURPLE   "\e[35m"        // ANSI ESC 文字色 紫
#define ANSI_TXT_COLOR_MAGENTA  "\e[36m"        // ANSI ESC 文字色 マゼンタ

// コマンドの種類
typedef enum {
    CMD_HELP,       // ヘルプ表示
    CMD_CLS,        // 画面クリア
    CMD_SYSTEM,     // システム情報表示
    CMD_RST,        // リセット
    CMD_MEM_DUMP,   // メモリダンプ
    CMD_REG,        // レジスタ操作8/16/32bit
    CMD_GPIO,       // GPIO制御
    CMD_I2C,        // I2C制御
    CMD_NEOPIXEL,   // NeoPixel制御
    CMD_TIMER,      // タイマーコマンド
    CMD_RND,        // 真性乱数をH/WのTRANGで生成
    CMD_SHA,        // H/WでSHA-256のハッシュ値を計算
    CMD_MCT,        // マルチコアテスト
    CMD_MT_TEST,    // 論理演算/四則演算/数学アプリのテスト
    CMD_UNKNOWN     // 不明なコマンド
} dbg_cmd_t;

// コマンド構造体
typedef struct {
    const char* p_cmd_str;           // コマンド文字列
    dbg_cmd_t cmd_type;              // コマンド種類
    const char* p_description;       // コマンドの説明
    int32_t min_args;                // 最小引数数
    int32_t max_args;                // 最大引数数
} dbg_cmd_info_t;

#pragma once
extern const dbg_cmd_info_t g_cmd_tbl[];

// コマンド引数構造体
typedef struct {
    int32_t argc;                    // 引数の数
    char* p_argv[DBG_CMD_MAX_ARGS];  // 引数の配列
} dbg_cmd_args_t;

// タイマー状態
typedef struct {
    bool is_running;                // タイマー実行中フラグ
    uint32_t req_time_sec;          // 設定時間 (秒)
    uint32_t start_time;            // 開始時間
    uint32_t duration;              // 設定時間（us）
    alarm_id_t alarm_id;            // アラームID
    uint8_t reg_order;              // 登録順序（1から開始）
} timer_state_t;

// 関数プロトタイプ
void dbg_com_init(void);
void dbg_com_main(void);

#endif // DBG_COM_H