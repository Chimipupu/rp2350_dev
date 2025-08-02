/**
 * @file dbd_com_app.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief デバッグモニタのアプリ
 * @version 0.1
 * @date 2025-08-02
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */

#include "dbg_com.h"
#include "ansi_esc.h"
#include "muc_rpxxx_util.h"
#include "pcb_def.h"
#include "app_main.h"
#include "app_math.h"
#include "muc_rpxxx_util.h"

#include "drv_neopixel.h"
extern neopixel_t s_neopixel;

static void dbg_com_init_msg(dbg_cmd_args_t *p_args);

void cmd_help(dbg_cmd_args_t *p_args);
static void cmd_cls(dbg_cmd_args_t *p_args);
static void cmd_system(dbg_cmd_args_t *p_args);
static void cmd_mt_test(dbg_cmd_args_t *p_args);
static void cmd_mct_test(dbg_cmd_args_t *p_args);
static void cmd_pi_calc(dbg_cmd_args_t *p_args);
#if defined(MCU_RP2350)
static void cmd_rnd(dbg_cmd_args_t *p_args);
static void cmd_sha(dbg_cmd_args_t *p_args);
#endif
static void cmd_rst(dbg_cmd_args_t *p_args);
static void cmd_timer(dbg_cmd_args_t *p_args);
static void cmd_rtc(dbg_cmd_args_t *p_args);
static void cmd_gpio(dbg_cmd_args_t *p_args);
static void cmd_mem_dump(dbg_cmd_args_t *p_args);
static void cmd_i2c(dbg_cmd_args_t *p_args);
static void cmd_reg(dbg_cmd_args_t *p_args);
static void cmd_neopixel(dbg_cmd_args_t *p_args);
static void cmd_unknown(dbg_cmd_args_t *p_args);

static int get_neopixel_color_from_name(const char* name);
static int parse_hex_color(const char* str, uint8_t *r, uint8_t *g, uint8_t *b);

// タイマー状態
static timer_state_t s_timer_alarn_state[TIMER_MAX_ALARMS]; // タイマーアラームのステート
static uint8_t s_available_tim_cnt = TIMER_MAX_ALARMS;      // 利用可能なタイマーアラームの登録順序の数

// コマンドテーブル
const dbg_cmd_info_t g_cmd_tbl[] = {
//  | コマンド文字列 | コマンド種類 | 説明 | 最小引数数 | 最大引数数 |
    {"help",    CMD_HELP,       &cmd_help,        "Show this help message", 0, 0},
    {"cls",     CMD_CLS,        &cmd_cls,         "Display Clear", 0, 0},
    {"sys",     CMD_SYSTEM,     &cmd_system,      "Show system information", 0, 0},
    {"rst",     CMD_RST,        &cmd_rst,         "Reboot", 0, 0},
    {"memd",    CMD_MEM_DUMP,   &cmd_mem_dump,    "Memory Dump Command. args -> (#address, length)", 2, 2},
    {"reg",     CMD_REG,        &cmd_reg,         "Register read/write: reg #addr r|w bits [#val]", 3, 4},
    {"i2c",     CMD_I2C,        &cmd_i2c,         "I2C control (port, command)", 2, 2},
    {"gpio",    CMD_GPIO,       &cmd_gpio,        "Control GPIO pin (pin, value)", 2, 2},
    {"px",      CMD_NEOPIXEL,   &cmd_neopixel,    "Control NeoPixel (command, args)", 1, 2},
    {"tm",      CMD_TIMER,      &cmd_timer,       "Set timer alarm (seconds)", 0, 1},
    {"rtc",     CMD_RTC,        &cmd_rtc,         "RTC Cmd (RP2040 ... H/W RTC, RP2350 ... AON Timer)", 0, 1},
#if defined(MCU_RP2350)
    {"rnd",     CMD_RND,        &cmd_rnd,         "Generate true random numbers using TRNG", 0, 1},
    {"sha",     CMD_SHA,        &cmd_sha,         "Calc SHA-256 Hash using H/W Accelerator", 0, 1},
#endif
    {"mt",      CMD_MT_TEST,    &cmd_mt_test,     "Math test", 0, 0},
    {"mct",     CMD_MCT,        &cmd_mct_test,    "Multi Core test", 0, 0},
};

