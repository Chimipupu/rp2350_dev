/**
 * @file dbg_com.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief デバッグモニタのコア処理
 * @version 0.1
 * @date 2025-06-13
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#include "dbg_com.h"
#include "ansi_esc.h"

#define KEY_LEFT    'D'    // 左矢印キー（ESC[D）
#define KEY_RIGHT   'C'    // 右矢印キー（ESC[C）
#define KEY_DELETE  0x7F   // Deleteキー

// コマンド履歴
static char s_cmd_history[CMD_HISTORY_MAX][DBG_CMD_MAX_LEN];
static uint8_t s_history_count = 0;  // コマンド履歴の数
static int8_t s_history_pos = -1;    // 現在の履歴位置（-1は最新）
static int32_t s_cursor_pos = 0;  // カーソル位置（0からs_cmd_indexの範囲）

static void dbg_com_execute_cmd(dbg_cmd_t cmd, dbg_cmd_args_t *p_args);

static void move_cursor_left(void);
static void move_cursor_right(void);
static void insert_char_at_cursor(char c);
static void delete_char_at_cursor(void);
static void backspace_at_cursor(void);
static void redraw_command_line(void);
static void clear_command_line(void);
static void overwrite_char_at_cursor(char c);

static int32_t split_str(char* p_str, dbg_cmd_args_t *p_args);

// コマンドバッファ
static char s_cmd_buffer[DBG_CMD_MAX_LEN];
static int32_t s_cmd_index = 0;

extern const size_t g_cmd_tbl_size;
extern void cmd_help(dbg_cmd_args_t *p_args);

/**
 * @brief カーソルを左に移動
 */
static void move_cursor_left(void)
{
    if (s_cursor_pos > 0) {
        s_cursor_pos--;
        printf("\b");  // カーソルを1文字左に移動
    }
}

/**
 * @brief カーソルを右に移動
 */
static void move_cursor_right(void)
{
    if (s_cursor_pos < s_cmd_index) {
        printf("%c", s_cmd_buffer[s_cursor_pos]);
        s_cursor_pos++;
    }
}

/**
 * @brief カーソル位置の文字を書き換え（上書きモード）
 * 
 * @param c 書き換える文字
 */
static void overwrite_char_at_cursor(char c)
{
    // カーソル位置の文字を上書き
    s_cmd_buffer[s_cursor_pos] = c;

    // バッファの末尾を超える場合は文字列長を更新
    if (s_cursor_pos >= s_cmd_index) {
        s_cmd_index = s_cursor_pos + 1;
        // バッファサイズチェック
        if (s_cmd_index >= DBG_CMD_MAX_LEN) {
            s_cmd_index = DBG_CMD_MAX_LEN - 1;
            return;
        }
    }

    // 文字を表示してカーソルを進める
    printf("%c", c);
    s_cursor_pos++;
}

/**
 * @brief カーソル位置の文字を書き換え（上書きモード）
 * 
 * @param c 書き換える文字
 */
static void insert_char_at_cursor(char c)
{
    if (s_cursor_pos >= DBG_CMD_MAX_LEN - 1) {
        return;  // バッファが満杯
    }

    // カーソル位置の文字を上書き
    s_cmd_buffer[s_cursor_pos] = c;

    // バッファの末尾を超える場合は文字列長を更新
    if (s_cursor_pos >= s_cmd_index) {
        s_cmd_index = s_cursor_pos + 1;
    }

    // 文字を表示してカーソルを進める
    printf("%c", c);
    s_cursor_pos++;
}

/**
 * @brief カーソル位置の文字を削除（Delete機能）
 */
static void delete_char_at_cursor(void)
{
    if (s_cursor_pos < s_cmd_index) {
        // カーソル位置以降の文字を左にシフト
        for (int32_t i = s_cursor_pos; i < s_cmd_index - 1; i++) {
            s_cmd_buffer[i] = s_cmd_buffer[i + 1];
        }

        s_cmd_index--;

        // カーソル位置以降を再描画
        for (int32_t i = s_cursor_pos; i < s_cmd_index; i++) {
            printf("%c", s_cmd_buffer[i]);
        }
        printf(" ");  // 最後の文字を消去

        // カーソルを正しい位置に戻す
        for (int32_t i = s_cursor_pos; i <= s_cmd_index; i++) {
            printf("\b");
        }
    }
}

/**
 * @brief カーソル位置の前の文字を削除（Backspace機能）
 */
static void backspace_at_cursor(void)
{
    if (s_cursor_pos > 0) {
        s_cursor_pos--;

        // カーソル位置以降の文字を左にシフト
        for (int32_t i = s_cursor_pos; i < s_cmd_index - 1; i++) {
            s_cmd_buffer[i] = s_cmd_buffer[i + 1];
        }

        s_cmd_index--;

        // カーソルを1文字左に移動してから再描画
        printf("\b");

        // カーソル位置以降を再描画
        for (int32_t i = s_cursor_pos; i < s_cmd_index; i++) {
            printf("%c", s_cmd_buffer[i]);
        }
        printf(" ");  // 最後の文字を消去

        // カーソルを正しい位置に戻す
        for (int32_t i = s_cursor_pos; i <= s_cmd_index; i++) {
            printf("\b");
        }
    }
}

