/**
 * @file app_main_core0.cpp
 * @author Chimipupu（https://github.com/Chimipupu）
 * @brief RP2350 Core0 アプリ
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "app_main_core0.hpp"
#include "app_neopixel.hpp"
// #include "app_oled.hpp"
// #include "app_util.hpp"

#define MAIN_DELAY      200

static void gpio_init(void);

static void gpio_init(void)
{
    // UART Init
    Serial.begin(115200);
    // delay(1000);

    // GPIO Init
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(OB_LED_PIN, OUTPUT);
    digitalWrite(OB_LED_PIN, LOW);
}

void app_main_init_core0(void)
{
    // UART 初期化
    DEBUG_PRINT_INIT(DEBUG_UART_BAUDRATE);

    // GPIO 初期化
    gpio_init();

    // NeoPicel 初期化
    app_neopixel_init();

    // OLED 初期化
    // app_oled_init();

    DEBUG_PRINTF("[Core0] ... Init\n");
}

void app_main_core0(void)
{
    // DEBUG_PRINTF("[Core0] ... This is Driver Core\n");

    // NeoPixel メイン
    app_neopixel_main();

    // OLED メイン
    // (TBD)
    // app_oled_test();

    SW_DELAY_MS(MAIN_DELAY);
}