#ifndef DBG_COM_H
#define DBG_COM_H

#include <stdio.h>
#include <stdint.h>
#include "rp2350_dev.h"

#define DBG_CMD_MAX_LEN 32 // コマンドの最大長
#define DBG_CMD_MAX_ARGS 4 // コマンドの最大引数数

// コマンドの種類
typedef enum {
    CMD_HELP,       // ヘルプ表示
    CMD_VER,        // バージョン表示
    CMD_CLOCK,      // クロック情報表示
    CMD_FP_TEST,    // 浮動小数点演算テスト
    CMD_PI_CALC,    // 円周率計算
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

// 関数プロトタイプ
void dbg_com_init(void);
void dbg_com_process(void);

#endif // DBG_COM_H