/**
 * @brief コマンドラインを再描画
 */
static void redraw_command_line(void)
{
    // 現在の行をクリア
    printf("\r> ");

    // コマンドバッファを表示
    for (int32_t i = 0; i < s_cmd_index; i++) {
        printf("%c", s_cmd_buffer[i]);
    }

    // カーソルを正しい位置に移動
    for (int32_t i = s_cursor_pos; i < s_cmd_index; i++) {
        printf("\b");
    }
}

/**
 * @brief コマンドラインをクリア
 */
static void clear_command_line(void)
{
    // 現在の入力バッファをクリア
    while (s_cmd_index > 0) {
        printf("\b \b");
        s_cmd_index--;
        WDT_RST();
    }
    s_cursor_pos = 0;
}

// コマンド引数を分割して解析
static int32_t split_str(char* p_str, dbg_cmd_args_t *p_args)
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

/**
 * @brief コマンドを履歴に追加する関数
 * 
 * @param p_cmd 追加するコマンド
 */
static void add_to_cmd_history(const char* p_cmd)
{
    // 履歴を1つずつ下にずらす
    for (int32_t i = CMD_HISTORY_MAX - 1; i > 0; i--)
    {
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
 * @brief コマンド文字列を解析してコマンド種類を返す
 * 
 * @param p_cmd_str コマンド文字列
 * @param p_args 引数構造体
 * @return dbg_cmd_t コマンド種類
 */
static dbg_cmd_t dbg_com_parse_cmd(const char* p_cmd_str, dbg_cmd_args_t *p_args)
{
    for (uint8_t i = 0; i < g_cmd_tbl_size; i++)
    {
        if (strcmp(p_cmd_str, g_cmd_tbl[i].p_cmd_str) == 0) {
            // 引数の数をチェック
            // if (p_args->argc - 1 < g_cmd_tbl[i].min_args || p_args->argc - 1 > g_cmd_tbl[i].max_args) {
            //     printf("Error: Invalid number of arguments. Expected %d-%d, got %d\n",
            //         g_cmd_tbl[i].min_args, g_cmd_tbl[i].max_args, p_args->argc - 1);
            //     return CMD_UNKNOWN;
            // }
            return g_cmd_tbl[i].cmd_type;
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
static void dbg_com_execute_cmd(dbg_cmd_t cmd, dbg_cmd_args_t *p_args)
{
    uint8_t i;

    for(i = 0; i < g_cmd_tbl_size; i++)
    {
        if(g_cmd_tbl[i].cmd_type == cmd) {
            g_cmd_tbl[i].p_func(p_args);
        }
    }
}

/**
 * @brief デバッグコマンドモニターの初期化
 */
void dbg_com_init(void)
{
    s_cmd_index = 0;
    s_cursor_pos = 0;
    printf(ANSI_ESC_CLS);
    cmd_help(NULL);
}

/**
 * @brief デバッグコマンドモニターのメイン処理
 */
void dbg_com_main(void)
{
    dbg_cmd_args_t args;
    int32_t c;

    if (s_cmd_index >= DBG_CMD_MAX_LEN - 1) {
        s_cmd_index = 0;
        s_cursor_pos = 0;
    }

    c = getchar();

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
            s_cursor_pos = 0;
            printf("> ");
        } else {
            printf("\n> ");
        }
    } else if (c == '\b' || c == KEY_BACKSPACE) {
        // Backspace処理
        backspace_at_cursor();
    } else if (c == KEY_DELETE) {
        // Delete処理
        delete_char_at_cursor();
    } else if (c == KEY_ESC) { // ESC
        c = getchar();
        if (c == KEY_ANSI_ESC) { // ANSI escape sequence
            c = getchar();
            if (c == KEY_UP) { // キーボードの上矢印
                if (s_history_pos < s_history_count - 1) {
                    // 現在の入力バッファをクリア
                    clear_command_line();

                    // コマンド履歴を1つ古いものに
                    s_history_pos++;
                    strcpy(s_cmd_buffer, s_cmd_history[s_history_pos]);
                    s_cmd_index = strlen(s_cmd_buffer);
                    s_cursor_pos = s_cmd_index; // カーソルを末尾に
                    printf("%s", s_cmd_buffer);
                }
            } else if (c == KEY_DOWN) { // キーボードの下矢印
                if (s_history_pos >= 0) {
                    // コマンド履歴の現在の入力バッファをクリア
                    clear_command_line();

                    // 履歴を1つ新しいものに
                    s_history_pos--;
                    if (s_history_pos < 0) {
                        s_cmd_index = 0;
                        s_cursor_pos = 0;
                    } else {
                        strcpy(s_cmd_buffer, s_cmd_history[s_history_pos]);
                        s_cmd_index = strlen(s_cmd_buffer);
                        s_cursor_pos = s_cmd_index; // カーソルを末尾に
                        printf("%s", s_cmd_buffer);
                    }
                }
            } else if (c == KEY_LEFT) { // 左矢印キー
                move_cursor_left();
            } else if (c == KEY_RIGHT) { // 右矢印キー
                move_cursor_right();
            }
        }
    } else if (c >= ' ' && c <= '~') {
        insert_char_at_cursor(c);
    }
}