#include "dbg_com.h"
#include "app_main.h"

// コマンド履歴
static char s_cmd_history[CMD_HISTORY_MAX][DBG_CMD_MAX_LEN];
static uint8_t s_history_count = 0;  // コマンド履歴の数
static int8_t s_history_pos = -1;    // 現在の履歴位置（-1は最新）

static void sort_available_orders(void);
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
static void cmd_mem_dump(const dbg_cmd_args_t* p_args);
static void cmd_i2c(const dbg_cmd_args_t* p_args);
static void cmd_reg(const dbg_cmd_args_t* p_args);

// コマンドテーブル
static const dbg_cmd_info_t s_cmd_table[] = {
    {"help",    CMD_HELP,    "Show this help message", 0, 0},
    {"ver",     CMD_VER,     "Show version information", 0, 0},
    {"clock",   CMD_CLOCK,   "Show clock information", 0, 0},
    {"rst",     CMD_RST,     "Reboot", 0, 0},
    {"mem_dump", CMD_MEM_DUMP, "Dump memory contents (address, length)", 2, 2},
    {"reg",     CMD_REG,     "Register read/write: reg #addr r|w bits [#val]", 3, 4},
    {"i2c",     CMD_I2C,     "I2C control (port, command)", 2, 2},
    {"gpio",    CMD_GPIO,    "Control GPIO pin (pin, value)", 2, 2},
    {"timer",   CMD_TIMER,   "Set timer alarm (seconds)", 0, 1},
    {"at",      CMD_AT_TEST, "int/float/double arithmetic test", 0, 0},
    {"pi",      CMD_PI_CALC, "Calculate pi using Gauss-Legendre", 0, 1},
    {"trig",    CMD_TRIG,    "Run sin,cos,tan functions test", 0, 0},
    {"atan2",   CMD_ATAN2,   "Run atan2 test", 0, 0},
    {"tan355",  CMD_TAN355,  "Run tan(355/226) test", 0, 0},
    {"isqrt",   CMD_ISQRT,   "Run 1/sqrt(x) test", 0, 0},
    {NULL,      CMD_UNKNOWN, NULL, 0, 0}
};

// コマンドバッファ
static char s_cmd_buffer[DBG_CMD_MAX_LEN];
static int32_t s_cmd_index = 0;

// タイマー状態
static timer_state_t s_timer_state[TIMER_MAX_ALARMS] = {0};
static uint8_t s_available_orders[TIMER_MAX_ALARMS] = {1, 2, 3, 4};  // 利用可能な登録順序
static uint8_t s_available_count = TIMER_MAX_ALARMS;  // 利用可能な登録順序の数

static int32_t split_str(char* p_str, dbg_cmd_args_t* p_args);

// コマンド引数を分割して解析
static int32_t split_str(char* p_str, dbg_cmd_args_t* p_args)
{
    char* p_token;
    char* p_next = p_str;
    char* p_end = p_str + strlen(p_str);

    p_args->argc = 0;
#ifdef DEBUG_DBG_COM
    printf("[DEBUG] : Input string = '%s'\n", p_str);  // 入力文字列の確認
#endif // DEBUG_DBG_COM

    while (p_next < p_end && p_args->argc < DBG_CMD_MAX_ARGS)
    {
        // スペースをスキップ
        while (*p_next == ' ' && p_next < p_end)
        {
            p_next++;
        }
        if (p_next >= p_end) break;

        // トークンの開始位置を記録
        p_token = p_next;

        // 次のスペースまたは文字列末尾まで移動
        while (*p_next != ' ' && p_next < p_end)
        {
            p_next++;
        }

        // 文字列を終端
        if (*p_next == ' ') {
            *p_next++ = '\0';
        }

#ifdef DEBUG_DBG_COM
        printf("[DEBUG] : Next token = '%s'\n", p_token);
#endif // DEBUG_DBG_COM
        p_args->p_argv[p_args->argc++] = p_token;
        WDT_RST();
    }

#ifdef DEBUG_DBG_COM
    printf("[DEBUG] : argc = %d\n", p_args->argc);
    for (int i = 0; i < p_args->argc; i++)
    {
        printf("[DEBUG] : argv[%d] = %s\n", i, p_args->p_argv[i]);
    }
#endif // DEBUG_DBG_COM

    return p_args->argc;
}

