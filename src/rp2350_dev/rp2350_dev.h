/**
 * @file rp2350_dev.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief Pico SDK生成ファイル
 * @version 0.1
 * @date 2025-06-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef RP2350_DEV_H
#define RP2350_DEV_H

#include "common.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/interp.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"

// #define _WDT_ENABLE_              // WDT有効マクロ
#ifdef _WDT_ENABLE_
    #define _WDT_OVF_TIME_MS_    3000 // WDTが鳴く時間(ms)
#endif // _WDT_ENABLE_

// [I2C関連]
#define I2C_0_PORT      i2c0
#define I2C_1_PORT      i2c1
#define I2C_PORT        I2C_0_PORT
#define I2C_0_SDA       16    // I2C0 SDA (GPIO 16)
#define I2C_0_SCL       17    // I2C0 SCL (GPIO 17)
#define I2C_1_SDA       18    // I2C1 SDA (GPIO 18)
#define I2C_1_SCL       19    // I2C1 SCL (GPIO 19)

// [SPI関連]
#define SPI_0_PORT      spi1
#define SPI_1_PORT      spi1
#define SPI_PORT        SPI_1_PORT
#define SPI_0_CS        9     // SPI0 CS   (GPIO 9)
#define SPI_0_SCK       6     // SPI0 SCK  (GPIO 6)
#define SPI_0_MISO      8     // SPI0 MISO (GPIO 8)
#define SPI_0_MOSI      7     // SPI0 MOSI (GPIO 7)
#define SPI_1_CS        13    // SPI1 CS   (GPIO 13)
#define SPI_1_SCK       10    // SPI1 SCK  (GPIO 10)
#define SPI_1_MISO      12    // SPI1 MISO (GPIO 12)
#define SPI_1_MOSI      11    // SPI1 MOSI (GPIO 11)

// UART
#define UART_0_PORT     uart0
#define UART_1_PORT     uart1
#define UART_PORT       UART_0_PORT
#define UART_BAUD_RATE  115200
#define UART_0_TX       0    // UART0 TX (GPIO 0)
#define UART_0_RX       1    // UART0 TX (GPIO 1)
#define UART_1_TX       4    // UART1 TX (GPIO 4)
#define UART_1_RX       5    // UART1 TX (GPIO 5)

#endif // RP2350_DEV_H