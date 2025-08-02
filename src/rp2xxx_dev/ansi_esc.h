/**
 * @file ansi_esc.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief ANSI ESC
 * @version 0.1
 * @date 2025-07-31
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#ifndef ANSI_ESC_H
#define ANSI_ESC_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

// 文字
#define ANSI_ESC_PG_RESET               "\x1b[0m"
#define ANSI_ESC_PG_BOLD                "\x1b[1m"
#define ANSI_ESC_PG_UNDERLINE           "\x1b[4m"

// 背景色
#define ANSI_ESC_BG_BLACK               "\x1b[40m"
#define ANSI_ESC_BG_RED                 "\x1b[41m"
#define ANSI_ESC_BG_GREEN               "\x1b[42m"
#define ANSI_ESC_BG_YELLOW              "\x1b[43m"
#define ANSI_ESC_BG_BLUE                "\x1b[44m"
#define ANSI_ESC_BG_MAGENTA             "\x1b[45m"
#define ANSI_ESC_BG_CYAN                "\x1b[46m"
#define ANSI_ESC_BG_WHITE               "\x1b[47m"

// 文字色
#define ANSI_ESC_PG_REVERSE             "\x1b[7m"
#define ANSI_ESC_PG_BLACK               "\x1b[30m"
#define ANSI_ESC_PG_RED                 "\x1b[31m"
#define ANSI_ESC_PG_GREEN               "\x1b[32m"
#define ANSI_ESC_PG_YELLOW              "\x1b[33m"
#define ANSI_ESC_PG_BLUE                "\x1b[34m"
#define ANSI_ESC_PG_MAGENTA             "\x1b[35m"
#define ANSI_ESC_PG_CYAN                "\x1b[36m"
#define ANSI_ESC_PG_WHITE               "\x1b[37m"

// 明るい文字色
#define ANSI_ESC_PG_BRIGHT_BLACK        "\x1b[90m"
#define ANSI_ESC_PG_BRIGHT_RED          "\x1b[91m"
#define ANSI_ESC_PG_BRIGHT_GREEN        "\x1b[92m"
#define ANSI_ESC_PG_BRIGHT_YELLOW       "\x1b[93m"
#define ANSI_ESC_PG_BRIGHT_BLUE         "\x1b[94m"
#define ANSI_ESC_PG_BRIGHT_MAGENTA      "\x1b[95m"
#define ANSI_ESC_PG_BRIGHT_CYAN         "\x1b[96m"
#define ANSI_ESC_PG_BRIGHT_WHITE        "\x1b[97m"

// 明るい背景色
#define ANSI_ESC_BG_BRIGHT_BLACK        "\x1b[100m"
#define ANSI_ESC_BG_BRIGHT_RED          "\x1b[101m"
#define ANSI_ESC_BG_BRIGHT_GREEN        "\x1b[102m"
#define ANSI_ESC_BG_BRIGHT_YELLOW       "\x1b[103m"
#define ANSI_ESC_BG_BRIGHT_BLUE         "\x1b[104m"
#define ANSI_ESC_BG_BRIGHT_MAGENTA      "\x1b[105m"
#define ANSI_ESC_BG_BRIGHT_CYAN         "\x1b[106m"
#define ANSI_ESC_BG_BRIGHT_WHITE        "\x1b[107m"

// その他装飾
#define ANSI_ESC_PG_DIM                 "\x1b[2m"
#define ANSI_ESC_PG_ITALIC              "\x1b[3m"
#define ANSI_ESC_PG_BLINK               "\x1b[5m"
#define ANSI_ESC_PG_HIDDEN              "\x1b[8m"
#define ANSI_ESC_PG_STRIKE              "\x1b[9m"

// カーソル移動
#define ANSI_ESC_CURSOR_UP(n)           "\x1b[" #n "A"
#define ANSI_ESC_CURSOR_DOWN(n)         "\x1b[" #n "B"
#define ANSI_ESC_CURSOR_FORWARD(n)      "\x1b[" #n "C"
#define ANSI_ESC_CURSOR_BACK(n)         "\x1b[" #n "D"

// 画面クリア
#define ANSI_ESC_CLEAR_SCREEN           "\x1b[2J"
#define ANSI_ESC_CLEAR_LINE             "\x1b[2K"

// カーソル位置指定（X:列, Y:行）
#define ANSI_ESC_CURSOR_POS(Y,X)        "\x1b[" #Y ";" #X "H"

// カーソル保存・復元
#define ANSI_ESC_SAVE_CURSOR            "\x1b[s"
#define ANSI_ESC_RESTORE_CURSOR         "\x1b[u"

// スクロール
#define ANSI_ESC_SCROLL_UP(n)           "\x1b[" #n "S"
#define ANSI_ESC_SCROLL_DOWN(n)         "\x1b[" #n "T"

// 画面全体クリア＆カーソルホーム
#define ANSI_ESC_CLEAR_ALL              "\x1b[2J\x1b[H"

// 画面バッファ切替（主/副）
#define ANSI_ESC_ALT_SCREEN_ON          "\x1b[?1049h"
#define ANSI_ESC_ALT_SCREEN_OFF         "\x1b[?1049l"

#endif // ANSI_ESC_H