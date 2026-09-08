/**
 * @file app_main.cpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief アプリメイン
 * @version 0.1
 * @date 2026-09-06
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */
#include "app_main.h"
#include "pcb_def.h"
#include "multi_core_cpu.h"
#include "dbg_cmd.h"

#if defined(RGBLED_PIN)
#include "app_neopixel.h"
#endif

#if defined(I2S_USE)
#include "i2s_sound.h"
#endif

// C Lib
#include <stdio.h>

// Arduino IDE Lib
#include <Arduino.h>

// ---------------------------------------------------
#if defined(RGBLED_PIN)
typedef struct {
    char color_num;
    led_color_t *p_color;
    const char *p_color_str;
} rgbled_args_t;

const rgbled_args_t g_rgbled_color_args_tbl[] = {
    { '0', (led_color_t *) &g_led_color_tbl[0].rgb, g_led_color_tbl[0].p_color_str },
    { '1', (led_color_t *) &g_led_color_tbl[1].rgb, g_led_color_tbl[1].p_color_str },
    { '2', (led_color_t *) &g_led_color_tbl[2].rgb, g_led_color_tbl[2].p_color_str },
    { '3', (led_color_t *) &g_led_color_tbl[3].rgb, g_led_color_tbl[3].p_color_str },
    { '4', (led_color_t *) &g_led_color_tbl[4].rgb, g_led_color_tbl[4].p_color_str },
    { '5', (led_color_t *) &g_led_color_tbl[5].rgb, g_led_color_tbl[5].p_color_str },
    { '6', (led_color_t *) &g_led_color_tbl[6].rgb, g_led_color_tbl[6].p_color_str },
    { '7', (led_color_t *) &g_led_color_tbl[7].rgb, g_led_color_tbl[7].p_color_str },
};
const uint8_t RGBLED_COLOR_ARGS_TBL_CNT = sizeof(g_rgbled_color_args_tbl) / sizeof(g_rgbled_color_args_tbl[0]);

static E_DBG_CMD_RESULT _cmd_rgbled(void *p_args);
#endif

static E_DBG_CMD_RESULT _cmd_debug(void *p_args);
static E_DBG_CMD_RESULT _cmd_cpu_fifo(void *p_args);

static const dbg_cmd_tbl_t s_ext_cmd_tbl[] = {
    {"debug",   "dbg", _cmd_debug},
    {"cpufifo", "cff", _cmd_cpu_fifo},
#if defined(RGBLED_PIN)
    {"rgbled",  "rl",  _cmd_rgbled},
#endif
};
static uint8_t _serial_read_func(void);

const dbg_cmd_config_t g_dbg_cmd_config = {
    .p_serial_read = _serial_read_func,
    .p_printf = DBG_PRINTF,
    .p_ext_cmd_tbl = (dbg_cmd_tbl_t *)s_ext_cmd_tbl,
    .ext_cmd_num = sizeof(s_ext_cmd_tbl) / sizeof(s_ext_cmd_tbl[0]),
};

// ---------------------------------------------------
// [Static関数]
static uint8_t _serial_read_func(void)
{
    return (uint8_t)Serial.read();
}

static E_DBG_CMD_RESULT _cmd_debug(void *p_args)
{
    static uint8_t s_led_val = 0xFF;
    dbg_cmd_args_t *p_cmd_args;

    DBG_PRINTF("-------------------------------\n");
    DBG_PRINTF("Debug Cmd\n");

    p_cmd_args = (dbg_cmd_args_t *)p_args;

    // 引数チェック
    if(strcmp(p_cmd_args->argv[0], "info") == 0) {
        DBG_PRINTF("PCB Info Print\n");
        pcb_info();
    }
    else if(strcmp(p_cmd_args->argv[0], "led") == 0)
    {
        digitalWrite(OB_LED_PIN, s_led_val);
        s_led_val = ~s_led_val;

        if(s_led_val > 0) {
            DBG_PRINTF("LED OFF\n");
        } else {
            DBG_PRINTF("LED ON\n");
        }
    } else if(strcmp(p_cmd_args->argv[0], "i2s") == 0)
    {
        DBG_PRINTF("I2S Test\n");

        // ドレミファソラシド
        i2s_play_melody(g_play_test_song, g_play_test_song_note_cnt);
    }

    DBG_PRINTF("-------------------------------\n");

    return CMD_RESULT_EXEC_OK;
}

