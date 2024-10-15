/**
 * @file app_main_core1.cpp
 * @author Chimipupu（https://github.com/Chimipupu）
 * @brief RP2350 Core1 アプリ
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "app_main_core1.hpp"
// #include "app_neopixel.hpp"
#include "app_oled.hpp"
// #include "app_util.hpp"s

#include "cpm.hpp"

#define SLAVE_FOUNED        0x00
#define MAIN_DELAY          300

# if 0
static void i2c_scan(void);

static void i2c_scan(void)
{
    uint8_t res;
    uint8_t addr;
    uint8_t nDevices;
    uint8_t cnt;
    uint8_t addr_buf[128] = {0};
    uint8_t *p_buf;

    nDevices = 0;
    p_buf = &addr_buf[0];
    Serial.println("Scanning...");

    for(addr = 1; addr < 127; addr++ )
    {
        Wire1.beginTransmission(addr);
        res = Wire1.endTransmission();

        if (res == SLAVE_FOUNED)
        {
            *p_buf = addr;
            p_buf++;
            nDevices++;
        }else{
            // NOP
        }
    }

    if (nDevices == 0){
        Serial.println("No I2C Slave Found\n");
    }else{
        Serial.print("I2C Slave = ");
        Serial.print(nDevices);
        Serial.print("\n");

        for(cnt = 0; cnt < nDevices; cnt++)
        {
            Serial.print("0x");
            if (addr_buf[cnt] < 16)
            {
                Serial.print("0");
            }
            Serial.print(addr_buf[cnt],HEX);
            Serial.print(" ");
        }
        Serial.print("\n");
    }
}
#endif

void app_main_init_core1(void)
{
    // OLED 初期化
    // app_oled_init();
    // app_oled_test();
    DEBUG_PRINTF("[Core1] ... Init\n");
}

void app_main_core1(void)
{
# if 0
    static uint8_t s_val = 0;

    DEBUG_PRINTF("[Core1] ... This is Application Core\n");

    i2c_scan();

    // LEDのトグル
    digitalWrite(OB_LED_PIN, s_val);
    s_val = !s_val;

    //(TBD) OLED メイン
    app_oled_test();
#endif
    // モニタプログラム
    cpm_main();

    // SW_DELAY_MS(MAIN_DELAY);
}