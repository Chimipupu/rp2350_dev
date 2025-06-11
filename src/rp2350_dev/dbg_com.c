#include "dbg_com.h"
#include "app_main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "pico/version.h"
#include "hardware/clocks.h"
#include "hardware/watchdog.h"
#include "hardware/timer.h"
#include "pico/time.h"
#include "hardware/gpio.h"

// コマンド履歴
static char s_cmd_history[CMD_HISTORY_MAX][DBG_CMD_MAX_LEN];
static uint8_t s_history_count = 0;  // コマンド履歴の数
static int8_t s_history_pos = -1;    // 現在の履歴位置（-1は最新）

static void dbg_com_init_msg(void);
static void cmd_help(void);
static void cmd_ver(void);
static void cmd_clock(void);
static void cmd_at_test(void);
static void cmd_pi_calc(const dbg_cmd_args_t* p_args);
static void cmd_rst(void);
static void cmd_unknown(void);
static void cmd_trig(void);
static void cmd_atan2(void);
static void cmd_tan355(void);
static void cmd_isqrt(void);
static void cmd_timer(const dbg_cmd_args_t* p_args);
static void cmd_gpio(const dbg_cmd_args_t* p_args);

// コマンドテーブル
static const dbg_cmd_info_t s_cmd_table[] = {
    {"help",    CMD_HELP,    "Show this help message", 0, 0},
    {"ver",     CMD_VER,     "Show version information", 0, 0},
    {"clock",   CMD_CLOCK,   "Show clock information", 0, 0},
    {"at",      CMD_AT_TEST, "int/float/double arithmetic test", 0, 0},
    {"pi",      CMD_PI_CALC, "Calculate pi using Gauss-Legendre", 0, 1},
    {"trig",    CMD_TRIG,    "Run sin,cos,tan functions test", 0, 0},
    {"atan2",   CMD_ATAN2,   "Run atan2 test", 0, 0},
    {"tan355",  CMD_TAN355,  "Run tan(355/226) test", 0, 0},
    {"isqrt",   CMD_ISQRT,   "Run 1/sqrt(x) test", 0, 0},
    {"timer",   CMD_TIMER,   "Set timer alarm (seconds)", 0, 1},
    {"gpio",    CMD_GPIO,    "Control GPIO pin (pin, value)", 2, 2},
    {"rst",     CMD_RST,     "Reboot", 0, 0},
    {NULL,      CMD_UNKNOWN, NULL, 0, 0}
};

// コマンドバッファ
static char s_cmd_buffer[DBG_CMD_MAX_LEN];
static int32_t s_cmd_index = 0;

// タイマー状態
static timer_state_t s_timer_state[TIMER_MAX_ALARMS] = {0};
static uint8_t s_available_orders[TIMER_MAX_ALARMS] = {1, 2, 3, 4};  // 利用可能な登録順序
static uint8_t s_available_count = TIMER_MAX_ALARMS;  // 利用可能な登録順序の数

// 利用可能な登録順序を昇順にソート
static void sort_available_orders(void) {
    for (int i = 0; i < s_available_count - 1; i++) {
        for (int j = 0; j < s_available_count - i - 1; j++) {
            if (s_available_orders[j] > s_available_orders[j + 1]) {
                uint8_t temp = s_available_orders[j];
                s_available_orders[j] = s_available_orders[j + 1];
                s_available_orders[j + 1] = temp;
            }
        }
    }
}

// タイマーコールバック関数
static int64_t timer_callback(alarm_id_t id, void *user_data) {
    // 対応するタイマーを探して状態を更新
    for (int i = 0; i < TIMER_MAX_ALARMS; i++) {
        if (s_timer_state[i].alarm_id == id) {
            printf("\nTimer #%d (alarm as #%d) alarm!\n", 
                i + 1, s_timer_state[i].reg_order);
            // 登録順序を再利用可能なリストに戻す
            s_available_orders[s_available_count++] = s_timer_state[i].reg_order;
            sort_available_orders();  // 利用可能な登録順序を昇順にソート
            s_timer_state[i].is_running = false;
            s_timer_state[i].alarm_id = 0;
            s_timer_state[i].reg_order = 0;
            break;
        }
    }
    printf("> ");
    return 0;  // 繰り返しなし
}

