# RP2350の性能評価

- RP2350の評価用F/Wの個人開発リポジトリ

## 開発環境

### H/W

<div align="center">
  <img width="500" src="/doc/写真/rp2350_dev_env_20250613.png">
</div>

- 基板
  - [`Raspberry Pi Pico 2`](https://www.raspberrypi.com/products/raspberry-pi-pico-2/)🔗
  - [`Raspberry Pi Pico 2 W`](https://www.raspberrypi.com/products/raspberry-pi-pico-2w)🔗
  - [`WeActStudio RP2350B`](https://github.com/WeActStudio/WeActStudio.RP2350BCoreBoard)🔗
  - [`WeActStudio RP2350A-V10`](https://github.com/WeActStudio/WeActStudio.RP2350ACoreBoard/tree/main/RP2350A_V10)🔗
  - [`WeActStudio RP2350A-V20`](https://github.com/WeActStudio/WeActStudio.RP2350ACoreBoard/tree/main/RP2350A_V20)🔗
  - [`Waveshare RP2350-PiZero`](https://www.waveshare.com/wiki/RP2350-PiZero)🔗

- マイコン ... 型番:`RP2350`
  - CPU ... `Arm Cortex-M33` x2コア(※デュアルコアCPU)
  - ROM ... 4MB or 16MB
  - RAM ... 520KB
  - UART ... 2ch
  - SPI ... 2ch
  - I2C ... 2ch
  - GPIO ... 30 or 48本
  - DMA ... 16本
  - タイマー
    - SysTick : 24bit x1本
    - WDT : 24bit x1本
    - RTC : なし(N/A)
    - 64bitタイマー
      - 汎用 : x2本 (1usカウント)
        - アラーム : 4つまで
      - AON Timer : x1本
        - 常時駆動の64bitタイマ
        - LPOSCの32.768KHz(精度1%)で@1sカウント

<!-- - I2Cスレーブ
  - `OLED液晶` ... 型番:`SSD1306` ※アドレス:0x3C
  - `湿温度センサ` ... 型番:`BME280` ※アドレス:0x76
  - `RTC` ... 型番:DS3231 ※アドレス:0x68 -->

### S/W

- Pico SDK ... Ver2.1.1
  - コンパイラ
  - gcc
    - コンパイルオプション設定
      - 最適化
        - `-O0` (最適化なし)
        - `-O3` (最適化最大)
        - `-Os` (サイズ優先)
        - `-Og` (デバッグ)
      - 浮動小数点
        - `-mfloat-abi=hard` H/Wの倍精度FPU
  - 標準出力: USB CDC経由でprintf()
  - リンクライブラリ
    - pico_stdlib
    - pico_multicore
    - hardware_spi/i2c/dma/pio/interp/timer/watchdog/clocks

### デバッガ

- [Debugprobe on pico](https://www.raspberrypi.com/documentation/microcontrollers/debug-probe.html)
  - F/W ... [v2.2.3](https://github.com/raspberrypi/debugprobe/releases/tag/debugprobe-v2.2.3)