// コマンドテーブルのコマンド数(const)
const size_t g_cmd_tbl_size = sizeof(g_cmd_tbl) / sizeof(g_cmd_tbl[0]);

// タイマーコールバック関数
static int64_t timer_callback(alarm_id_t id, void *user_data)
{
    // 対応するタイマーを探して状態を更新
    for (int i = 0; i < TIMER_MAX_ALARMS; i++)
    {
        if (s_timer_alarn_state[i].alarm_id == id) {
            printf("\nTimer #%d Alarm! (Set time : %d)\n", i + 1, s_timer_alarn_state[i].req_time_sec);

            // 登録順序を再利用可能なリストに戻す
            s_timer_alarn_state[i].is_run = false;
            s_timer_alarn_state[i].alarm_id = 0;
            s_timer_alarn_state[i].order = 0;
            break;
        }
    }

    printf("> ");

    return 0;  // 繰り返しなし
}

static void dbg_com_init_msg(dbg_cmd_args_t *p_args)
{
    printf("\nDebug Command Monitor for %s Ver%d.%d.%d\n",MCU_NAME,
                                                        FW_VERSION_MAJOR,
                                                        FW_VERSION_MINOR,
                                                        FW_VERSION_REVISION);
    printf("Copyright (c) 2025 Chimipupu All Rights Reserved.\n");
    printf("Type 'help' for available commands\n");
#ifdef _WDT_ENABLE_
    printf("[INFO] Wanning! WDT Enabled: %dms\n", _WDT_OVF_TIME_MS_);
#endif // _WDT_ENABLE_
}

void cmd_help(dbg_cmd_args_t *p_args)
{
    dbg_com_init_msg(p_args);

    printf("\nAvailable %d commands:\n", g_cmd_tbl_size);
    for (uint8_t i = 0; i < g_cmd_tbl_size; i++)
    {
        printf("  %-10s - %s\n", g_cmd_tbl[i].p_cmd_str, g_cmd_tbl[i].p_description);
    }
}

static void cmd_cls(dbg_cmd_args_t *p_args)
{
    printf(ANSI_ESC_CLS);
}

