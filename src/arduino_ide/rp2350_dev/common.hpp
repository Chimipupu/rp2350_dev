/**
 * @file common.hpp
 * @author Chimipupu（https://github.com/Chimipupu）
 * @brief RP2350評価プログラムの評価プログラム用 共通ヘッダー
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef COMMON_HPP
#define COMMON_HPP

#include <Arduino.h>
#include <stdint.h>
#include "rp2350_reg.hpp"

// typedef enum {
//     CPU_CORE_0 = 0x00,
//     CPU_CORE_1,
//     CPU_CORE_UNKNOWN = 0xFF
// } E_CPU_CORE;

#define DUAL_CORE_BUILD

// Use Pin Define
#define RGBLED_PIN          23   // GPIO 23
#define BUTTON_PIN          24   // GPIO 24
#define OB_LED_PIN          25   // GPIO 25
#define I2C_SDA             6    // GPIO 6
#define I2C_SCL             7    // GPIO 7

#define core0_init          setup
#define core0_main          loop
#define core1_init          setup1
#define core1_main          loop1
#define DEBUG_PRINTF        Serial.printf
#define DEBUG_PRINT_INIT    Serial.begin

#define SW_DELAY_MS         delay

#define GPIO_PORT_DIR       pinMode
#define GPIO_OUTPUT         digitalWrite

#define ON                  0x00    // ON ... Active Low
#define OFF                 0x01

// I2Cスレーブ
#define QMC5883_I2C_ADDR    0x0D  // HMC5883のパチモン
#define HMC5883_I2C_ADDR    0x1E
#define MPU9250_I2C_ADDR    0x68
#define MPU6050_I2C_ADDR    0x69  // ADOピンをHIGH = 0x69(Low/HiZ = 0x68)
#define OLED_I2C_ADDR       0x3C

extern "C"
{
    static inline void GPIO_TOGGLE(uint8_t port)
    {
        static bool s_port_val = OFF;

        GPIO_OUTPUT(port, s_port_val);
        s_port_val = !s_port_val;
    }
} /* extern "C" */

#endif /* COMMON_HPP */