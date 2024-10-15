/**
 * @file app_oled.cpp
 * @author Chimipupu（https://github.com/Chimipupu）
 * @brief OLEDアプリ
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#if 0
#include "app_oled.hpp"
#include "math_uc.hpp"

#define MATH_PI_CALC_TIME 3
#define FIBONACCI_N 14

typedef enum
{
    ROTATION_A = 0, // USB (未調査)
    ROTATION_B,     // USB (未調査)
    ROTATION_C,     // USB 左
    ROTATION_D,     // USB 左 画面反転
};

#ifdef LCD_SSD1306
static LGFX_SSD1306 lcd;   // 0.96インチのOLED用
#endif /* LCD_SSD1306 */

#ifdef LCD_SH110x
static LGFX_SH110x lcd;    // 1.3インチのOLED用
#endif /* LCD_SH110x */

static LGFX_Sprite sprite(&lcd);

void app_oled_init(void)
{
    pinMode(I2C_SDA, INPUT_PULLUP);	//	SDA
	pinMode(I2C_SCL, INPUT_PULLUP);	//	SCL
    Wire1.setSDA(I2C_SDA);
    Wire1.setSCL(I2C_SCL);
    Wire1.begin();

    lcd.init();
    lcd.setRotation(ROTATION_C);

    // sprite.setColorDepth(8);
    // sprite.setTextWrap(false);  // 改行をしない
    sprite.setTextSize(OLED_TXT_SIZE);
    sprite.createSprite(lcd.width(), lcd.height());
}

void app_oled_test(void)
{
    uint32_t i, cnt;
    volatile double c;
    volatile double pi;
    volatile double phi;
    volatile double napier;
    volatile uint32_t a_fib[FIBONACCI_N] = {0};
    volatile double result;
    static uint32_t s_dbg_val = 0;
    static double s_val = 0;

    // 英数字 表示テスト
    sprite.fillScreen(OLED_BACK_COLOR);
    sprite.setTextColor(OLED_TXT_COLOR);
    sprite.setCursor(0, 0);
    sprite.setFont(&fonts::Font0);
    sprite.printf("RP2350 Dual Core uC\n");
    sprite.printf("133MHz ARM Cortex-M0+\n");
    sprite.printf("ROM 2MB, RAM 264KB\n");
    sprite.printf("Very Good! ARM uC!\n");
    s_val = MATH_PI;
    sprite.printf("pi=%.015lf\n", s_val);
    s_val = s_val * 2;
    sprite.printf("2pi=%.015lf\n", s_val);
    sprite.pushSprite(0, 0);
    delay(3000);

#if 1
    // 日本語 表示テスト
    sprite.fillScreen(OLED_BACK_COLOR);
    sprite.setTextColor(OLED_TXT_COLOR);
    sprite.setCursor(0, 0);
    sprite.pushSprite(0, 0);
    // sprite.setFont(&fonts::lgfxJapanGothic_12); // ゴシック体 12px
    // sprite.printf("[ゴシック体 12px]\n");
    sprite.setFont(&fonts::efontJA_12);         // eFont 12px
    sprite.printf("[eFont 12px]\n");
    sprite.printf("日本語は12pxが下限\n");
    sprite.printf("ラズベリーパイ ピコ\n");
    sprite.printf("あいうえおアイウエオ\n");
    sprite.printf("電子工作 最&高!!!\n");
    sprite.pushSprite(0, 0);
    delay(3000);

    // 計算結果表示テスト
    sprite.fillScreen(OLED_BACK_COLOR);
    sprite.setTextColor(OLED_TXT_COLOR);
    sprite.setCursor(0, 0);

    // tan(355/226)の計算（※期待値:-7497258.185...）
    result = math_calc_accuracy();
    sprite.printf("res=%.4f\n", result);

    // 円周率π
    for(i=1; i<=MATH_PI_CALC_TIME; i++)
    {
        pi = math_pi_calc(i);
    }
    sprite.printf("pi=%.15f\n", pi);

    // ネイピアe
    napier = math_napier_calc();
    sprite.printf("e=%.15f\n", napier);

    // 黄金比φ
    phi = math_goldenratio_calc();
    sprite.printf("phi=%.15f\n", phi);

    // フィボナッチ数列
    // sprite.printf("Fn(n = %d)\n", FIBONACCI_N);
    for(i = 1; i<= FIBONACCI_N; i++)
    {
        a_fib[i-1] = math_fibonacci_calc(i);
        sprite.print(a_fib[i-1]);
        sprite.print(",");
    }
    sprite.print("\n");

    // (DEBUG)
    sprite.printf("dbg = %d\n",s_dbg_val);
    s_dbg_val++;

    sprite.pushSprite(0, 0);
    delay(3000);
#endif
}
#endif