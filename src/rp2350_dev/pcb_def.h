/**
 * @file pcb_def.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief 基板定義ヘッダ
 * @version 0.1
 * @date 2025-07-27
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#ifndef PCB_DEF_H
#define PCB_DEF_H

#include "rp2350_util.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "hardware/sha256.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/interp.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"

// [基板定義]
// #define PCB_PICO2                             // Raspberry Pi Pico 2
// #define PCB_PICO2W                            // Raspberry Pi Pico 2 W
// #define PCB_WEACT_RP2350B                     // WeAct RP2350B
#define PCB_WEACT_RP2350A_V10                 // WeAct RP2350A-V10
// #define PCB_WEACT_RP2350A_V20                 // WeAct RP2350A-V20
// #define PCB_RP2350_PIZERO                     // Waveshare RP2350-PiZero

// [基板名]
#pragma once
#if defined(PCB_PICO2)
    #define PCB_NAME "Pico2"
#elif defined(PCB_PICO2W)
    #define PCB_NAME "Pico2W"
#elif defined(PCB_WEACT_RP2350B)
    #define PCB_NAME "WeAct RP2350B"
#elif defined(PCB_WEACT_RP2350A_V10)
    #define PCB_NAME "WeAct RP2350A-V10"
#elif defined(PCB_WEACT_RP2350A_V20)
    #define PCB_NAME "WeAct RP2350A-V20"
#elif defined(PCB_RP2350_PIZERO)
    #define PCB_NAME "RP2350-PiZero"
#endif

// 基板を定義してなかっｔらビルドエラーにする
#if !defined(PCB_NAME)
    #error PCB is not defined. Please define the board type. at mcu_board_def
#endif //PCB_NAME

// [基板固有定義]
#if defined(PCB_RP2350_PIZERO)
    // USB
    #define PIO_USB_DP_PIN  28                          // RP2350-PiZeroのPIOUSBのD+ピン
    #define PIO_USB_DM_PIN  29                          // RP2350-PiZeroのPIOUSBのD-ピン

    // DVI(HDMI)
    #define DVI_D2P_PIN     32                          // RP2350-PiZeroのDVIのD2+ピン
    #define DVI_D2N_PIN     33                          // RP2350-PiZeroのDVIのD2+ピン
    #define DVI_D1P_PIN     34                          // RP2350-PiZeroのDVIのD2+ピン
    #define DVI_D1N_PIN     35                          // RP2350-PiZeroのDVIのD2+ピン
    #define DVI_D0P_PIN     36                          // RP2350-PiZeroのDVIのD2+ピン
    #define DVI_D0N_PIN     37                          // RP2350-PiZeroのDVIのD2+ピン
    #define DVI_CLK_P_PIN   38                          // RP2350-PiZeroのDVIのCLK+ピン
    #define DVI_CLK_N_PIN   39                          // RP2350-PiZeroのDVIのCLK-ピン
    #define DVI_SDA_PIN     44                          // RP2350-PiZeroのDVIのSDAピン
    #define DVI_SCL_PIN     45                          // RP2350-PiZeroのDVIのSCLピン
    #define DVI_CEC_PIN     46                          // RP2350-PiZeroのDVIのCECピン

    // SDカード
    #define SD_SCK_PIN      30                          // RP2350-PiZeroのSDカードのSCKピン
    #define SD_MOSI_PIN     31                          // RP2350-PiZeroのSDカードのMOSIピン
    #define SD_MISO_PIN     40                          // RP2350-PiZeroのSDカードのMISOピン
    #define SD_D1_PIN       41                          // RP2350-PiZeroのSDカードのD1ピン
    #define SD_D2_PIN       42                          // RP2350-PiZeroのSDカードのD2ピン
    #define SD_CS_PIN       43                          // RP2350-PiZeroのSDカードのCSピン

    // PSRAM
    #define PSRAM_CS_PIN    47                          // RP2350-PiZeroのPSRAMのCSピン
#endif //PCB_RP2350_PIZERO

#if defined(PCB_PICO2W)
    #define PCB_LED_PIN    CYW43_WL_GPIO_LED_PIN   // Raspberry Pi Pico 2 WのLEDピン
    void cyw43_led_tgl(void);
#elif defined(PCB_WEACT_RP2350A_V10)
    #define PCB_LED_PIN    25   // LEDピン
    #define PCB_LED_2_PIN  24   // LEDピン
#else
    #define PCB_LED_PIN    25   // LEDピン
#endif

#if defined(PCB_WEACT_RP2350A_V10) || defined(PCB_WEACT_RP2350B)
    #define PCB_BTN_PIN 23                        // WeAct RP2350A-V10とWeAct RP2350Bのボタンピン
#endif //PCB_WEACT_RP2350A_V10

// [メモリ関連]
#if defined(PCB_WEACT_RP2350B) || defined(PCB_RP2350_PIZERO)
    #define MCU_FLASH_SIZE    16                        // フラッシュサイズ (16MB)
    #define PCB_PSRAM_ENABLE
    #define MCU_PSRAM_SIZE    8                         // PSRAMサイズ (8MB)
#else
    #define MCU_FLASH_SIZE    4                         // RP2350のフラッシュサイズ (4MB)
#endif //PCB_WEACT_RP2350B
#define MCU_RAM_SIZE      520                       // RP2350のSRAMサイズ (520KB)

// [PIO関連]
// #define RPI_PIO_USE

// [NeoPixel]
#define PCB_NEOPIXEL
#ifdef PCB_NEOPIXEL
    // #define NEOPIXEL_LED_CNT         4   // NeoPixelの数
    #define NEOPIXEL_LED_CNT         8   // NeoPixelの数
    // #define NEOPIXEL_LED_CNT         16  // NeoPixelの数
    // #define NEOPIXEL_LED_CNT         64  // NeoPixelの数
    #define PCB_NEOPIXEL_PIN   15  // NeoPixelのデータピン
#endif //PCB_NEOPIXEL

// [タイマ関連]
// #define TIMER_ALARM_IRQ_ENABLE

// [WDT関連]
// #define _WDT_ENABLE_                             // WDT有効マクロ
#ifdef _WDT_ENABLE_
    #define _WDT_OVF_TIME_MS_ 3000                      // WDTが鳴く時間(ms)
#endif // _WDT_ENABLE_

// [I2C関連]
#define I2C_0_PORT             i2c0
#define I2C_1_PORT             i2c1
#define I2C_PORT               I2C_0_PORT
#define I2C_BIT_RATE_100KHZ    (100 * 1000)         // RP2350のI2C Standard Mode  ... 100kHz
#define I2C_BIT_RATE_400KHZ    (400 * 1000)         // RP2350のI2C Fast Mode      ... 400kHz
#define I2C_BIT_RATE_1MHZ      (1000 * 1000)        // RP2350のI2C Fast Mode Plus ... 1MHz
#define I2C_BIT_RATE            I2C_BIT_RATE_100KHZ
#define I2C_0_SDA               16                  // I2C0 SDA (GPIO 16)
#define I2C_0_SCL               17                  // I2C0 SCL (GPIO 17)
#define I2C_1_SDA               18                  // I2C1 SDA (GPIO 18)
#define I2C_1_SCL               19                  // I2C1 SCL (GPIO 19)

// [SPI関連]
#define SPI_0_PORT              spi1
#define SPI_1_PORT              spi1
#define SPI_PORT                SPI_1_PORT
#define SPI_BIT_RATE_1MHZ       (1 * 1000000)
#define SPI_BIT_RATE_10MHZ      (10 * 1000000)
#define SPI_BIT_RATE_20MHZ      (10 * 1000000)
#define SPI_BIT_RATE_50MHZ      (50 * 1000000)
#define SPI_BIT_RATE_60MHZ      (60 * 1000000)
#define SPI_BIT_RATE_62MHZ      (62 * 1000000)
#define SPI_BIT_RATE_62P5MHZ    (62.5 * 1000000)    // RP2350のSPIの最大レート 62.5MHz
#define SPI_BIT_RATE            SPI_BIT_RATE_1MHZ
#define SPI_0_CS                9                   // SPI0 CS   (GPIO 9)
#define SPI_0_SCK               6                   // SPI0 SCK  (GPIO 6)
#define SPI_0_MISO              8                   // SPI0 MISO (GPIO 8)
#define SPI_0_MOSI              7                   // SPI0 MOSI (GPIO 7)
#define SPI_1_CS                13                  // SPI1 CS   (GPIO 13)
#define SPI_1_SCK               10                  // SPI1 SCK  (GPIO 10)
#define SPI_1_MISO              12                  // SPI1 MISO (GPIO 12)
#define SPI_1_MOSI              11                  // SPI1 MOSI (GPIO 11)

// UART
#define UART_0_PORT             uart0
#define UART_1_PORT             uart1
#define UART_PORT               UART_0_PORT
#define UART_BAUD_RATE_9600     9600
#define UART_BAUD_RATE_14400    14400
#define UART_BAUD_RATE_19200    19200
#define UART_BAUD_RATE_38400    38400
#define UART_BAUD_RATE_57600    57600
#define UART_BAUD_RATE_115200   115200
#define UART_BAUD_RATE_230400   230400
#define UART_BAUD_RATE_460800   460800
#define UART_BAUD_RATE_921600   921600
#define UART_BAUD_RATE          UART_BAUD_RATE_921600
#define UART_0_TX               0                   // UART0 TX (GPIO 0)
#define UART_0_RX               1                   // UART0 TX (GPIO 1)
#define UART_1_TX               4                   // UART1 TX (GPIO 4)
#define UART_1_RX               5                   // UART1 TX (GPIO 5)

#define GPIO_PIN_0      0
#define GPIO_PIN_1      1
#define GPIO_PIN_2      2
#define GPIO_PIN_3      3
#define GPIO_PIN_4      4
#define GPIO_PIN_5      5
#define GPIO_PIN_6      6
#define GPIO_PIN_7      7
#define GPIO_PIN_8      8
#define GPIO_PIN_9      9
#define GPIO_PIN_10     10
#define GPIO_PIN_11     11
#define GPIO_PIN_12     12
#define GPIO_PIN_13     13
#define GPIO_PIN_14     14
#define GPIO_PIN_15     15
#define GPIO_PIN_16     16
#define GPIO_PIN_17     17
#define GPIO_PIN_18     18
#define GPIO_PIN_19     19
#define GPIO_PIN_20     20
#define GPIO_PIN_21     21
#define GPIO_PIN_22     22
#define GPIO_PIN_23     23
#define GPIO_PIN_24     24
#define GPIO_PIN_25     25
#define GPIO_PIN_26     26
#define GPIO_PIN_27     27
#define GPIO_PIN_28     28
#define GPIO_PIN_29     29
#define GPIO_PIN_30     30
#define GPIO_PIN_31     31
#define GPIO_PIN_32     32
#define GPIO_PIN_33     33
#define GPIO_PIN_34     34
#define GPIO_PIN_35     35
#define GPIO_PIN_36     36
#define GPIO_PIN_37     37
#define GPIO_PIN_38     38
#define GPIO_PIN_39     39
#define GPIO_PIN_40     40
#define GPIO_PIN_41     41
#define GPIO_PIN_42     42
#define GPIO_PIN_43     43
#define GPIO_PIN_44     44
#define GPIO_PIN_45     45
#define GPIO_PIN_46     46
#define GPIO_PIN_47     47
#define GPIO_PIN_48     48
#define GPIO_PIN_49     49
#define GPIO_PIN_50     50

#define ADC_CH_0        0
#define ADC_CH_1        1
#define ADC_CH_2        2
#define ADC_CH_3        3   // Pico 2では、GPIO29がVSYSに接続されてる
#define ADC_CH_4        4   // RP2350A(QFN-60)では、CPUコアの温度センサに接続
#define ADC_CH_5        5   // RP2350B(QFN-80)専用
#define ADC_CH_6        6   // RP2350B(QFN-80)専用
#define ADC_CH_7        7   // RP2350B(QFN-80)専用
#define ADC_CH_8        8   // RP2350B(QFN-80)では、CPUコアの温度センサに接続

#define VREF_VOLTAGE    3.3f
#define VBUS_VOLTAGE    5.0f

#define PORT_OFF        0x00
#define PORT_ON         0x01

#endif // PCB_DEF_H