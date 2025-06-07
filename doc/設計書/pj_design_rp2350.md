# RP2350評価F/W設計書

## 1. 概要

RP2350で動作するデバッグコマンドモニターの設計仕様書。
整数演算、浮動小数点演算、三角関数演算などの実行時間の計測も可能して性能を評価する。

## 2. システム構成

### 2.1 モジュール構成

```mermaid
graph TD
    A[app_main] --> B[dbg_com]
    A --> C[演算性能評価]
    A --> D[実行時間計測]
    B --> E[コマンド解析]
    B --> F[コマンド実行]
    C --> G[整数演算]
    C --> H[浮動小数点演算]
    C --> I[三角関数演算]
    C --> J[円周率計算]
```

### 2.2 シーケンス図

#### 2.2.1 システム起動シーケンス
```mermaid
sequenceDiagram
    participant Core0
    participant Core1
    participant dbg_com
    
    Core0->>Core1: 起動
    Core1->>dbg_com: 初期化
    dbg_com-->>Core1: 初期化完了
    Core1->>dbg_com: コマンド待機
```

#### 2.2.2 コマンド実行シーケンス
```mermaid
sequenceDiagram
    participant User
    participant dbg_com
    participant app_main
    
    User->>dbg_com: コマンド入力
    dbg_com->>dbg_com: コマンド解析
    dbg_com->>app_main: テスト関数実行
    app_main->>app_main: 実行時間計測
    app_main-->>dbg_com: 結果返却
    dbg_com-->>User: 結果表示
```

## 3. API仕様

### 3.1 デバッグコマンドモニター (dbg_com)

#### 3.1.1 初期化関数
```c
void dbg_com_init(void);
```
- 機能: デバッグコマンドモニターの初期化
- 引数: なし
- 戻り値: なし

#### 3.1.2 メイン処理関数
```c
void dbg_com_process(void);
```
- 機能: デバッグコマンドモニターのメイン処理
- 引数: なし
- 戻り値: なし

### 3.2 アプリケーション (app_main)

#### 3.2.1 整数演算テスト
```c
void int_add_test(void);
void int_sub_test(void);
void int_mul_test(void);
void int_div_test(void);
```
- 機能: 整数演算のテスト実行
- 引数: なし
- 戻り値: なし
- 備考: TEST_LOOP_CNT回（100万回）の演算を実行

#### 3.2.2 浮動小数点演算テスト
```c
void float_add_test(void);
void float_sub_test(void);
void float_mul_test(void);
void float_div_test(void);
void double_add_test(void);
void double_sub_test(void);
void double_mul_test(void);
void double_div_test(void);
```
- 機能: 浮動小数点演算のテスト実行
- 引数: なし
- 戻り値: なし
- 備考: TEST_LOOP_CNT回（100万回）の演算を実行

#### 3.2.3 三角関数テスト
```c
void trig_functions_test(void);
void atan2_test(void);
void tan_355_226_test(void);
void inverse_sqrt_test(void);
```
- 機能: 三角関数演算のテスト実行
- 引数: なし
- 戻り値: なし
- 備考: 各関数の演算結果を表示

#### 3.2.4 円周率計算
```c
double calculate_pi_gauss_legendre(int iterations);
```
- 機能: Gauss-Legendreアルゴリズムによる円周率計算
- 引数: iterations - 計算の反復回数
- 戻り値: 計算された円周率の値
- 備考: 高精度な計算結果を返却

#### 3.2.5 実行時間計測
```c
void measure_execution_time(void (*func)(void), const char* func_name, ...);
```
- 機能: 関数の実行時間を計測
- 引数:
  - func: 計測対象の関数ポインタ
  - func_name: 関数名（表示用）
  - ...: 可変長引数
- 戻り値: なし
- 備考: time_us_32()を使用したマイクロ秒単位の計測

## 4. ソース仕様

### 4.1 定数定義
```c
#define TEST_LOOP_CNT 1000000  // 演算テストの実行回数
#define DBG_CMD_MAX_LEN 32     // コマンドの最大長
#define DBG_CMD_MAX_ARGS 4     // コマンドの最大引数数
```

