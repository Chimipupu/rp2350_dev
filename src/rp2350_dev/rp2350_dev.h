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

// I2C
#define I2C_PORT        i2c0
#define I2C_0_PORT      i2c0
#define I2C_0_SDA       4
#define I2C_0_SCL       5
#define I2C_1_PORT      i2c1
#define I2C_1_SDA       6
#define I2C_1_SCL       7

// SPI
#define SPI_PORT        spi1
#define PIN_MISO        12
#define PIN_CS          13
#define PIN_SCK         10
#define PIN_MOSI        11

// UART
#define UART_ID         uart1
#define BAUD_RATE       115200
#define UART_TX_PIN     0
#define UART_RX_PIN     1

#endif // RP2350_DEV_H