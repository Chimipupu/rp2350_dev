#include "dbg_com.h"
#include "app_main.h"
#include "pico/version.h"
#include <stdio.h>
#include <string.h>
#include "hardware/clocks.h"
#include "hardware/watchdog.h"
#include <stdlib.h>

// コマンドテーブル
static const dbg_cmd_info_t s_cmd_table[] = {
    {"help",    CMD_HELP,    "Show this help message", 0, 0},
    {"ver",     CMD_VER,     "Show version information", 0, 0},
    {"clock",   CMD_CLOCK,   "Show clock information", 0, 0},
    {"fp",      CMD_FP_TEST, "Run floating point test", 0, 0},
    {"pi",      CMD_PI_CALC, "Calculate pi using Gauss-Legendre [iterations]", 0, 1},
    {"rst",     CMD_RST,     "Reset the system", 0, 0},
    {NULL,      CMD_UNKNOWN, NULL, 0, 0}
};

// コマンドバッファ
static char s_cmd_buffer[DBG_CMD_MAX_LEN];
static int32_t s_cmd_index = 0;

/**
 * @brief 文字列をトークンに分割する
 * 
 * @param p_str 分割する文字列
 * @param p_args 引数構造体
 * @return int32_t トークンの数
 */
static int32_t split_string(char* p_str, dbg_cmd_args_t* p_args)
{
    p_args->argc = 0;
    char* p_token = strtok(p_str, " ");

    while (p_token != NULL && p_args->argc < DBG_CMD_MAX_ARGS) {
        p_args->p_argv[p_args->argc++] = p_token;
        p_token = strtok(NULL, " ");
    }

    return p_args->argc;
}

/**
 * @brief デバッグコマンドモニターの初期化
 */
void dbg_com_init(void)
{
    printf("\nDebug Command Monitor\n");
    printf("Type 'help' for available commands\n");
    printf("> ");
}

/**
 * @brief コマンド文字列を解析してコマンド種類を返す
 * 
 * @param p_cmd_str コマンド文字列
 * @param p_args 引数構造体
 * @return dbg_cmd_t コマンド種類
 */
static dbg_cmd_t dbg_com_parse_cmd(const char* p_cmd_str, dbg_cmd_args_t* p_args)
{
    for (int32_t i = 0; s_cmd_table[i].p_cmd_str != NULL; i++) {
        if (strcmp(p_cmd_str, s_cmd_table[i].p_cmd_str) == 0) {
            // 引数の数をチェック
            if (p_args->argc - 1 < s_cmd_table[i].min_args || p_args->argc - 1 > s_cmd_table[i].max_args) {
                printf("Error: Invalid number of arguments. Expected %d-%d, got %d\n",
                    s_cmd_table[i].min_args, s_cmd_table[i].max_args, p_args->argc - 1);
                return CMD_UNKNOWN;
            }
            return s_cmd_table[i].cmd_type;
        }
    }
    return CMD_UNKNOWN;
}

/**
 * @brief コマンドを実行する
 * 
 * @param cmd コマンド種類
 * @param p_args 引数構造体
 */
static void dbg_com_execute_cmd(dbg_cmd_t cmd, const dbg_cmd_args_t* p_args)
{
    switch (cmd) {
        case CMD_HELP:
            printf("\nAvailable commands:\n");
            for (int32_t i = 0; s_cmd_table[i].p_cmd_str != NULL; i++) {
                printf("  %-10s - %s\n", s_cmd_table[i].p_cmd_str, s_cmd_table[i].p_description);
            }
            break;

        case CMD_VER:
            printf("Pico SDK version: %d.%d.%d\n",
                PICO_SDK_VERSION_MAJOR,
                PICO_SDK_VERSION_MINOR,
                PICO_SDK_VERSION_REVISION);
            break;

        case CMD_CLOCK:
            printf("System Clock:\t%d MHz\n", clock_get_hz(clk_sys) / 1000000);
            printf("USB Clock:\t%d MHz\n", clock_get_hz(clk_usb) / 1000000);
            break;

        case CMD_FP_TEST:
            measure_execution_time(floating_point_test, "floating_point_test");
            break;

        case CMD_PI_CALC: {
            int32_t iterations = 3;
            if (p_args->argc > 1) {
                iterations = atoi(p_args->p_argv[1]);
                if (iterations <= 0) {
                    printf("Error: Invalid iteration count. Must be positive.\n");
                    break;
                }
            }
            printf("\nCalculating Pi using Gauss-Legendre algorithm (%d iterations):\n", iterations);
            for (int32_t i = 1; i <= iterations; i++) {
                double pi = calculate_pi_gauss_legendre(i);
                printf("Iteration %d: π ≈ %.15f\n", i, pi);
            }
            break;
        }

        case CMD_RST:
            printf("Resetting system...\n");
            watchdog_reboot(0, 0, 0);   // WDTで即時リセット
            break;

        case CMD_UNKNOWN:
            printf("Unknown command. Type 'help' for available commands.\n");
            break;
    }
    printf("> ");
}

/**
 * @brief デバッグコマンドモニターのメイン処理
 */
void dbg_com_process(void)
{
    if (s_cmd_index >= DBG_CMD_MAX_LEN - 1) {
        s_cmd_index = 0;
    }

    int32_t c = getchar();
    if (c == '\r' || c == '\n') {
        if (s_cmd_index > 0) {
            s_cmd_buffer[s_cmd_index] = '\0';
            printf("\n");

            dbg_cmd_args_t args;
            split_string(s_cmd_buffer, &args);
            if (args.argc > 0) {
                dbg_cmd_t cmd = dbg_com_parse_cmd(args.p_argv[0], &args);
                dbg_com_execute_cmd(cmd, &args);
            }
            s_cmd_index = 0;
        } else {
            printf("\n> ");
        }
    } else if (c == '\b' || c == 127) {
        if (s_cmd_index > 0) {
            s_cmd_index--;
            printf("\b \b");
        }
    } else if (c >= ' ' && c <= '~') {
        s_cmd_buffer[s_cmd_index++] = c;
        putchar(c);
    }
}