/**
 * @file pcb_def.h
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief RP2350基板定義
 * @version 0.1
 * @date 2026-09-06
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */
#ifndef PCB_DEF_HPP
#define PCB_DEF_HPP

#include <stdint.h>
#include <string.h>

#define PCB_RPI_PICO_2

// Raspberry Pi Pico 2
#ifdef PCB_RPI_PICO_2
    #define OB_LED_PIN               25  // 緑色LED (GPIO 25)

    // I2S
    #define I2S_USE                  1
#if defined(I2S_USE)
    #define I2S_DOUT_PIN             22
    #define I2S_LRCLK_PIN            21
    #define I2S_BCLK_PIN             20
#endif

#if 0
    #define BUTTON_PIN               24  // ボタン (GPIO 24)

    #define RGBLED_PIN               23  // Neopixel (GPIO 23)
    #define RGBLED_NUM               1   // RGBLEDの数
    #define RGBLED_MAX_BRIGHTNESS    32  // RGBLEDの最大輝度
    #define RGBLED_COLOR_ON_TIMER    100 // RGBLEDの1色の表示時間
#endif

    // I2C
    #define I2C_SDA_PIN              4   // I2C0 SDA
    #define I2C_SCL_PIN              5   // I2C0 SCL
    #define I2C_1_SDA_PIN            6   // I2C1 SDA
    #define I2C_1_SCL_PIN            7   // I2C1 SCL

    // SPI
    #define SPI_CS_PIN               1   // SPI0 CS
    #define SPI_SCK_PIN              2   // SPI0 CLK
    #define SPI_MISO_PIN             0   // SPI0 MISO
    #define SPI_MOSI_PIN             3   // SPI0 MOSI

    // UART
    #define UART_TX_PIN              12  // UART0 TX
    #define UART_RX_PIN              13  // UART0 RX
#endif // PCB_RPI_PICO_2

void pcb_info(void);
void pcb_gpio_init(void);
void pcb_i2c_init(void);
void pcb_uart_init(void);
unsigned int DBG_PRINTF(const char *p_fmt, ...);

#endif // PCB_DEF_HPP