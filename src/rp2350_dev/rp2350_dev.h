#ifndef RP2350_DEV_H
#define RP2350_DEV_H

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

#define UART_ID         uart1
#define BAUD_RATE       115200
#define UART_TX_PIN     4
#define UART_RX_PIN     5

#define SPI_PORT        spi0
#define PIN_MISO        16
#define PIN_CS          17
#define PIN_SCK         18
#define PIN_MOSI        19

#define I2C_PORT        i2c0
#define I2C_SDA         8
#define I2C_SCL         9

#endif // RP2350_DEV_H