static void cmd_system(dbg_cmd_args_t *p_args)
{
    uint32_t sys_clock, usb_clock, adc_clock, ref_clock;
    float cpu_temp;

    printf("\n[System Information]\n");

    // Pico SDK
    pico_sdk_version_print();

#if defined(MCU_RP2350)
    // 時刻表示
    aon_current_time_print();
#endif

    // 基板
    printf("\n[PCB Info]\nPCB Name : %s\n", PCB_NAME);

    // マイコン
    cpu_temp = get_cpu_temp_from_adc();
    printf("MCU : %s\n", MCU_NAME);
#if defined(MCU_RP2040)
    printf("CPU : Arm Cortex-M0+ (DualCore)\n");
#elif defined(MCU_RP2350)
    printf("CPU : Arm Cortex-M33 (DualCore)\n");
    rp2xxx_chip_package_print();    // チップパッケージ
#endif
    rp2xxx_chip_rev_print();        // チップRev
    printf("CPU temp = %.02f℃\n", cpu_temp);

    // レジスタ表示
    rp2xxx_reg_info();

    // ROM/RAM
    printf("\n[Mem Info]\n");
    printf("Flash Size : %d MB\n", MCU_FLASH_SIZE);
    printf("RAM Size : %d KB\n", MCU_RAM_SIZE);
#if defined(PCB_PSRAM_ENABLE)
    printf("PSRAM Size : %d MB\n", MCU_PSRAM_SIZE);
#endif // MCU_PSRAM_SIZE

    // クロック関連
    printf("\n[Clock Info]\n");
#if 0
    uint32_t clk_gp0, clk_gp1, clk_gp2, clk_gp3;
    clk_gp0 = clock_get_hz(clk_gpout0) / 1000000;
    clk_gp1 = clock_get_hz(clk_gpout1) / 1000000;
    clk_gp2 = clock_get_hz(clk_gpout2) / 1000000;
    clk_gp3 = clock_get_hz(clk_gpout3) / 1000000;
    printf("CLK_GPOUT0 : %d MHz\n", clk_gp0);
    printf("CLK_GPOUT1 : %d MHz\n", clk_gp1);
    printf("CLK_GPOUT2 : %d MHz\n", clk_gp2);
    printf("CLK_GPOUT3 : %d MHz\n", clk_gp3);
#endif
    ref_clock = clock_get_hz(clk_ref) / 1000000;
    sys_clock = clock_get_hz(clk_sys) / 1000000;
    usb_clock = clock_get_hz(clk_usb) / 1000000;
    adc_clock = clock_get_hz(clk_adc) / 1000000;
    printf("CLK_REF : %d MHz\n", ref_clock);
    printf("CLK_SYS : %d MHz\n", sys_clock);
    printf("CLK_USB : %d MHz\n", usb_clock);
    printf("CLK_ADC : %d MHz\n", adc_clock);

    // GPIO
    printf("\n[GPIO Info]\n");
#if defined(PCB_WEACT_RP2350A_V10)
    printf("GPIO %d : On Board Green LED Pin\n", PCB_LED_PIN);
    printf("GPIO %d : On Board Blue LED Pin\n", PCB_LED_2_PIN);
#else
    printf("GPIO %d : On Board LED Pin\n", PCB_LED_PIN);
#endif

#if defined(PCB_BTN_PIN)
    printf("GPIO %d : On Board Button Pin\n", PCB_BTN_PIN);
#endif

#ifdef PCB_NEOPIXEL
    printf("GPIO %d : NeoPixel Data Pin\n", PCB_NEOPIXEL_PIN);
    printf("NeoPixel Count : %d\n", NEOPIXEL_LED_CNT);
#endif // PCB_NEOPIXEL

    // I2C
    printf("\n[I2C Info]\n");
    printf("I2C 0 : %d bps, SDA/SCL (GPIO %d/%d)\n",
            I2C_BIT_RATE, I2C_0_SDA, I2C_0_SCL);
    printf("I2C 1 : %d bps, SDA/SCL (GPIO %d/%d)\n",
            I2C_BIT_RATE, I2C_1_SDA, I2C_1_SCL);

    // SPI
    printf("\n[SPI Info]\n");
    printf("SPI 0 : %d bps, CS/SCK/MISO/MOSI(GPIO %d/%d/%d/%d)\n",
            SPI_BIT_RATE, SPI_0_CS, SPI_0_SCK, SPI_0_MISO, SPI_0_MOSI);
    printf("SPI 1 : %d bps, CS/SCK/MISO/MOSI(GPIO %d/%d/%d/%d)\n",
            SPI_BIT_RATE, SPI_1_CS, SPI_1_SCK, SPI_1_MISO, SPI_1_MOSI);
    // UART
    printf("\n[UART Info]\n");
    printf("UART 0 : %d bps 8N1, TX/RX (GPIO %d%d)\n",
            UART_BAUD_RATE, UART_0_TX, UART_0_RX);
    printf("UART 1 : %d bps 8N1, TX/RX (GPIO %d%d)\n",
            UART_BAUD_RATE, UART_1_TX, UART_1_RX);
}