// タイマーコールバック関数
static int64_t timer_callback(alarm_id_t id, void *user_data)
{
    // 対応するタイマーを探して状態を更新
    for (int i = 0; i < TIMER_MAX_ALARMS; i++)
    {
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

// 利用可能な登録順序を昇順にソート
static void sort_available_orders(void)
{
    for (int i = 0; i < s_available_count - 1; i++)
    {
        for (int j = 0; j < s_available_count - i - 1; j++)
        {
            if (s_available_orders[j] > s_available_orders[j + 1]) {
                uint8_t temp = s_available_orders[j];
                s_available_orders[j] = s_available_orders[j + 1];
                s_available_orders[j + 1] = temp;
            }
        }
    }
}

static void dbg_com_init_msg(void)
{
    printf("\nDebug Command Monitor for RP2350 Ver 0.1\n");
    printf("Copyright (c) 2025 Chimipupu(https://github.com/Chimipupu)\n");
    printf("Type 'help' for available commands\n");
#ifdef _WDT_ENABLE_
    printf("[INFO] Wanning! WDT Enabled: %dms\n", _WDT_OVF_TIME_MS_);
#endif // _WDT_ENABLE_
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

        case CMD_MEM_DUMP:
            cmd_mem_dump(p_args);
            break;

        case CMD_I2C:
            cmd_i2c(p_args);
            break;

        case CMD_RST:
            cmd_rst();
            break;

        case CMD_REG:
            cmd_reg(p_args);
            break;

        case CMD_UNKNOWN:
            cmd_unknown();
            break;
    }
}

/**
 * @brief メモリダンプコマンド関数
 * 
 * @param p_args コマンド引数の構造体ポインタ
 */
static void cmd_mem_dump(const dbg_cmd_args_t* p_args)
{
    if (p_args->argc != 3) {
        printf("Error: Invalid number of arguments. Usage: mem_dump <address> <length>\n");
        return;
    }

    // アドレスを16進数文字列から数値に変換
    uint32_t addr;
    if (sscanf(p_args->p_argv[1], "#%x", &addr) != 1) {
        printf("Error: Invalid address format. Use hexadecimal with # prefix (e.g., #F0000000)\n");
        return;
    }

    // 長さを16進数文字列から数値に変換
    uint32_t length;
    if (sscanf(p_args->p_argv[2], "#%x", &length) != 1) {
        printf("Error: Invalid length format. Use hexadecimal with # prefix (e.g., #10)\n");
        return;
    }

    // メモリダンプの処理時間を計測開始
    volatile uint32_t start_time = time_us_32();

    // ヘッダー行を表示
    printf("Address  ");
    for (int i = 0; i < 16; i++) {
        printf("%02X ", i);
    }
    printf("| ASCII\n");
    printf("-------- ");
    for (int i = 0; i < 16; i++) {
        printf("---");
    }
    printf("| ------\n");

    // 16バイトずつダンプ
    for (uint32_t offset = 0; offset < length; offset += 16) {
        // アドレス表示
        printf("%08X: ", addr + offset);

        // 16バイト分のデータを表示
        for (int i = 0; i < 16; i++) {
            if (offset + i < length) {
                uint8_t data = *((volatile uint8_t*)(addr + offset + i));
                printf("%02X ", data);
            } else {
                printf("   ");  // データがない場合は空白を表示
            }
        }

        // ASCII表示
        printf("| ");
        for (int i = 0; i < 16; i++) {
            if (offset + i < length) {
                uint8_t data = *((volatile uint8_t*)(addr + offset + i));
                // 表示可能なASCII文字のみ表示
                printf("%c", (data >= 32 && data <= 126) ? data : '.');
            } else {
                printf(" ");  // データがない場合は空白を表示
            }
        }
        printf("\n");
    }

    // 処理時間を計測終了
    volatile uint32_t end_time = time_us_32();
    printf("\nMemory dump completed (proc time: %u us)\n", end_time - start_time);
}

/**
 * @brief I2Cスキャンコマンド関数
 * 
 * @param p_args コマンド引数の構造体ポインタ
 */
static void cmd_i2c(const dbg_cmd_args_t* p_args)
{
    if (p_args->argc != 3) {
        printf("Error: Invalid number of arguments. Usage: i2c <port> <command>\n");
        printf("Commands:\n");
        printf("  s - Scan I2C bus for devices\n");
        return;
    }

    // I2Cポート番号を取得
    uint8_t port = atoi(p_args->p_argv[1]);
    if (port != 0 && port != 1) {
        printf("Error: Only I2C ports 0 and 1 are supported.\n");
        return;
    }

    const char* cmd = p_args->p_argv[2];

    // I2Cのポートスキャン
    if (strcmp(cmd, "s") == 0) {
        i2c_slave_scan(port);
    } else {
        printf("Error: Unknown I2C command '%s'\n", cmd);
    }
}

/**
 * @brief レジスタR/Wコマンド関数
 * 
 * @param p_args コマンド引数の構造体ポインタ
 */
static void cmd_reg(const dbg_cmd_args_t* p_args)
{
    uint32_t wval = 0;
    uint32_t val = 0;
    uint32_t addr = 0;
    uint32_t rval = 0;

    if (p_args->argc != 4 && p_args->argc != 5) {
        printf("Error: Usage: reg #ADDR r|w BITS [#VAL]\n");
        printf("  e.g. reg #F000FF00 r 8\n");
        printf("  e.g. reg #F000FF00 w 32 #FFDC008F\n");
        return;
    }

    if (sscanf(p_args->p_argv[1], "#%x", &addr) != 1) {
        printf("Error: Invalid address format. Use #HEX (e.g. #F000FF00)\n");
        return;
    }
    char rw = p_args->p_argv[2][0];
    int bits = atoi(p_args->p_argv[3]);
    if (!(bits == 8 || bits == 16 || bits == 32)) {
        printf("Error: Bit width must be 8, 16, or 32\n");
        return;
    }
    if (rw == 'r') { // 読み取り
        if (p_args->argc != 4) {
            printf("Error: Read usage: reg #ADDR r BITS\n");
            return;
        }
        // app_main.hのマクロを使用
        if (bits == 8) val = REG_READ_BYTE(0, addr);
        else if (bits == 16) val = REG_READ_WORD(0, addr);
        else if (bits == 32) val = REG_READ_DWORD(0, addr);
        printf("[REG] Read %dbit @ 0x%08X = 0x%0*X\n", bits, addr, bits / 4, val);
    } else if (rw == 'w') { // 書き込み
        sscanf(p_args->p_argv[4], "#%x", &wval);
        if (bits == 8) {
            REG_WRITE_BYTE(0, addr, (uint8_t)wval);
        } else if (bits == 16) {
            REG_WRITE_WORD(0, addr, (uint16_t)wval);
        } else if (bits == 32) {
            REG_WRITE_DWORD(0, addr, (uint32_t)wval);
        }
        printf("[REG] Write %dbit @ 0x%08X = 0x%0*X\n", bits, addr, bits / 4, wval);
        } else {
        printf("Error: 2nd arg must be 'r' or 'w'\n");
    }
}

/**
 * @brief デバッグコマンドモニターのメイン処理
 */
void dbg_com_process(void)
{
    dbg_cmd_args_t args;

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

            split_str(s_cmd_buffer, &args);
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
                        WDT_RST();
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
                        WDT_RST();
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