static void dbg_com_init_msg(void)
{
    printf("\nDebug Command Monitor for RP2350 Ver 0.1\n");
    printf("Copyright (c) 2025 Chimipupu(https://github.com/Chimipupu)\n");
    printf("Type 'help' for available commands\n");
}

static void cmd_help(void)
{
    dbg_com_init_msg();

    printf("\nAvailable commands:\n");
    for (int32_t i = 0; s_cmd_table[i].p_cmd_str != NULL; i++) {
        printf("  %-10s - %s\n", s_cmd_table[i].p_cmd_str, s_cmd_table[i].p_description);
    }
}

static void cmd_ver(void)
{
    printf("Pico SDK version: %d.%d.%d\n",
        PICO_SDK_VERSION_MAJOR,
        PICO_SDK_VERSION_MINOR,
        PICO_SDK_VERSION_REVISION);
}

static void cmd_clock(void)
{
    printf("System Clock:\t%d MHz\n", clock_get_hz(clk_sys) / 1000000);
    printf("USB Clock:\t%d MHz\n", clock_get_hz(clk_usb) / 1000000);
}

static void cmd_at_test(void)
{
    printf("\nInteger Arithmetic Test:\n");
    measure_execution_time(int_add_test, "int_add_test");
    measure_execution_time(int_sub_test, "int_sub_test");
    measure_execution_time(int_mul_test, "int_mul_test");
    measure_execution_time(int_div_test, "int_div_test");

    printf("\nFloat Arithmetic Tests:\n");
    measure_execution_time(float_add_test, "float_add_test");
    measure_execution_time(float_sub_test, "float_sub_test");
    measure_execution_time(float_mul_test, "float_mul_test");
    measure_execution_time(float_div_test, "float_div_test");

    printf("\nDouble Arithmetic Tests:\n");
    measure_execution_time(double_add_test, "double_add_test");
    measure_execution_time(double_sub_test, "double_sub_test");
    measure_execution_time(double_mul_test, "double_mul_test");
    measure_execution_time(double_div_test, "double_div_test");
}

static void cmd_pi_calc(const dbg_cmd_args_t* p_args)
{
    int32_t iterations = 3;
    volatile double pi;

    if (p_args->argc > 1) {
        iterations = atoi(p_args->p_argv[1]);
        if (iterations <= 0) {
            printf("Error: Invalid iteration count. Must be positive.\n");
            return;
        }
    }
    printf("\nCalculating Pi using Gauss-Legendre algorithm (%d iterations):\n", iterations);
    for (int32_t i = 1; i <= iterations; i++) {
        volatile uint32_t start_time = time_us_32();
        pi = calculate_pi_gauss_legendre(i);
        volatile uint32_t end_time = time_us_32();
        printf("Iteration %d: π ≈ %.15f (proc time: %u us)\n", i, pi, end_time - start_time);
    }
}

static void cmd_rst(void)
{
    printf("Resetting system...\n");
    watchdog_reboot(0, 0, 0);   // WDTで即時リセット
}

static void cmd_unknown(void)
{
    printf("Unknown command. Type 'help' for available commands.\n");
}

static void cmd_trig(void)
{
    printf("\nTrigonometric Functions Test:\n");
    measure_execution_time(trig_functions_test, "trig_functions_test");
    printf("Test completed: sin(45°), cos(45°), tan(45°)\n");
}

static void cmd_atan2(void)
{
    printf("\nAtan2 Test:\n");
    measure_execution_time(atan2_test, "atan2_test");
    printf("Test completed: atan2(1.0, 1.0)\n");
}

static void cmd_tan355(void)
{
    printf("\nTan(355/226) Test:\n");
    double result = tan(355.0 / 226.0);
    printf("Expected: %.5f\n", TAN_355_226_EXPECTED);
    printf("Calculated: %.5f\n", result);
    printf("Difference: %.5f (%.2f%%)\n", 
           result - TAN_355_226_EXPECTED,
           ((result - TAN_355_226_EXPECTED) / TAN_355_226_EXPECTED) * 100.0);
    measure_execution_time(tan_355_226_test, "tan_355_226_test");
}

static void cmd_isqrt(void)
{
    printf("\nInverse Square Root Test:\n");
    measure_execution_time(inverse_sqrt_test, "inverse_sqrt_test");
    printf("Test completed: 1/sqrt(x) for x = 2.0, 3.0, 4.0, 5.0\n");
}

