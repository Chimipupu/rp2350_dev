/**
 * @file dbg_cmd.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief デバッグシリアルコマンド
 * @version 0.1
 * @date 2026-08-23
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */

#ifndef DBG_CMD_H
#define DBG_CMD_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// --------------------------------------------------------------------------
#define KEY_BACK_SPACE      0x08
#define KEY_DEL             0x7F

// 文字色
#define ASCII_TXT_RED       "\33[31m"
#define ASCII_TXT_GREEN     "\33[32m"
#define ASCII_TXT_YELLOW    "\33[33m"
#define ASCII_TXT_BLUE      "\33[34m"
#define ASCII_TXT_MAGENTA   "\33[35m"
#define ASCII_TXT_CYAN      "\33[36m"
#define ASCII_TXT_WHITE     "\33[37m"

// 高輝度文字色
#define ASCII_TXT_B_RED     "\33[91m"
#define ASCII_TXT_B_GREEN   "\33[92m"
#define ASCII_TXT_B_YELLOW  "\33[93m"
#define ASCII_TXT_B_BLUE    "\33[94m"
#define ASCII_TXT_B_MAGENTA "\33[95m"
#define ASCII_TXT_B_CYAN    "\33[96m"
#define ASCII_TXT_B_WHITE   "\33[97m"

// 背景色
#define ASCII_BG_BLACK      "\33[40m"
#define ASCII_BG_RED        "\33[41m"
#define ASCII_BG_GREEN      "\33[42m"
#define ASCII_BG_YELLOW     "\33[43m"
#define ASCII_BG_BLUE       "\33[44m"
#define ASCII_BG_MAGENTA    "\33[45m"
#define ASCII_BG_CYAN       "\33[46m"
#define ASCII_BG_WHITE      "\33[47m"

// 文字装飾 (Text Style)
#define ASCII_STYLE_BOLD    "\33[1m"  // 太字
#define ASCII_STYLE_UNDER   "\33[4m"  // 下線
#define ASCII_STYLE_BLINK   "\33[5m"  // 点滅
#define ASCII_STYLE_REVERSE "\33[7m"  // 反転

// リセット
#define ASCII_RESET         "\33[0m"  // すべての設定をクリア
#define ANSI_ESC_CLEAR      "\33[2J\33[H" // 画面消去
// --------------------------------------------------------------------------
#define DBG_CMD_MAX_ARGS    4

typedef struct {
    uint8_t argc;
    char *argv[DBG_CMD_MAX_ARGS];
} dbg_cmd_args_t;

typedef enum {
    CMD_TYPE_BASIC = 0x00, // 基本コマンド
    CMD_TYPE_EXT,          // 拡張コマンド
    CMD_TYPE_NONE = 0xFF   // コマンドなし
} E_DBG_CMD_TYPE;

// コマンド実行結果
typedef enum {
    CMD_RESULT_NONE                 = 0x00, // コマンド処理なし
    CMD_RESULT_EXEC_OK              = 0x01, // コマンド実行成功
    CMD_RESULT_EXEC_ERROR           = 0x02, // コマンド実行失敗
    CMD_RESULT_EXEC_IN_PROGRESS     = 0x03, // コマンド実行中
    CMD_RESULT_ARGS_ERROR           = 0xF0, // コマンド引数エラー
    CMD_RESULT_UNKNOWN_ERROR        = 0xFF, // 不明なエラー
} E_DBG_CMD_RESULT;

// コマンドコールバック関数
typedef E_DBG_CMD_RESULT(*p_cmd_func)(void *p_args);

// コマンドテーブル構造体
typedef struct {
    const char *p_cmd_str;          // コマンド文字列
    const char *p_cmd_str_short;    // コマンド短縮文字列
    p_cmd_func pfunc;               // コールバック関数
} dbg_cmd_tbl_t;

// シリアル Read/Write関数
typedef uint8_t (*p_serial_read_func)(void);
typedef size_t (*p_printf_func)(const char *p_str, ...);

// コマンド設定構造体
typedef struct {
    p_serial_read_func p_serial_read;
    p_printf_func p_printf;
    dbg_cmd_tbl_t *p_ext_cmd_tbl;         // 拡張コマンドのテーブルポインタ
    uint8_t ext_cmd_num;                  // 拡張コマンド数
} dbg_cmd_config_t;

// --------------------------------------------------------------------------
void dbg_cmd_init(dbg_cmd_config_t *p_cmd_config);
void dbg_cmd_poll(void);
void dbg_cmd_main(void);
// --------------------------------------------------------------------------

#endif // DBG_CMD_H