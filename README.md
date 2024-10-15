# RP2350評価プログラム(C++)

RP2350 Firmeware C++ Develop by Chimi\
RP2350のC++ 個人開発リポジトリ🥳

## 開発環境構築
### VSCode
Zenn『Pico SDKの開発環境構築の最適解 📝』 by ちみ
- https://zenn.dev/chimipupu/articles/039597a904849f

### Arduino IED
- `基本設定`から👇下記URLを追加

```
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
```

- `ボードマネージャ`から`Raspberry Pi Pico/RP2040 by Earle F Philhower, III`をダウンロード or 更新

## ✅実装した機能

- ARM Cortex-M33 x2コア
  - ✅Core0 ... ドライバコア
    - Neopixcelのアプリ
  - ✅Core1 ... アプリコア
    - ✅OLEDのアプリ
    - ✅モニタプログラム(UART)
    - ✅数学アプリ
      - 円周率π、自然数の底e、黄金比、フィボナッチ数列、高速逆平方根
- PIO x3基
    - ✅PIO0 ... TBD
    - ✅PIO1 ... TBD
    - ✅PIO2 ... TBD

## TBD🛠️
- ARM Cortex-M33
    - 🛠️割込み（IRQ） ... ボタンやPIOからIRQがほしい
    - 🛠️FreeRTOS ... CPU x2コア分を搭載
    - 🛠️アプリ追加 ... I2C、SPI、スリープ機能

- PIO
    - 🛠️GPIO ... PIOでGPIOをパタパタさせたい
    - 🛠️VGA ... モニターで描画したい
    - 🛠️SDIO ... SDカードを使いたい

## 🔰RP2350とは？🔰
<div align="center">
<img width="500" src="https://cdn.discordapp.com/attachments/1291907929154392076/1295883998849011773/image.png?ex=67104585&is=670ef405&hm=72f56fe434ee1dc00928c103a50b59b196bce581a9b153f881c50017a785f3a3&">
</div>

Raspberry Piの **ARM Cortex-M33 x2コア と RISC-V x2コア** を搭載したマイコン🥳

[公式URL]
- https://www.raspberrypi.com/products/rp2350/

[データシート]
- https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf

## スペック
- クロック ... 150MHz
- CPU
    - x2コア ARM Cortex-M33
   - x2コア Hazard3(RV32I)
- SRAM ... 520KB
- FPU ... 倍精度
- PIO ... x3基(SM x12)
- UART ... x2ch
- SPI ... x2ch
- I2C ... x2ch
- GPIO ... 30本 or 48本
- PWM ... 24本

# パッケージ
- `RP2350A` ... OnChip  Flash N/A , QFN-60, GPIO x48
- `RP2350B` ... OnChip  Flash N/A, QFN-80, GPIO x48
- `RP2354A` ... OnChip Flash 2MB , QFN-60, GPIO x30
- `RP2354B` ... OnChip Flash 2MB , QFN-80, GPIO x48