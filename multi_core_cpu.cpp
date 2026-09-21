/**
 * @file multi_core_cpu.cpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief マルチコアCPU
 * @note 詳細設計書:「RP2350評価FW_詳細設計_初版」のシート「CPU_FIFOデータフォーマット」
 * @version 0.1
 * @date 2026-09-06
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */
#include "multi_core_cpu.h"
#include "pcb_def.h"
#include "app_main.h"
#include <Arduino.h>

#if defined(RGBLED_PIN)
#include "app_neopixel.h"
#endif

// ---------------------------------------------------
static cpu_fifo_t s_cpu_core_0_fifo;
static cpu_fifo_t s_cpu_core_1_fifo;

static E_CPU_FIFO_DATA_TYPE _get_cpu_fifo_data_type(uint32_t fifo_data);
static uint32_t _get_cpu_fifo_data(uint32_t fifo_data);
static void _cpu_fifo_proc_cpu_core_0(void);
static void _cpu_fifo_proc_cpu_core_1(void);

// ---------------------------------------------------
// [Static関数]

static E_CPU_FIFO_DATA_TYPE _get_cpu_fifo_data_type(uint32_t fifo_data)
{
    return (E_CPU_FIFO_DATA_TYPE) ((fifo_data & CPU_FIFO_DATA_TYPE_BIT) >> 24);
}

static uint32_t _get_cpu_fifo_data(uint32_t fifo_data)
{
    return (uint32_t)(fifo_data & CPU_FIFO_DATA_BIT);
}

static void _cpu_fifo_proc_cpu_core_0(void)
{
    bool is_ret_rx_fifo;
    bool is_ret_tx_fifo;
    uint32_t data_type = 0;
    uint32_t local_fifo_data = 0;
    uint32_t tmp_u32 = 0;
#if defined(RGBLED_PIN)
    led_color_t rgb_val;
#endif

    is_ret_rx_fifo = cpu_fifo_rx_data(&s_cpu_core_0_fifo.rx.fifo[s_cpu_core_0_fifo.rx.idx]);

    if(is_ret_rx_fifo != false) {
        local_fifo_data = s_cpu_core_0_fifo.rx.fifo[s_cpu_core_0_fifo.rx.idx];
        Serial.printf("[DEBUG] CPU Core 0, RX FIFO[%d]: 0x%08X\n",
                        s_cpu_core_0_fifo.rx.idx,
                        local_fifo_data
                    );
        s_cpu_core_0_fifo.rx.idx = (s_cpu_core_0_fifo.rx.idx + 1) % CPU_FIFO_BUF_SIZE;
        data_type = _get_cpu_fifo_data_type(local_fifo_data);

        switch (data_type)
        {
            case CPU_FIFO_DATA_TYPE_RGBLED:
                tmp_u32 = _get_cpu_fifo_data(local_fifo_data);
#if defined(RGBLED_PIN)
                rgb_val.rgb = tmp_u32;
                app_neopixel_set_rgb(0, &rgb_val);
#endif
                tmp_u32 = 0;
                tmp_u32 = (CPU_FIFO_DATA_TYPE_RESULT << 24) | CPU_FIFO_RESULT_OK;
                set_cpu_fifo_tx_data(tmp_u32);
                break;

            default:
                break;
        }
    }

    if(s_cpu_core_0_fifo.tx.cnt > 0) {
        is_ret_tx_fifo = cpu_fifo_tx_data(s_cpu_core_0_fifo.tx.fifo[s_cpu_core_0_fifo.tx.idx]);
        if(is_ret_tx_fifo != false) {
            s_cpu_core_0_fifo.tx.cnt--;
            Serial.printf("[DEBUG] CPU Core 0, TX FIFO[%d]: 0x%08X\n",
                            s_cpu_core_0_fifo.tx.idx,
                            s_cpu_core_0_fifo.tx.fifo[s_cpu_core_0_fifo.tx.idx]
                        );
            s_cpu_core_0_fifo.tx.idx = (s_cpu_core_0_fifo.tx.idx + 1) % CPU_FIFO_BUF_SIZE;
        }
    }
}

static void _cpu_fifo_proc_cpu_core_1(void)
{
    bool is_ret_tx_fifo = false;
    bool is_ret_rx_fifo = false;
    uint32_t data_type = 0;
    uint32_t local_fifo_data = 0;

    is_ret_rx_fifo = cpu_fifo_rx_data(&s_cpu_core_1_fifo.rx.fifo[s_cpu_core_1_fifo.tx.idx]);

    if(is_ret_rx_fifo != false) {
        local_fifo_data = s_cpu_core_1_fifo.rx.fifo[s_cpu_core_1_fifo.tx.idx];
        Serial.printf("[DEBUG] CPU Core 1, RX FIFO[%d]: 0x%08X\n",
                        s_cpu_core_0_fifo.rx.idx,
                        local_fifo_data
                    );
        s_cpu_core_0_fifo.rx.idx = (s_cpu_core_0_fifo.rx.idx + 1) % CPU_FIFO_BUF_SIZE;
        data_type = _get_cpu_fifo_data_type(local_fifo_data);

        switch (data_type)
        {
            case CPU_FIFO_DATA_TYPE_CONFIG:
                // TODO
                break;

            case CPU_FIFO_DATA_TYPE_REQUEST:
                // TODO
                break;

            case CPU_FIFO_DATA_TYPE_RESULT:
                // TODO
                break;

            case CPU_FIFO_DATA_TYPE_MSG_ASCII:
                // TODO
                break;

            case CPU_FIFO_DATA_TYPE_DEBUG:
                // TODO
                break;

            default:
                break;
        }
    }

    if(s_cpu_core_1_fifo.tx.cnt > 0) {
        is_ret_tx_fifo = cpu_fifo_tx_data(s_cpu_core_1_fifo.tx.fifo[s_cpu_core_1_fifo.tx.idx]);
        if(is_ret_tx_fifo != false) {
            s_cpu_core_1_fifo.tx.cnt--;
            Serial.printf("[DEBUG] CPU Core 1, TX FIFO[%d]: 0x%08X\n",
                            s_cpu_core_1_fifo.tx.idx,
                            s_cpu_core_1_fifo.tx.fifo[s_cpu_core_1_fifo.tx.idx]);
            s_cpu_core_1_fifo.tx.idx = (s_cpu_core_1_fifo.tx.idx + 1) % CPU_FIFO_BUF_SIZE;
        }
    }
}