/**
 * @brief タイマーコマンド関数
 * 
 * @param p_args コマンド引数の構造体ポインタ
 */
static void cmd_timer(const dbg_cmd_args_t* p_args)
{
    if (p_args->argc > 1) {
        int32_t seconds = atoi(p_args->p_argv[1]);
        if (seconds <= 0) {
            printf("Error: Invalid timer duration. Must be positive.\n");
            return;
        }
        if (seconds > TIMER_MAX_SECONDS) {
            printf("Error: Timer duration exceeds maximum of %d seconds.\n", TIMER_MAX_SECONDS);
            return;
        }

        // 空いているタイマースロットを探す
        int free_slot = -1;
        for (int i = 0; i < TIMER_MAX_ALARMS; i++) {
            if (!s_timer_state[i].is_running) {
                free_slot = i;
                break;
            }
        }

        if (free_slot == -1) {
            printf("Error: All %d hardware timers are in use.\n", TIMER_MAX_ALARMS);
            return;
        }

        // H/Wでアラームを設定（us単位）
        uint64_t delay_us = seconds * 1000000ULL;
        alarm_id_t alarm_id = add_alarm_in_us(delay_us, timer_callback, NULL, true);
        if (alarm_id > 0) {
            s_timer_state[free_slot].is_running = true;
            s_timer_state[free_slot].start_time = time_us_32();
            s_timer_state[free_slot].duration = delay_us;
            s_timer_state[free_slot].alarm_id = alarm_id;
            // 利用可能な登録順序から最小のものを使用
            s_timer_state[free_slot].reg_order = s_available_orders[0];
            // 使用した登録順序を削除し、残りを前に詰める
            for (int i = 0; i < s_available_count - 1; i++) {
                s_available_orders[i] = s_available_orders[i + 1];
            }
            s_available_count--;
            printf("Timer #%d (alarm as #%d) set for %d seconds.\n",
                    free_slot + 1, s_timer_state[free_slot].reg_order, seconds);
        } else {
            printf("Error: Failed to set timer.\n");
        }
    } else {
        // 引数なしの場合は現在のタイマー状態を表示
        bool any_running = false;
        for (int i = 0; i < TIMER_MAX_ALARMS; i++) {
            if (s_timer_state[i].is_running) {
                uint32_t elapsed = time_us_32() - s_timer_state[i].start_time;
                uint32_t remaining = (elapsed < s_timer_state[i].duration) ?
                                    (s_timer_state[i].duration - elapsed) : 0;
                printf("Timer #%d (alarm as #%d): %u seconds remaining.\n",
                        i + 1, s_timer_state[i].reg_order,
                       (remaining + 500000) / 1000000);  // 四捨五入
                any_running = true;
            }
        }
        if (!any_running) {
            printf("No timers are running.\n");
        }
    }
}

/**
 * @brief GPIO制御コマンド関数
 * 
 * @param p_args コマンド引数の構造体ポインタ
 */
static void cmd_gpio(const dbg_cmd_args_t* p_args)
{
    if (p_args->argc != 3) {
        printf("Error: Invalid number of arguments. Usage: gpio <pin> <value>\n\n");
        return;
    }

    // コマンドからGPIOのピン番号とピンの値を取得
    uint8_t pin = atoi(p_args->p_argv[1]);
    uint8_t value = atoi(p_args->p_argv[2]);

    // GPIOのピン番号チェック
    if (pin < 0 || pin > GPIO_PIN_NUM_MAX) {
        printf("Error: Invalid GPIO pin number. Must be between 0 and %d.\n\n", GPIO_PIN_NUM_MAX);
        return;
    }

    // Low/High以外は受け付けない
    if (value != 0 && value != 1) {
        printf("Error: Invalid GPIO value. Must be 0 or 1.\n\n");
        return;
    }

    // GPIOの初期化と設定
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);

    // GPIO操作の処理時間を計測
    volatile uint32_t start_time = time_us_32();
    gpio_put(pin, value);
    volatile uint32_t end_time = time_us_32();

    printf("GPIO %d set to %d (proc time: %u us)\n\n", pin, value, end_time - start_time);
}

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
 * @brief コマンドを履歴に追加する関数
 * 
 * @param p_cmd 追加するコマンド
 */
