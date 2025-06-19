# RP2350の性能評価

- RP2350の評価用F/Wの個人開発リポジトリ

## 開発環境

### デバッガ

- [Debugprobe on pico](https://www.raspberrypi.com/documentation/microcontrollers/debug-probe.html)
  - F/W ... [v2.2.2](https://github.com/raspberrypi/debugprobe/releases/tag/debugprobe-v2.2.2)

### H/W

<div align="center">
  <img width="500" src="/doc/写真/rp2350_dev_env_20250613.png">
</div>

- 基板 ... 型番:`Raspberry Pi Pico 2`
- マイコン ... 型番:`RP2350`
  - CPU ... `Arm Cortex-M33` x2コア(※デュアルコアCPU)
  - ROM ... 4MB
  - RAM ... 520KB

- I2Cスレーブ
  - `OLED液晶` ... 型番:`SSD1306` ※アドレス:0x3C
  - `湿温度センサ` ... 型番:`BME280` ※アドレス:0x76
  - `RTC` ... 型番:DS3231 ※アドレス:0x68

### S/W

- Pico SDK ... Ver2.1.1
  - コンパイラ設定
    - C言語: C11
    - C++: C++17
    - 浮動小数点演算: H/Wの倍精度FPU（-mfloat-abi=hard）
  - 標準出力: USB経由（UART無効）
  - リンクライブラリ
    - pico_stdlib
    - pico_multicore
    - hardware_spi/i2c/dma/pio/interp/timer/watchdog/clocks

## F/W

F/Wの詳細は[設計書🔗](/doc/設計書/pj_rp2350.md)を確認すること

### コマンド一覧

- `help` - コマンド一覧表示

<div align="center">
  <img width="500" src="/doc/写真/rp2350_dev_dbgcom_ver1.0.png">
</div>

  ```shell
  > help

  Debug Command Monitor for RP2350 Ver 0.1
  Copyright (c) 2025 Chimipupu(https://github.com/Chimipupu)
  Type 'help' for available commands

  Available commands:
    help       - Show this help message
    ver        - Show version information
    clock      - Show clock information
    rst        - Reboot
    mem_dump   - Dump memory contents (address, length)
    reg        - Register read/write: reg #addr r|w bits [#val]
    i2c        - I2C control (port, command)
    gpio       - Control GPIO pin (pin, value)
    timer      - Set timer alarm (seconds)
    at         - int/float/double arithmetic test
    pi         - Calculate pi using Gauss-Legendre
    trig       - Run sin,cos,tan functions test
    atan2      - Run atan2 test
    tan355     - Run tan(355/226) test
    isqrt      - Run 1/sqrt(x) test
  ```

- `reg <#Addr> <R/W> <Bits> <#Val>` - レジスタのR/W
  - `#Addr`: 32bitアドレス 例) #20000000
  - `R/W`: `r`読み取り、`w`書き込み
  - `Bits`: `8`8bit、`16`16bit、`32`32bit
  - `#Val`: ※`R/W`が`w`のときのみ有効

  <div align="center">
    <img width="500" src="/doc/写真/reg_cmd_write_verify_ok_ver1.0.png">
  </div>

  ```shell
  > reg #20000000 w 8 #AB
  [REG] Write 8bit @ 0x20000000 = 0xAB
  > reg #20000000 r 8
  [REG] Read 8bit @ 0x20000000 = 0xAB
  ```

- `mem_dump <address> <length>` - メモリダンプ
  - メモリダンプ（開始アドレス、長さ）

  ```shell
  > mem_dump #00000000 #DC
  Address  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F | ASCII
  -------- ------------------------------------------------| ------
  00000000: 00 00 00 F0 89 00 00 00 F9 02 00 00 F9 02 00 00 | ................
  00000010: 4D 75 02 02 D4 7C 75 00 35 00 28 43 29 20 32 30 | Mu...|u.5.(C) 20
  00000020: 32 34 20 52 61 73 70 62 65 72 72 79 20 50 69 20 | 24 Raspberry Pi
  00000030: 4C 74 64 00 84 FE 18 07 47 F6 D4 43 84 46 07 E0 | Ltd.....G..C.F..
  00000040: 10 2A 00 D3 02 33 4F 48 92 00 D0 41 00 0F 1B 18 | .*...3OH...A....
  00000050: 18 88 5A 88 58 B1 84 45 F2 D1 0A 42 F0 D0 11 40 | ..Z.X..E...B...@
  00000060: 49 08 03 D2 52 08 FB D3 02 33 F9 E7 18 1D A4 FE | I...R....3......
  00000070: 38 07 70 47 16 FE 3C 37 08 B5 FF F7 DB FF 00 88 | 8.pG..<7........
  00000080: 0C BC 06 FE 3C 27 18 47 3F 48 80 30 5F F4 40 41 | ....<'.G?H.0_.@A
  00000090: 01 60 80 22 42 60 00 F5 00 30 01 60 3A 48 C8 30 | .`."B`...0.`:H.0
  000000A0: 01 21 07 23 44 F2 A0 64 47 F6 E1 75 3E C0 4F F0 | .!.#D..dG..u>.O.
  000000B0: 50 40 01 68 29 BB 35 49 A1 F5 03 70 80 F3 0A 88 | P@.h).5I...p....
  000000C0: 00 22 04 C0 88 42 FC D1 8D 46 03 F0 49 BB 72 B6 | ."...B...F..I.r.
  000000D0: 30 BF FC E7 18 FE 31 C7 20 BF 20 6D             | 0.....1. . m

  Memory dump completed (proc time: 7541 us)
  ```

- `i2c <port> <mode>` - I2C通信制御
  - port: `0` (I2C0) or `1` (I2C1)
  - mode: `s` 7bitスレーブアドレス（0x00~0x7F）をすべてスキャン

  <div align="center">
    <img width="500" src="/doc/写真/i2c_scan_ver1.0.png">
  </div>

  ```shell
  > i2c 0 s
  Scanning I2C0 bus...
         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
  0x00:  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
  0x10:  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
  0x20:  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
  0x30:  -  -  -  -  -  -  -  -  -  -  -  -  *  -  -  -
  0x40:  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
  0x50:  -  -  -  -  -  -  -  *  -  -  -  -  -  -  -  -
  0x60:  -  -  -  -  -  -  -  -  *  -  -  -  -  -  -  -
  0x70:  -  -  -  -  -  -  *  -  -  -  -  -  -  -  -  -

  I2C Scan complete! (Slave:4, 0x3C, 0x57, 0x68, 0x76)
  ```

- `gpio <pin> <value>` - GPIO制御（ピン番号、値）

  ```shell
  > gpio 1 1
  GPIO 1 set to 1 (proc time: 1234 us)
  > gpio 1 0
  GPIO 1 set to 0 (proc time: 1234 us)
  ```

- `timer [seconds]` - タイマーアラーム設定（秒単位）

  ```shell
  > timer 5
  Timer #1 (alarm as #1) set for 5 seconds.
  > timer
  Timer #1 (alarm as #1): 3 seconds remaining.
  ```

- `rst` - システムリセット
  - ⚡ リセット機能
    - ウォッチドッグタイマーによる即時リセット

  ```shell
  > rst
  Resetting system...
  ```

- `ver` - SDKバージョン表示

  ```shell
  > ver
  Pico SDK version: 2.1.1
  ```

- `clock` - システムクロック情報表示

  ```shell
  > clock
  System Clock:    133 MHz
  USB Clock:      48 MHz
  ```

- `at` - int/float/double四則演算テスト

  ```shell
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

  ```shell
  > pi 3
  Calculating Pi using Gauss-Legendre algorithm (3 iterations):
  Iteration 1: π ≈ 3.140579250522168 (proc time: 1234 us)
  Iteration 2: π ≈ 3.141592646213543 (proc time: 1234 us)
  Iteration 3: π ≈ 3.141592653589794 (proc time: 1234 us)
  ```

- `trig` - 三角関数テスト実行

  ```shell
  > trig
  Trigonometric Functions Test:
  proc time trig_functions_test: 1234 us
  Test completed: sin(45°), cos(45°), tan(45°)
  ```

- `atan2` - atan2関数テスト実行

  ```shell
  > atan2
  Atan2 Test:
  proc time atan2_test: 1234 us
  Test completed: atan2(1.0, 1.0)
  ```

- `tan355` - tan(355/226)テスト実行

  ```shell
  > tan355
  Tan(355/226) Test:
  Expected: -7497258.185325587
  Calculated: -7497258.185325587
  Difference: 0.000000000 (0.00%)
  proc time tan_355_226_test: 1234 us
  ```

- `isqrt` - 逆平方根テスト実行

  ```shell
  > isqrt
  Inverse Square Root Test:
  proc time inverse_sqrt_test: 1234 us
  Test completed: 1/sqrt(x) for x = 2.0, 3.0, 4.0, 5.0
  ```
