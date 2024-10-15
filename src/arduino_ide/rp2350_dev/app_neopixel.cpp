/**
 * @file app_neopixel.cpp
 * @author Chimipupu（https://github.com/Chimipupu）
 * @brief NeoPixelアプリ
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "app_neopixel.hpp"

#define MAX_LIGHT               32 // NEOPIEXLの最大輝度
#define MAX_BRIGHTNESS          32
#define LED_COLOR_ON_TIMER      100
#define RGBLED_NUM      1

Adafruit_NeoPixel pixels(RGBLED_NUM,
                        RGBLED_PIN,
                        NEO_GRB + NEO_KHZ800);

void app_neopixel_init(void)
{
    pixels.begin();
    pixels.clear();
    pixels.show();
}

void app_neopixel_main(void)
{
    static uint8_t s_red = 0;
    static uint8_t s_green = 0;
    static uint8_t s_blue = 0;
    uint8_t cnt, red,green,blue;

    red = MAX_BRIGHTNESS;
    for(green = 0; green < MAX_BRIGHTNESS; green++)
    {
        for(cnt = 0; cnt < RGBLED_NUM; cnt++)
        {
            pixels.setPixelColor(cnt, pixels.Color(s_red, s_green, s_blue));
            pixels.show();
        }
        s_green++;
        s_red--;
        delay(LED_COLOR_ON_TIMER);
    }
    s_red = 0;

    for(blue = 0; blue < MAX_BRIGHTNESS; blue++)
    {
        for(cnt = 0; cnt < RGBLED_NUM; cnt++)
        {
            pixels.setPixelColor(cnt, pixels.Color(s_red, s_green, s_blue));
            pixels.show();
        }
        s_blue++;
        s_green--;
        delay(LED_COLOR_ON_TIMER);
    }
    s_green = 0;

    for(red = 0; red < MAX_BRIGHTNESS; red++)
    {
        for(cnt = 0; cnt < RGBLED_NUM; cnt++)
        {
            pixels.setPixelColor(cnt, pixels.Color(s_red, s_green, s_blue));
            pixels.show();
        }
        s_red++;
        s_blue--;
        delay(LED_COLOR_ON_TIMER);
    }
    s_blue = 0;
}