void set_cpu_fifo_tx_data(uint32_t data)
{
    uint8_t cpu_core;
    cpu_fifo_t *p_cpu_fifo;

    cpu_core = get_core_num();
    if(cpu_core == 0) {
        p_cpu_fifo = &s_cpu_core_0_fifo;
    } else {
        p_cpu_fifo = &s_cpu_core_1_fifo;
    }

    p_cpu_fifo->tx.idx = p_cpu_fifo->tx.cnt % CPU_FIFO_BUF_SIZE;
    p_cpu_fifo->tx.fifo[p_cpu_fifo->tx.idx] = data;
    p_cpu_fifo->tx.cnt++;
}

void dump_cpu_fifo_buf(void)
{
    uint8_t i;
    uint32_t fifo_val;

# if 1
    // CPU Core 0
    for(i = 0; i < CPU_FIFO_BUF_SIZE; i++)
    {
        fifo_val = s_cpu_core_0_fifo.rx.fifo[i];
        Serial.printf("[CPU Core 0] CPU RX FIFO Buf[%d]: 0x%08X\n", i, fifo_val);
    }
    Serial.printf("\n");
    for(i = 0; i < CPU_FIFO_BUF_SIZE; i++)
    {
        fifo_val = s_cpu_core_0_fifo.tx.fifo[i];
        Serial.printf("[CPU Core 0] CPU TX FIFO Buf[%d]: 0x%08X\n", i, fifo_val);
    }
#endif

    Serial.printf("\n");

#if 1
    // CPU Core 1
    for(i = 0; i < CPU_FIFO_BUF_SIZE; i++)
    {
        fifo_val = s_cpu_core_1_fifo.rx.fifo[i];
        Serial.printf("[CPU Core 1] CPU RX FIFO Buf[%d]: 0x%08X\n", i, fifo_val);
    }
    Serial.printf("\n");
    for(i = 0; i < CPU_FIFO_BUF_SIZE; i++)
    {
        fifo_val = s_cpu_core_1_fifo.tx.fifo[i];
        Serial.printf("[CPU Core 1] CPU TX FIFO Buf[%d]: 0x%08X\n", i, fifo_val);
    }
#endif
}
// ---------------------------------------------------
// [API]

/**
 * @brief CPU FIFOにデータを送信
 * @param data 送信データ(uint32_t)
 */
bool cpu_fifo_tx_data(uint32_t data)
{
    bool ret = false;
    ret = rp2040.fifo.push_nb(data);
    return ret;
}

/**
 * @brief CPU FIFOからデータを受信
 */
bool cpu_fifo_rx_data(uint32_t *p_data)
{
    bool ret = false;
    uint8_t fifo_data_num;

    fifo_data_num = rp2040.fifo.available();

    if (fifo_data_num > 0) {
        ret = rp2040.fifo.pop_nb(p_data);
    }

    return ret;
}

// ---------------------------------------------------
// ********** [CPU Core 0] ***********
/**
 * @brief CPU Core 0の初期化
 */
void cpu_core_0_init(void)
{
    memset(&s_cpu_core_0_fifo, 0x00, sizeof(s_cpu_core_0_fifo));

    pcb_gpio_init(); // GPIO初期化

    pcb_i2c_init(); // I2C初期化

#if defined(RGBLED_PIN)
    // RGBLED 初期化
    app_neopixel_init(RGBLED_PIN, RGBLED_NUM, RGBLED_MAX_BRIGHTNESS);
#endif

    pcb_uart_init(); // UART初期化

    app_main_core_0_init(); // アプリ初期化
}

/**
 * @brief CPU Core 0 メイン
 */
void cpu_core_0_main(void)
{
    _cpu_fifo_proc_cpu_core_0(); // CPU FIFO処理

#ifdef BUTTON_PIN
    btn_polling();
#endif

    app_main_core_0();           // CPU Core 0用アプリ
}

// ---------------------------------------------------
// ********** [CPU Core 1] ***********
/**
 * @brief CPU Core 1の初期化
 */
void cpu_core_1_init(void)
{
    memset(&s_cpu_core_1_fifo, 0x00, sizeof(s_cpu_core_1_fifo));

    // RGBLED 初期化
#if defined(RGBLED_PIN)
    app_neopixel_init(RGBLED_PIN, RGBLED_NUM, RGBLED_MAX_BRIGHTNESS);

    for(uint8_t i = 0; i < CPU_FIFO_BUF_SIZE; i++)
    {
        s_cpu_core_1_fifo.tx_buf[i] = (g_led_color_tbl[i].rgb.rgb | (CPU_FIFO_DATA_TYPE_RGBLED << 24));
    }
#endif

    app_main_core_1_init(); // アプリ初期化
}

/**
 * @brief CPU Core 1 メイン
 */
void cpu_core_1_main(void)
{
    _cpu_fifo_proc_cpu_core_1(); // CPU FIFO処理
    app_main_core_1();           // CPU Core 1用アプリ
}

// ---------------------------------------------------