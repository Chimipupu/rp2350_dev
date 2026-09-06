/**
 * @file dbg_cmd.cpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief デバッグシリアルコマンド
 * @version 0.1
 * @date 2026-08-23
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */
#include "dbg_cmd.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <Arduino.h>

// --------------------------------------------------------------------------
#define UART_RX_BUF_SIZE        128
#define UART_CMD_RX_BUF_SIZE    64

static volatile uint8_t s_rx_buf[UART_RX_BUF_SIZE];
static uint8_t s_rx_buf_idx = 0;
static volatile uint8_t s_rx_cmd_buf[UART_CMD_RX_BUF_SIZE];
static bool s_is_rx_uart_cmd_flg = false;

static dbg_cmd_tbl_t *ps_cmd_tbl = NULL;
static E_DBG_CMD_TYPE s_cmd_type = CMD_TYPE_NONE;
static uint8_t s_cmd_num;
static uint8_t s_rx_cmd_no;
static dbg_cmd_config_t s_cmd_config;
p_cmd_func ps_exec_cmd_func = NULL;

static dbg_cmd_args_t s_cmd_args;

static E_DBG_CMD_RESULT _cmd_help(void *p_args);
static E_DBG_CMD_RESULT _cmd_clear(void *p_args);
static const dbg_cmd_tbl_t s_basic_cmd_tbl[] = {
    {"help", "?", _cmd_help},
    {"clear", "cls", _cmd_clear},
};
static const uint8_t BASIC_CMD_NUM = sizeof(s_basic_cmd_tbl) / sizeof(s_basic_cmd_tbl[0]);

// --------------------------------------------------------------------------
// [Static]

static bool _cmd_ready(uint8_t *p_cmd_buf)
{
    bool is_ret;
    uint8_t i;
    char *p_token;
    dbg_cmd_tbl_t *p_tbl;

    is_ret = false;
    p_tbl = ps_cmd_tbl;

    if(s_cmd_config.p_printf == NULL)
    {
        return false;
    }

    if(s_rx_buf_idx == 0)
    {
        s_is_rx_uart_cmd_flg = false;
        return false;
    }

    // CRLFを受信までをコピー、ヌル文字の終端をいれる
    for(i = 0; i < s_rx_buf_idx; i++)
    {
        if((s_rx_buf[i] == '\r') || (s_rx_buf[i] == '\n')) {
            p_cmd_buf[i] = '\0';
            break;
        }
        p_cmd_buf[i] = s_rx_buf[i];
    }
    p_cmd_buf[i] = '\0';

    s_cmd_type = CMD_TYPE_NONE;
    s_cmd_args.argc = 0;

    // スペース区切りで文字列分割
    p_token = strtok((char *)p_cmd_buf, " ");

    if (p_token != NULL)
    {
        // 最初のトークンをコマンド文字列で判定処理
        for(i = 0; i < BASIC_CMD_NUM; i++)
        {
            if((strcmp(s_basic_cmd_tbl[i].p_cmd_str, p_token) == 0) || \
                (strcmp(s_basic_cmd_tbl[i].p_cmd_str_short, p_token) == 0)
            ) {
                s_cmd_type = CMD_TYPE_BASIC;
                ps_exec_cmd_func = s_basic_cmd_tbl[i].pfunc;
                s_cmd_config.p_printf("Basic Cmd: %s\n", p_token);
                break;
            }
        }

        if(s_cmd_type == CMD_TYPE_NONE)
        {
            for(i = 0; i < s_cmd_num; i++)
            {
                if((strcmp(p_tbl->p_cmd_str, p_token) == 0) || \
                (strcmp(p_tbl->p_cmd_str_short, p_token) == 0)
                ) {
                    s_cmd_type = CMD_TYPE_EXT;
                    ps_exec_cmd_func = p_tbl->pfunc;
                    s_cmd_config.p_printf("Ext Cmd: %s\n", p_token);
                    break;
                }
                p_tbl++;
            }
        }

        // 残りのトークンを引数として取得
        while ((p_token = strtok(NULL, " ")) != NULL)
        {
            if (s_cmd_args.argc < DBG_CMD_MAX_ARGS)
            {
                s_cmd_args.argv[s_cmd_args.argc] = p_token;
                s_cmd_args.argc++;
            }
        }
    }

    switch (s_cmd_type)
    {
        case CMD_TYPE_BASIC:
        case CMD_TYPE_EXT:
            s_rx_cmd_no = i;
            is_ret = true;
            break;

        case CMD_TYPE_NONE:
        default:
            s_cmd_config.p_printf("?\r\n");
            ps_exec_cmd_func = NULL;
            break;
    }

    memset((void *)&s_rx_buf[0], 0x00, UART_RX_BUF_SIZE);
    s_rx_buf_idx = 0;
    s_is_rx_uart_cmd_flg = false;

    return is_ret;
}