static void add_to_cmd_history(const char* p_cmd)
{
    // 履歴を1つずつ下にずらす
    for (int32_t i = CMD_HISTORY_MAX - 1; i > 0; i--) {
        strcpy(s_cmd_history[i], s_cmd_history[i - 1]);
    }

    // 新しいコマンドを追加
    strncpy(s_cmd_history[0], p_cmd, DBG_CMD_MAX_LEN - 1);
    s_cmd_history[0][DBG_CMD_MAX_LEN - 1] = '\0';

    // 履歴数を更新（最大値で制限）
    if (s_history_count < CMD_HISTORY_MAX) {
        s_history_count++;
    }

    // 履歴位置をリセット
    s_history_pos = -1;
}

/**
 * @brief デバッグコマンドモニターの初期化
 */
void dbg_com_init(void)
{
    cmd_help();
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
            cmd_help();
            break;

        case CMD_VER:
            cmd_ver();
            break;

        case CMD_CLOCK:
            cmd_clock();
            break;

        case CMD_AT_TEST:
            cmd_at_test();
            break;

        case CMD_PI_CALC:
            cmd_pi_calc(p_args);
            break;

        case CMD_TRIG:
            cmd_trig();
            break;

        case CMD_ATAN2:
            cmd_atan2();
            break;

        case CMD_TAN355:
            cmd_tan355();
            break;

        case CMD_ISQRT:
            cmd_isqrt();
            break;

        case CMD_TIMER:
            cmd_timer(p_args);
            break;

        case CMD_GPIO:
            cmd_gpio(p_args);
            break;

        case CMD_RST:
            cmd_rst();
            break;

        case CMD_UNKNOWN:
            cmd_unknown();
            break;
    }
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
    // デリミタでCRかLFが来たらコマンドの受付を終わる
    if (c == '\r' || c == '\n') {
        if (s_cmd_index > 0) {
            s_cmd_buffer[s_cmd_index] = '\0';
            printf("\n");

            // コマンド履歴に入力されたコマンドを追加
            add_to_cmd_history(s_cmd_buffer);

            dbg_cmd_args_t args;
            split_string(s_cmd_buffer, &args);
            if (args.argc > 0) {
                dbg_cmd_t cmd = dbg_com_parse_cmd(args.p_argv[0], &args);
                dbg_com_execute_cmd(cmd, &args);
            }
            s_cmd_index = 0;
            printf("> ");
        } else {
            printf("\n> ");
        }
    } else if (c == '\b' || c == KEY_BACKSPACE) {
        if (s_cmd_index > 0) {
            s_cmd_index--;
            printf("\b \b");
        }
    } else if (c == KEY_ESC) {  // ESC
        c = getchar();
        if (c == KEY_ANSI_ESC) {    // ANSI escape sequence
            c = getchar();
            if (c == KEY_UP) {  // キーボードの上矢印
                if (s_history_pos < s_history_count - 1) {
                    // 現在の入力バッファをクリア
                    while (s_cmd_index > 0) {
                        printf("\b \b");
                        s_cmd_index--;
                    }
                    // コマンド履歴を1つ古いものに
                    s_history_pos++;
                    strcpy(s_cmd_buffer, s_cmd_history[s_history_pos]);
                    s_cmd_index = strlen(s_cmd_buffer);
                    printf("%s", s_cmd_buffer);
                }
            } else if (c == KEY_DOWN) {  // キーボードの下矢印
                if (s_history_pos >= 0) {
                    // コマンド履歴の現在の入力バッファをクリア
                    while (s_cmd_index > 0) {
                        printf("\b \b");
                        s_cmd_index--;
                    }
                    // 履歴を1つ新しいものに
                    s_history_pos--;
                    if (s_history_pos < 0) {
                        s_cmd_index = 0;
                    } else {
                        strcpy(s_cmd_buffer, s_cmd_history[s_history_pos]);
                        s_cmd_index = strlen(s_cmd_buffer);
                        printf("%s", s_cmd_buffer);
                    }
                }
            }
        }
    } else if (c >= ' ' && c <= '~') {
        s_cmd_buffer[s_cmd_index++] = c;
        putchar(c);
    }
}