### 4.2 データ構造
```c
typedef struct {
    const char* p_cmd_str;     // コマンド文字列
    dbg_cmd_t cmd_type;        // コマンド種類
    const char* p_description; // コマンドの説明
    int32_t min_args;          // 最小引数数
    int32_t max_args;          // 最大引数数
} dbg_cmd_info_t;

typedef struct {
    int32_t argc;                    // 引数の数
    char* p_argv[DBG_CMD_MAX_ARGS]; // 引数の配列
} dbg_cmd_args_t;
```

## 5. コマンド仕様

### 5.1 実装コマンド一覧

| コマンド | 説明 | 引数 |
|---------|------|------|
| help | コマンド一覧表示 | なし |
| ver | SDKバージョン表示 | なし |
| clock | システムクロック情報表示 | なし |
| at | int/float/double四則演算テスト | なし |
| pi | 円周率計算 | [実行回数] |
| trig | 三角関数テスト実行 | なし |
| atan2 | atan2関数テスト実行 | なし |
| tan355 | tan(355/226)テスト実行 | なし |
| isqrt | 逆平方根テスト実行 | なし |
| rst | システムリセット | なし |

### 5.2 コマンド実行フロー
1. コマンド入力受付
2. コマンド文字列の解析
3. コマンド種類の判定
4. 引数の数チェック
5. コマンド実行
6. 結果表示

## 6. ビルドシステム仕様

### 6.1 開発環境
- CMake 3.13以上
- C言語標準: C11
- C++言語標準: C++17
- Pico SDK: 2.1.1
- ツールチェーン: 14_2_Rel1
- picotool: 2.1.1

### 6.2 ビルド設定
```cmake
# コンパイルオプション
- 浮動小数点演算: H/Wの倍精度FPU（-mfloat-abi=hard）
- 標準出力: USB経由 (UART無効)
- コンパイルコマンドのエクスポート有効

# リンクライブラリ
- pico_stdlib
- pico_multicore
- hardware_spi
- hardware_i2c
- hardware_dma
- hardware_pio
- hardware_interp
- hardware_timer
- hardware_watchdog
- hardware_clocks
```

### 6.3 ターゲット設定
- ボード: pico2
- プロジェクト名: rp2350_dev
- バージョン: 0.1

## 7. ハードウェア初期化仕様

### 7.1 通信インターフェース初期化

#### 7.1.1 SPI設定
```c
- クロック周波数: 1MHz
- ピン設定:
  - MISO: GPIO_FUNC_SPI
  - CS: GPIO_FUNC_SIO (出力)
  - SCK: GPIO_FUNC_SPI
  - MOSI: GPIO_FUNC_SPI
```

#### 7.1.2 I2C設定
```c
- クロック周波数: 400KHz
- ピン設定:
  - SDA: GPIO_FUNC_I2C (プルアップ有効)
  - SCL: GPIO_FUNC_I2C (プルアップ有効)
```

#### 7.1.3 UART設定
```c
- ボーレート: 115200bps
- フォーマット: 8N1
- ピン設定:
  - TX: GPIO_FUNC_UART
  - RX: GPIO_FUNC_UART
```

### 7.2 DMA設定
```c
- チャンネル: 未使用チャンネルを自動選択
- 転送サイズ: 8ビット
- モード: ブロッキング
- 設定:
  - 読み出しアドレスインクリメント: 有効
  - 書き込みアドレスインクリメント: 有効
```

### 7.3 PIO設定
```c
- 使用PIO: pio0
- プログラム: blink_program
- ピン設定:
  - デフォルトLEDピンまたはGPIO6
  - 周波数: 3Hz
```

### 7.4 割り込み設定
```c
- タイマー割り込み: 2000ms間隔
- 補間器設定: デフォルト設定
```

### 7.5 WDT(ウォッチドッグタイマー)設定
```c
- 状態: 無効化（デバッグ用）
- 設定値:
  - タイムアウト: 100ms
  - デバッグ時一時停止: 有効
```

### 7.6 マルチコア設定
```c
- CPU Core0: メインアプリケーション実行
- CPU Core1: デバッグコマンドモニター実行
- 起動シーケンス:
  1. CPU Core0を初期化
  2. CPU Core1を起動
  3. 各コアで独立したメインループ実行
```