static E_DBG_CMD_RESULT _cmd_cpu_fifo(void *p_args)
{
    uint32_t tmp_u32;
    dbg_cmd_args_t *p_cmd_args;

    DBG_PRINTF("-------------------------------\n");
    DBG_PRINTF("CPU FIFO Debug Cmd\n");

    p_cmd_args = (dbg_cmd_args_t *)p_args;

    // 引数チェック
    if(strcmp(p_cmd_args->argv[0], "dump") == 0)
    {
        DBG_PRINTF("CPU FIFO Buf Dump\n");
        dump_cpu_fifo_buf();
    }
    else if (strcmp(p_cmd_args->argv[0], "dbg") == 0)
    {
        set_cpu_core_1_tx_fifo_data_flg();
    }
    else if (strcmp(p_cmd_args->argv[0], "set") == 0)
    {
        tmp_u32 = (uint32_t)strtoul(p_cmd_args->argv[1], NULL, 16);
        Serial.printf("Set, CPU Core 1 Tx FIFO Data: 0x%08X\n", tmp_u32);
        set_cpu_core_1_tx_fifo_data(tmp_u32);
    }

    DBG_PRINTF("-------------------------------\n");
    return CMD_RESULT_EXEC_OK;
}

#if defined(RGBLED_PIN)
static E_DBG_CMD_RESULT _cmd_rgbled(void *p_args)
{
    uint8_t i;
    char color_num;
    dbg_cmd_args_t *p_cmd_args;

    DBG_PRINTF("-------------------------------\n");
    DBG_PRINTF("RGBLED Cmd\n");

    p_cmd_args = (dbg_cmd_args_t *)p_args;

    // 引数チェック
    if(strcmp(p_cmd_args->argv[0], "help") == 0)
    {
        for(i = 0; i < RGBLED_COLOR_ARGS_TBL_CNT; i++)
        {
            DBG_PRINTF("rgbled color %c -> %s\n",
                        g_rgbled_color_args_tbl[i].color_num,
                        g_rgbled_color_args_tbl[i].p_color_str);
        }
    }
    else if(strcmp(p_cmd_args->argv[0], "color") == 0)
    {
        color_num = *(p_cmd_args->argv[1]);
        for(i = 0; i < RGBLED_COLOR_ARGS_TBL_CNT; i++)
        {
            if(color_num == g_rgbled_color_args_tbl[i].color_num)
            {
                app_neopixel_set_rgb(0, (led_color_t *) g_rgbled_color_args_tbl[i].p_color);
                DBG_PRINTF("Set RGBLED Color: %s\n", g_rgbled_color_args_tbl[i].p_color_str);
                break;
            }
        }
    }

    DBG_PRINTF("-------------------------------\n");
    return CMD_RESULT_EXEC_OK;
}
#endif
// ---------------------------------------------------
// ********** [CPU Core 0] ***********
/**
 * @brief アプリ初期化 Core 1
 */
void app_main_core_0_init(void)
{
    // TODO
}

/**
 * @brief アプリメイン Core 0
 */
void app_main_core_0(void)
{
    // TODO
}

// ---------------------------------------------------
// ********** [CPU Core 1] ***********
/**
 * @brief アプリ初期化 Core 1
 */
void app_main_core_1_init(void)
{
#if defined(I2S_USE)
    i2s_sound_init(I2S_DOUT_PIN, I2S_LRCLK_PIN, I2S_BCLK_PIN);
#endif

    dbg_cmd_init((dbg_cmd_config_t *) &g_dbg_cmd_config);
}

/**
 * @brief アプリメイン Core 1
 */
void app_main_core_1(void)
{
    dbg_cmd_poll();
    dbg_cmd_main();
}

// ---------------------------------------------------