static void cmd_mt_test(dbg_cmd_args_t *p_args)
{
    // 数学関連テスト
    app_math_math_test();

    // 四則演算テスト(inr,float,double)
    printf("\nInteger Arithmetic Test: @%d\n", TEST_LOOP_CNT);
    proc_exec_time(int_add_test, "int_add_test");
    proc_exec_time(int_sub_test, "int_sub_test");
    proc_exec_time(int_mul_test, "int_mul_test");
    proc_exec_time(int_div_test, "int_div_test");

    printf("\nFloat Arithmetic Tests: @%d\n", TEST_LOOP_CNT);
    proc_exec_time(float_add_test, "float_add_test");
    proc_exec_time(float_sub_test, "float_sub_test");
    proc_exec_time(float_mul_test, "float_mul_test");
    proc_exec_time(float_div_test, "float_div_test");

    printf("\nDouble Arithmetic Tests: @%d\n", TEST_LOOP_CNT);
    proc_exec_time(double_add_test, "double_add_test");
    proc_exec_time(double_sub_test, "double_sub_test");
    proc_exec_time(double_mul_test, "double_mul_test");
    proc_exec_time(double_div_test, "double_div_test");
}

static void cmd_mct_test(dbg_cmd_args_t *p_args)
{
    uint32_t data = 0;

    // Core1からCore0にテストデータを投げる
    data = MULTI_CORE_TEST_DATA;
    set_multicore_fifo(data);
    printf("[Core 1] TX FIFO Data to Core 0 : 0x%08X\n", data);
}

static void cmd_pi_calc(dbg_cmd_args_t *p_args)
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
    for (uint32_t i = 1; i <= iterations; i++)
    {
        volatile uint32_t start_time = time_us_32();
        pi = app_math_pi_calc(i);
        volatile uint32_t end_time = time_us_32();
        printf("Iteration %d: π ≈ %.15f (proc time: %u us)\n", i, pi, end_time - start_time);
    }
}

#if defined(MCU_RP2350)
static void cmd_sha(dbg_cmd_args_t *p_args)
{
    uint8_t padding_buf[64 * 2];
    uint8_t hash_buf[64 * 2];
    size_t padding_len;

    if (p_args->argc < 2 || p_args->argc > 2) {
        printf("Usage: sha <data>\n");
        return;
    }

    memset(hash_buf, 0, sizeof(hash_buf));
    memset(padding_buf, 0, sizeof(padding_buf));

#if 1
    char *msg;
    msg = p_args->p_argv[1];
#else
    // (DEBUG)
    // const char msg[] = "RP2350 H/W SHA-256 TEST";    // SHA256期待値「40AEDCA712E82FD6128B6A9AFCA1350ABA27AC1259E92AE3BADE0C0C4FE892E2」
    const char msg[] = "ABC";                        // SHA256期待値「B5D4045C3F466FA91FE2CC6ABE79232A1A57CDF104F7A26E716E0A1E2789DF78」
#endif

    printf("\nSHA-256 Hash Calc(H/W)\n");
    printf("\nCalc str : %s\n", msg);
    // SHA-256のパディング処理
    sha256_padding((const uint8_t *)msg, strlen(msg), padding_buf, &padding_len);
    show_mem_dump((uint32_t)padding_buf, 64);

    // SHA-256のハッシュ値を計算
    hardware_calc_sha256((const uint8_t *)padding_buf, padding_len, hash_buf);
    printf("\nSHA-256 Hash : ");
    for (size_t i = 0; i < 32; ++i)
    {
        printf("%02X", hash_buf[i]);
    }
    printf("\n");
    show_mem_dump((uint32_t)hash_buf, 64);
}

static void cmd_rnd(dbg_cmd_args_t *p_args)
{
    int32_t i, count;

    if (p_args->argc < 2 || p_args->argc > 2) {
        printf("Usage: rnd <count>\n");
        return;
    }

    count = atoi(p_args->p_argv[1]);
    uint32_t rand_buf[count];
    memset(rand_buf, 0, sizeof(rand_buf));
    if (count <= 0) {
        printf("Error: Invalid count. Must be positive.\n");
        return;
    }

    // TRNGで真性乱数を生成
    printf("\nTRANG gen random num cnt:%d\n", count);
    trang_gen_rand_num_u32(rand_buf, count);

#if 1
    for (i = 0; i < count; i++)
    {
        printf("[DEBUG]generated rand num(%d): %u\n", i, rand_buf[i]);
    }
#endif

    printf("\n");
}
#endif

