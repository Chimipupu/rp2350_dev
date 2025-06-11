# RP2350の性能評価

RP2350の評価用F/Wの個人開発リポジトリ

## 開発環境

『H/W』

- Raspberry Pi Pico 2

『S/W』

- Pico SDK ... Ver2.1.1

## 概要

### システム構成
- マルチコア構成（CPU Core0, Core1）
  - CPU Core0: メインアプリケーション実行
  - CPU Core1: デバッグコマンドモニター実行
- 通信インターフェース
  - SPI: 1MHz, 8bit
  - I2C: 400KHz
  - UART: 115200bps, 8N1
- その他H/W機能
  - DMA: 8bit転送
  - PIO: LED制御（3Hz）
  - タイマー割り込み: 2000ms
  - WDT: デバッグ時無効

### ビルド構成
- コンパイラ設定
  - C言語: C11
  - C++: C++17
  - 浮動小数点演算: H/Wの倍精度FPU（-mfloat-abi=hard）
- 標準出力: USB経由（UART無効）
- リンクライブラリ
  - pico_stdlib
  - pico_multicore
  - hardware_spi/i2c/dma/pio/interp/timer/watchdog/clocks

## 実装内容

F/Wの詳細は[設計書🔗](/doc/設計書/pj_rp2350.md)を確認すること

### コマンド一覧
  - `help` - コマンド一覧表示
    ```
    > help
    Debug Command Monitor for RP2350 Ver 0.1
    Copyright (c) 2025 Chimipupu(https://github.com/Chimipupu)
    Type 'help' for available commands

    Available commands:
      help      - Show this help message
      ver       - Show version information
      clock     - Show clock information
      at        - int/float/double arithmetic test
      pi        - Calculate pi using Gauss-Legendre
      trig      - Run sin,cos,tan functions test
      atan2     - Run atan2 test
      tan355    - Run tan(355/226) test
      isqrt     - Run 1/sqrt(x) test
      timer     - Set timer alarm (seconds)
      gpio      - Control GPIO pin (pin, value)
      mem_dump  - Dump memory contents (address, length)
      rst       - Reboot
    ```

  - `ver` - SDKバージョン表示
    ```
    > ver
    Pico SDK version: 2.1.1
    ```

  - `clock` - システムクロック情報表示
    ```
    > clock
    System Clock:    133 MHz
    USB Clock:      48 MHz
    ```

  - `at` - int/float/double四則演算テスト
    ```
    > at
    Integer Arithmetic Test:
    proc time int_add_test: 1234 us
    proc time int_sub_test: 1234 us
    proc time int_mul_test: 1234 us
    proc time int_div_test: 1234 us

    Float Arithmetic Tests:
    proc time float_add_test: 1234 us
    proc time float_sub_test: 1234 us
    proc time float_mul_test: 1234 us
    proc time float_div_test: 1234 us

    Double Arithmetic Tests:
    proc time double_add_test: 1234 us
    proc time double_sub_test: 1234 us
    proc time double_mul_test: 1234 us
    proc time double_div_test: 1234 us
    ```

  - `pi [iterations]` - 円周率計算（反復回数指定可能）
    ```
    > pi 3
    Calculating Pi using Gauss-Legendre algorithm (3 iterations):
    Iteration 1: π ≈ 3.140579250522168 (proc time: 1234 us)
    Iteration 2: π ≈ 3.141592646213543 (proc time: 1234 us)
    Iteration 3: π ≈ 3.141592653589794 (proc time: 1234 us)
    ```

  - `trig` - 三角関数テスト実行
    ```
    > trig
    Trigonometric Functions Test:
    proc time trig_functions_test: 1234 us
    Test completed: sin(45°), cos(45°), tan(45°)
    ```

  - `atan2` - atan2関数テスト実行
    ```
    > atan2
    Atan2 Test:
    proc time atan2_test: 1234 us
    Test completed: atan2(1.0, 1.0)
    ```

  - `tan355` - tan(355/226)テスト実行
    ```
    > tan355
    Tan(355/226) Test:
    Expected: -7497258.185325587
    Calculated: -7497258.185325587
    Difference: 0.000000000 (0.00%)
    proc time tan_355_226_test: 1234 us
    ```

  - `isqrt` - 逆平方根テスト実行
    ```
    > isqrt
    Inverse Square Root Test:
    proc time inverse_sqrt_test: 1234 us
    Test completed: 1/sqrt(x) for x = 2.0, 3.0, 4.0, 5.0
    ```

  - `timer [seconds]` - タイマーアラーム設定（秒単位）
    ```
    > timer 5
    Timer #1 (alarm as #1) set for 5 seconds.
    > timer
    Timer #1 (alarm as #1): 3 seconds remaining.
    ```

  - `gpio <pin> <value>` - GPIO制御（ピン番号、値）
    ```
    > gpio 1 1
    GPIO 1 set to 1 (proc time: 1234 us)
    > gpio 1 0
    GPIO 1 set to 0 (proc time: 1234 us)
    ```

  - `mem_dump <address> <length>`
    - メモリダンプ（開始アドレス、長さ）

    ```
    > mem_dump #F0000000 #10
    Address  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F | ASCII
    -------- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- | ------
    F0000000: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F | ................

    Memory dump completed (proc time: 1234 us)
    ```

  - `rst` - システムリセット
    - ⚡ リセット機能
      - ウォッチドッグタイマーによる即時リセット

    ```
    > rst
    Resetting system...
    ```