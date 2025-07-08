/**
 * @file mcu_board_def.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief 基板定義ヘッダ
 * @version 0.1
 * @date 2025-07-08
 * 
 * @copyright Copyright (c) 2025 Chimipupu(https://github.com/Chimipupu)
 * 
 */
#ifndef MCU_BOARD_DEF_H
#define MCU_BOARD_DEF_H

#include "mcu_util.h"
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

// #define MCU_BOARD_PICO2                             // Raspberry Pi Pico 2
#define MCU_BOARD_PICO2W                            // Raspberry Pi Pico 2 W
// #define MCU_BOARD_WEACTRP2350B                      // WeActStudio RP2350B

// [基板固有定義]
#if defined(MCU_BOARD_PICO2W)
#define MCU_BOARD_LED_PIN    CYW43_WL_GPIO_LED_PIN   // Raspberry Pi Pico 2 WのLEDピン
void cyw43_led_tgl(void);
#else
#define MCU_BOARD_LED_PIN 25                        // LEDピン
#endif

#if defined(MCU_BOARD_WEACTRP2350B)
#define MCU_BOARD_BTN_PIN 23                        // WeActStudio RP2350Bのボタンピン
#endif // MCU_BOARD_WEACTRP2350B

#define MCU_FLASH_SIZE    4                         // RP2350のフラッシュサイズ (4MB)
#define MCU_RAM_SIZE      520                       // RP2350のSRAMサイズ (520KB)

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

#endif // MCU_BOARD_DEF_H