static void cmd_rst(dbg_cmd_args_t *p_args)
{
    printf("Resetting system...\n");
    watchdog_reboot(0, 0, 0);   // WDTで即時リセット
}

static void cmd_unknown(dbg_cmd_args_t *p_args)
{
    printf(ANSI_ESC_PG_RED "[ERROR] Unknown command. Type 'help' for available commands.\n" ANSI_ESC_PG_RESET);
}

/**
 * @brief タイマーコマンド関数
 * 
 * @param p_args コマンド引数の構造体ポインタ
 */
static void cmd_timer(dbg_cmd_args_t *p_args)
{
    if (p_args->argc > 1) {
        int32_t seconds = atoi(p_args->p_argv[1]);
        if (seconds <= 0) {
            printf("Error: Invalid timer set_time_us. Must be positive.\n");
            return;
        }
        if (seconds > TIMER_MAX_SECONDS) {
            printf("Error: Timer set_time_us exceeds maximum of %d seconds.\n", TIMER_MAX_SECONDS);
            return;
        }

        // 空いているタイマースロットを探す
        int free_slot = -1;
        for (int i = 0; i < TIMER_MAX_ALARMS; i++)
        {
            if (!s_timer_alarn_state[i].is_run) {
                free_slot = i;
                break;
            }
        }

        if (free_slot == -1) {
            printf("Error: All %d hardware timers are in use.\n", TIMER_MAX_ALARMS);
            return;
        }

        // H/Wでアラームを設定（us単位）
        s_timer_alarn_state[free_slot].req_time_sec = seconds;
        uint64_t delay_us = seconds * 1000000ULL;
        alarm_id_t alarm_id = add_alarm_in_us(delay_us, timer_callback, NULL, true);
        if (alarm_id > 0) {
            s_timer_alarn_state[free_slot].is_run = true;
            s_timer_alarn_state[free_slot].start_time = time_us_32();
            s_timer_alarn_state[free_slot].set_time_us = delay_us;
            s_timer_alarn_state[free_slot].alarm_id = alarm_id;
            s_timer_alarn_state[free_slot].order = 0;
            s_available_tim_cnt--;
            printf("Timer #%d Alarm Set %d s\n",free_slot + 1, seconds);
        } else {
            printf("Error: Failed to set timer.\n");
        }
    } else {
        // 引数なしの場合は現在のタイマー状態を表示
        bool any_running = false;
        for (int i = 0; i < TIMER_MAX_ALARMS; i++) {
            if (s_timer_alarn_state[i].is_run) {
                uint32_t elapsed = time_us_32() - s_timer_alarn_state[i].start_time;
                uint32_t remaining = (elapsed < s_timer_alarn_state[i].set_time_us) ?
                                    (s_timer_alarn_state[i].set_time_us - elapsed) : 0;
                printf("Timer alarm #%d = %u s remaining.\n",
                        i + 1,
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
 * @brief RTCコマンド（リソース ... RP2040 = H/W RTC, RP2350 = AON Timer）
 * 
 * @param p_args コマンド引数の構造体ポインタ
 */
static void cmd_rtc(dbg_cmd_args_t *p_args)
{
#if defined(MCU_RP2350)
    // 引数が2つ以上
    if (p_args->argc > 1) {
        // "g"なら現在時刻表示
        if (strcmp(p_args->p_argv[1], "g") == 0) {
            aon_current_time_print();
        }
        // "s"なら時刻設定
        else if (strcmp(p_args->p_argv[1], "s") == 0) {
            if (p_args->argc < 4) {
                printf("Usage: rtc s YYYY/MM/DD HH:MM:SS\n");
                printf("Example: rtc s 2025/08/02 15:30:00\n");
                return;
            }

            // 日付と時刻を結合して一つの文字列にする
            char time_str[32];
            snprintf(time_str, sizeof(time_str), "%s %s", p_args->p_argv[2], p_args->p_argv[3]);

            if (aon_set_time_from_string(time_str)) {
                printf("AON Timer set to: %s\n", time_str);
            } else {
                printf("Failed to set AON Timer (arg = %s). Format: YYYY/MM/DD HH:MM:SS\n", time_str);
            }
        } else {
            printf("Usage: rtc g | rtc s YYYY/MM/DD HH:MM:SS\n");
            printf("  g - Get current time\n");
            printf("  s - Set time (format: YYYY/MM/DD HH:MM:SS)\n");
            printf("Example: rtc s 2025/08/02 15:30:00\n");
        }
    } else {
        // 引数なし = 現在時刻表示
        aon_current_time_print();
    }
#endif // MCU_RP2350
}

/**
 * @brief GPIO制御コマンド関数
 * 
 * @param p_args コマンド引数の構造体ポインタ
 */
static void cmd_gpio(dbg_cmd_args_t *p_args)
{
    if (p_args->argc != 3) {
        printf("Error: Invalid number of arguments. Usage: gpio <pin> <value>\n\n");
        return;
    }

    // コマンドからGPIOのピン番号とピンの値を取得
    uint8_t pin = atoi(p_args->p_argv[1]);
    uint8_t value = atoi(p_args->p_argv[2]);

    // GPIOのピン番号チェック
    if (pin < 0 || pin > GPIO_MAX_PIN_NUM) {
        printf("Error: Invalid GPIO pin number. Must be between 0 and %d.\n\n", GPIO_MAX_PIN_NUM);
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
 * @brief メモリダンプコマンド関数
 * 
 * @param p_args コマンド引数の構造体ポインタ
 */
static void cmd_mem_dump(dbg_cmd_args_t *p_args)
{
    uint32_t addr;
    uint32_t length;

    if (p_args->argc != 3) {
        printf("Error: Invalid number of arguments. Usage: mem_dump <address> <length>\n");
        return;
    }

    // アドレスを16進数文字列から数値に変換
    if (sscanf(p_args->p_argv[1], "#%x", &addr) != 1) {
        printf("Error: Invalid address format. Use hexadecimal with # prefix (e.g., #F0000000)\n");
        return;
    }

    // 長さを16進数文字列から数値に変換
    if (sscanf(p_args->p_argv[2], "#%x", &length) != 1) {
        printf("Error: Invalid length format. Use hexadecimal with # prefix (e.g., #10)\n");
        return;
    }

    volatile uint32_t start_time = time_us_32();
    show_mem_dump(addr, length);
    volatile uint32_t end_time = time_us_32();
    printf("\nMemory dump completed (proc time: %u us)\n", end_time - start_time);
}

/**
 * @brief I2Cスキャンコマンド関数
 * 
 * @param p_args コマンド引数の構造体ポインタ
 */
static void cmd_i2c(dbg_cmd_args_t *p_args)
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
static void cmd_reg(dbg_cmd_args_t *p_args)
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

static int get_neopixel_color_from_name(const char* name)
{
    if (strcasecmp(name, "black") == 0) return NEOPIXEL_COLOR_BLACK;
    if (strcasecmp(name, "red") == 0) return NEOPIXEL_COLOR_RED;
    if (strcasecmp(name, "green") == 0) return NEOPIXEL_COLOR_GREEN;
    if (strcasecmp(name, "blue") == 0) return NEOPIXEL_COLOR_BLUE;
    if (strcasecmp(name, "yellow") == 0) return NEOPIXEL_COLOR_YELLOW;
    if (strcasecmp(name, "cyan") == 0) return NEOPIXEL_COLOR_CYAN;
    if (strcasecmp(name, "magenta") == 0) return NEOPIXEL_COLOR_MAGENTA;
    if (strcasecmp(name, "orange") == 0) return NEOPIXEL_COLOR_ORANGE;
    if (strcasecmp(name, "purple") == 0) return NEOPIXEL_COLOR_PURPLE;
    if (strcasecmp(name, "pink") == 0) return NEOPIXEL_COLOR_PINK;
    if (strcasecmp(name, "white") == 0) return NEOPIXEL_COLOR_WHITE;
    return -1;
}

static int parse_hex_color(const char* str, uint8_t *r, uint8_t *g, uint8_t *b)
{
    if ((str == NULL) || (strlen(str) != 7) || (str[0] != '#')) {
        return -1;
    }

    uint8_t rv, gv, bv = 0;
    if (sscanf(str+1, "%02x%02x%02x", &rv, &gv, &bv) == 3) {
        *r = (uint8_t)rv;
        *g = (uint8_t)gv;
        *b = (uint8_t)bv;
        return 0;
    }
    return -1;
}

/**
 * @brief NeoPixel制御コマンド関数
 * 
 * @param p_args 
 */
static void cmd_neopixel(dbg_cmd_args_t *p_args)
{
    uint8_t r, g, b = 0;
    int idx = 0;
    uint8_t all_set_flag = 0;
    int color_enum = 0;

    char* p_mode_str = p_args->p_argv[1];
    // 第二引数がcls
    if(strcasecmp(p_mode_str, "cls") == 0) {
        drv_neopixel_init(&s_neopixel);
        printf("All NeoPixel Cleared!\n");
        return;
    }

    if(strcasecmp(p_mode_str, "fade") == 0) {
        // Core 0にLEDフェードを実行要求
        set_multicore_fifo(PROC_NEOPIXEL_FADE);
        printf("All NeoPixel Color Fade! at Core 0\n");
        return;
    }

    // コマンド引数チェック
    if (p_args->argc < 3) {
        printf("Usage: neopixel <index|mode> <color|#RRGGBB>\n");
        return;
    }

    // 第二引数がall
    if(strcasecmp(p_mode_str, "all") == 0) {
        all_set_flag = 1;
    } else {
        idx = atoi(p_args->p_argv[1]);
        // 指定indexチェック
        if ((idx < 1) || idx > s_neopixel.led_cnt) {
            printf("Error: indexは1～%d\n", s_neopixel.led_cnt);
            return;
        }
    }

    // 第三引数が文字
    uint8_t led_idx = (uint8_t)idx - 1;
    const char* color_str = p_args->p_argv[2];
    color_enum = get_neopixel_color_from_name(color_str);
    if (color_enum >= 0) {
        if (all_set_flag != 0) {
            // 全てのNeoPixelに同じ色を設定
            for (uint8_t i = 0; i < s_neopixel.led_cnt; i++)
            {
                drv_neopixel_get_pixel_color(&s_neopixel, i, color_enum);
            }
            drv_neopixel_show(&s_neopixel);
            printf("All NeoPixels set to %s\n", color_str);
            return;
        } else {
            // 指定されたNeoPixelに色を設定
            drv_neopixel_set_pixel_color(&s_neopixel, led_idx, color_enum);
            printf("NeoPixel[%d] = %s\n", led_idx, color_str);
            return;
        }
    }

    // 第三引数が#RRGGBB形式
    if (parse_hex_color(color_str, &r, &g, &b) == 0) {
        if (all_set_flag != 0) {
            // 全てのNeoPixelに同じ色を設定
            drv_neopixel_set_all_led_color(&s_neopixel, r, g, b);
            printf("All NeoPixels set to #%02X%02X%02X\n", r, g, b);
            return;
        } else {
            drv_neopixel_set_pixel_rgb(&s_neopixel, led_idx, r, g, b);
            printf("NeoPixel[%d] = #%02X%02X%02X\n", led_idx, r, g, b);
            return;
        }
    }
}