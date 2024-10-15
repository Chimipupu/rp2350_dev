/**
 * @file app_oled.hpp
 * @author Chimipupu（https://github.com/Chimipupu）
 * @brief OLEDアプリ
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#if 0
#ifndef APP_OLED_HPP
#define APP_OLED_HPP

#include "common.hpp"
#include <Wire.h>

#define LGFX_USE_V1
#define LGFX_AUTODETECT
#include <LovyanGFX.hpp>

#define I2C_NORMAL_FREQ  100000 // I2C @100KHz
#define I2C_FAST_FREQ    400000 // I2C Fast Mode @400KHz
// #define I2C_FREQ         I2C_NORMAL_FREQ
#define I2C_FREQ         I2C_FAST_FREQ

#define SCREEN_WIDTH   128 // OLED display width, in pixels
#define SCREEN_HEIGHT  64 // OLED display height, in pixels
#define OLED_RESET     -1

#define OLED_BACK_COLOR     TFT_BLACK
#define OLED_TXT_COLOR      TFT_WHITE
#define OLED_TXT_SIZE       1

// #define LCD_SSD1306
#define LCD_SH110x

#ifdef LCD_SSD1306
// 0.96インチのOLED用
class LGFX_SSD1306 : public lgfx::LGFX_Device
{
    lgfx::Panel_SSD1306   _panel_instance;
    lgfx::Bus_I2C   _bus_instance;

    public:
        LGFX_SSD1306()
        {
            {
                auto cfg = _bus_instance.config();
                cfg.i2c_port    = 1;                // 使用するI2C (0 or 1)
                cfg.freq_write  = I2C_FREQ;         // Write @100KHz or 400KHz
                cfg.freq_read   = I2C_FREQ;         // read  @100KHz or 400KHz
                cfg.pin_sda     = I2C_SDA;          // SDAピン
                cfg.pin_scl     = I2C_SCL;          // SCLピン
                cfg.i2c_addr    = OLED_I2C_ADDR;    // スレーブアドレス
                _bus_instance.config(cfg);
                _panel_instance.setBus(&_bus_instance);
            }

            {
                auto cfg = _panel_instance.config();
                cfg.memory_width  = SCREEN_WIDTH;     // LCDのの幅
                cfg.memory_height =  SCREEN_HEIGHT;   // LCDの高さ
                _panel_instance.config(cfg);
                setPanel(&_panel_instance);
            }
            setPanel(&_panel_instance);
        }
};
#endif /* LCD_SSD1306 */

#ifdef LCD_SH110x
// 1.3インチのOLED用
class LGFX_SH110x : public lgfx::LGFX_Device
{
    lgfx::Panel_SH110x  _panel_instance;
    lgfx::Bus_I2C   _bus_instance;

    public:
        LGFX_SH110x()
        {
            {
                auto cfg = _bus_instance.config();
                cfg.i2c_port    = 1;                // 使用するI2C (0 or 1)
                cfg.freq_write  = I2C_FREQ;         // Write @100KHz or 400KHz
                cfg.freq_read   = I2C_FREQ;         // read  @100KHz or 400KHz
                cfg.pin_sda     = I2C_SDA;          // SDAピン
                cfg.pin_scl     = I2C_SCL;          // SCLピン
                cfg.i2c_addr    = OLED_I2C_ADDR;    // スレーブアドレス
                _bus_instance.config(cfg);
                _panel_instance.setBus(&_bus_instance);
            }

            {
                // auto cfg = _panel_instance.config();
                // cfg.memory_width  = SCREEN_WIDTH;     // LCDのの幅
                // cfg.memory_height =  SCREEN_HEIGHT;   // LCDの高さ
                // _panel_instance.config(cfg);
                // setPanel(&_panel_instance);
            }
            setPanel(&_panel_instance);
        }
};
#endif /* LCD_SH110x */

void app_oled_init(void);
void app_oled_test(void);

#endif /* APP_OLED_HPP */
#endif