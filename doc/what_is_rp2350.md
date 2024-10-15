# RP2350の調査 by ちみ

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

## H/W
### Raspberry Pi Pico 2
Flash 4MB, PSRAM N/A

- 秋月電子 ... **税込み880円**（2024年10月5日現在） https://akizukidenshi.com/catalog/g/g129604/

- スイッチサイエンス ... **税込み990円**（2024年10月5日現在） https://www.switch-science.com/products/9809

### PGA2350
Flash 16MB, PSRAM 8MB

- スイッチサイエンス ... **税込み1848円**
    - https://www.switch-science.com/products/9907

- 千石電商 ... **税込み1900円**
    - https://www.sengoku.co.jp/mod/sgk_cart/detail.php?code=EEHD-6H6N

### Pimoroni Pico Plus 2
Flash 16MB, PSRAM 8MB

- スイッチサイエンス ... **税込み2651円**
    - https://www.switch-science.com/products/9906?_pos=1&_sid=caf054cc3&_ss=r

- 千石電商 ... **税込み2500円**
    - https://www.sengoku.co.jp/mod/sgk_cart/detail.php?code=EEHD-6H6P

## 開発環境(Arduino IDE)
- `基本設定`から👇下記URLを追加

```
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
```

- `ボードマネージャ`から`Raspberry Pi Pico/RP2040 by Earle F Philhower, III`をダウンロード or 更新