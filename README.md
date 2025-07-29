# RP2xxxの性能評価F/W @PicoSDK版

- RP2xxx(RP2040 or RP2350)の性能評価F/W(PicoSDK版)の個人開発リポジトリ

## H/W

### RP2040基板

- 📍[VCC-GND YD-RP2040](https://www.aliexpress.us/item/1005004004120604.html?gatewayAdapt=4itemAdapt)🔗
- マイコン ... 型番:`RP2040`
  - CPU ... `Arm Cortex-M0+` x2コア(※デュアルコアCPU)
  - ROM ... 16MB
  - RAM ... 520KB

### RP2350基板
- 📍[Raspberry Pi Pico 2](https://www.raspberrypi.com/products/raspberry-pi-pico-2/)🔗
- 📍[Raspberry Pi Pico 2 W](https://www.raspberrypi.com/products/raspberry-pi-pico-2w)🔗
- 📍[WeActStudio RP2350B](https://github.com/WeActStudio/WeActStudio.RP2350BCoreBoard)🔗
- 📍[WeActStudio RP2350A-V10](https://github.com/WeActStudio/WeActStudio.RP2350ACoreBoard/tree/main/RP2350A_V10)🔗
- 📍[WeActStudio RP2350A-V20](https://github.com/WeActStudio/WeActStudio.RP2350ACoreBoard/tree/main/RP2350A_V20)🔗
- 📍[Waveshare RP2350-PiZero](https://www.waveshare.com/wiki/RP2350-PiZero)🔗
- 📍[Waveshare RP2350-Zero](https://www.waveshare.com/wiki/RP2350-Zero)🔗

- マイコン ... 型番:`RP2350`
  - CPU ... `Arm Cortex-M33` x2コア(※デュアルコアCPU)
  - ROM ... 4MB or 16MB
  - RAM ... 520KB
  - FPU ... 倍精度FPU
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

## S/W

- Pico SDK ... Ver2.1.1
  - コンパイラ
  - gcc
    - コンパイルオプション設定
      - 最適化
        - `-O0` (最適化なし)
        - `-O3` (最適化最大)
        - `-Os` (サイズ優先)
        - `-Og` (デバッグ)
      - 浮動小数点(※RP2350がマイコンのとき)
        - `-mfloat-abi=hard` H/Wの倍精度FPU
  - 標準出力: USB CDC経由でprintf()
  - リンクライブラリ
    - pico_stdlib
    - pico_multicore
    - hardware_spi/i2c/dma/pio/interp/timer/watchdog/clocks

## デバッガ

- 📍[Debugprobe on pico](https://www.raspberrypi.com/documentation/microcontrollers/debug-probe.html)
  - F/W ... [v2.2.3](https://github.com/raspberrypi/debugprobe/releases/tag/debugprobe-v2.2.3)