static E_DBG_CMD_RESULT _cmd_help(void *p_args)
{
    uint8_t i;

    if(s_cmd_config.p_printf == NULL)
    {
        return CMD_RESULT_EXEC_ERROR;
    }


    s_cmd_config.p_printf("Help Cmd Exec\r\n");
    s_cmd_config.p_printf("--------------------------------\r\n");
    s_cmd_config.p_printf("Basic Commands\r\n");
    s_cmd_config.p_printf("No, Cmd, Short Cmd\r\n");

    for(i = 0; i < BASIC_CMD_NUM; i++)
    {
        s_cmd_config.p_printf(ASCII_TXT_GREEN
                            "%d, %s, %s\r\n"
                            ASCII_RESET,
                            i, s_basic_cmd_tbl[i].p_cmd_str, s_basic_cmd_tbl[i].p_cmd_str_short);
    }

    if(s_cmd_config.p_ext_cmd_tbl != NULL) {
        s_cmd_config.p_printf("--------------------------------\r\n");
        s_cmd_config.p_printf("Extend Commands\r\n");
        s_cmd_config.p_printf("No, Cmd, Short Cmd\r\n");

        for(i = 0; i < s_cmd_config.ext_cmd_num; i++)
        {
            s_cmd_config.p_printf(ASCII_TXT_GREEN
                                "%d, %s, %s\r\n"
                                ASCII_RESET,
                                i, s_cmd_config.p_ext_cmd_tbl[i].p_cmd_str,
                                s_cmd_config.p_ext_cmd_tbl[i].p_cmd_str_short
            );
        }
    }

    s_cmd_config.p_printf("--------------------------------\r\n");

    return CMD_RESULT_EXEC_OK;
}

static E_DBG_CMD_RESULT _cmd_clear(void *p_args)
{
    if(s_cmd_config.p_printf == NULL)
    {
        return CMD_RESULT_EXEC_ERROR;
    }

    s_cmd_config.p_printf(ANSI_ESC_CLEAR);

    return CMD_RESULT_EXEC_OK;
}
// --------------------------------------------------------------------------
// [APP]

void dbg_cmd_poll(void)
{
    uint8_t tmp;

    if(s_cmd_config.p_serial_read != NULL)
    {
        tmp = s_cmd_config.p_serial_read();

        if ((tmp == KEY_BACK_SPACE) || (tmp == KEY_DEL))
        {
            if(s_rx_buf_idx > 1)
            {
                s_rx_buf_idx--;
            }
            else
            {
                s_rx_buf_idx = 0;
            }
        }
        else
        {
            if ((tmp == '\r') || (tmp == '\n'))
            {
                s_is_rx_uart_cmd_flg = true;
            }

            // ASCII判定
            if ((tmp >= 0x20) && (tmp <= 0x7E))
            {
                s_rx_buf[s_rx_buf_idx] = tmp;
                s_rx_buf_idx = (s_rx_buf_idx + 1) % UART_RX_BUF_SIZE;
            }
        }
    }
}

void dbg_cmd_init(dbg_cmd_config_t *p_cmd_config)
{
    ps_cmd_tbl = (dbg_cmd_tbl_t *) &s_basic_cmd_tbl[0];

    if(p_cmd_config == NULL) {
        return;
    }

    if((p_cmd_config->p_serial_read == NULL) || (p_cmd_config->p_printf == NULL))
    {
        return;
    }

    if((p_cmd_config->p_ext_cmd_tbl == NULL) || (p_cmd_config->ext_cmd_num == 0)) {
        return;
    }

    s_cmd_config = *p_cmd_config;
    s_cmd_num = s_cmd_config.ext_cmd_num;
    ps_cmd_tbl = p_cmd_config->p_ext_cmd_tbl;

    _cmd_help(NULL);
}

void dbg_cmd_main(void)
{
    bool is_ret;

    if(s_is_rx_uart_cmd_flg != false)
    {
        is_ret = _cmd_ready((uint8_t *) &s_rx_cmd_buf[0]);

        if(is_ret != false)
        {
            for (uint8_t i = 0; i < s_cmd_args.argc; i++)
            {
                s_cmd_config.p_printf("Arg[%d]: %s\n", i, s_cmd_args.argv[i]);
            }
            ps_exec_cmd_func((void *)&s_cmd_args);
        }

        memset((void *)&s_rx_cmd_buf[0], 0x00, UART_CMD_RX_BUF_SIZE);
    }
}