#include "app_main.h"
#include "dbg_com.h"
#include <stdarg.h>
#include <math.h>
#include "pico/version.h"
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"

static void pico_sdk_version_print(void);

static void pico_sdk_version_print(void)
{
    printf("Pico SDK version: %d.%d.%d\n",
        PICO_SDK_VERSION_MAJOR,
        PICO_SDK_VERSION_MINOR,
        PICO_SDK_VERSION_REVISION);
}

void int_add_test(void)
{
    volatile uint32_t val, i = 0;

    for (i = 0; i < TEST_LOOP_CNT; i++)
    {
        val += 1;
    }
}

void int_sub_test(void)
{
    volatile uint32_t i = 0;
    volatile uint32_t val = TEST_LOOP_CNT;

    for (i = 0; i < TEST_LOOP_CNT; i++)
    {
        val -= 1;
    }
}

void int_mul_test(void)
{
    volatile uint32_t i = 0;
    volatile uint32_t val = 1;

    for (i = 0; i < TEST_LOOP_CNT; i++)
    {
        val = val * 1;
    }
}

void int_div_test(void)
{
    volatile uint32_t i = 0;
    volatile uint32_t val = 1;

    for (i = 0; i < TEST_LOOP_CNT; i++)
    {
        val = val / 1;
    }
}

void float_add_test(void)
{
    volatile float val = 0.0f;
    volatile float inc = 1.0f;
    volatile uint32_t i = 0;

    for (i = 0; i < TEST_LOOP_CNT; i++)
    {
        val = val + inc;
    }
}

void float_sub_test(void)
{
    volatile float val = TEST_LOOP_CNT;
    volatile float dec = 1.0f;
    volatile uint32_t i = 0;

    for (i = 0; i < TEST_LOOP_CNT; i++)
    {
        val = val - dec;
    }
}

void float_mul_test(void)
{
    volatile float val = 1.0f;
    volatile float mul = 1.0f;
    volatile uint32_t i = 0;

    for (i = 0; i < TEST_LOOP_CNT; i++)
    {
        val = val * mul;
    }
}

void float_div_test(void)
{
    volatile float val = 1.0f;
    volatile float div = 1.0f;
    volatile uint32_t i = 0;

    for (i = 0; i < TEST_LOOP_CNT; i++)
    {
        val = val / div;
    }
}

void double_add_test(void)
{
    volatile double val = 0.0;
    volatile double inc = 1.0;
    volatile uint32_t i = 0;

    for (i = 0; i < TEST_LOOP_CNT; i++)
    {
        val = val + inc;
    }
}

void double_sub_test(void)
{
    volatile double val = TEST_LOOP_CNT;
    volatile double dec = 1.0;
    volatile uint32_t i = 0;

    for (i = 0; i < TEST_LOOP_CNT; i++)
    {
        val = val - dec;
    }
}

void double_mul_test(void)
{
    volatile double val = 1.0;
    volatile double mul = 1.0;
    volatile uint32_t i = 0;

    for (i = 0; i < TEST_LOOP_CNT; i++)
    {
        val = val * mul;
    }
}

void double_div_test(void)
{
    volatile double val = 1.0;
    volatile double div = 1.0;
    volatile uint32_t i = 0;

    for (i = 0; i < TEST_LOOP_CNT; i++)
    {
        val = val / div;
    }
}

void trig_functions_test(void)
{
    volatile double angle = 45.0;  // 45 degrees
    volatile double rad = angle * M_PI / 180.0;  // convert to radians

    printf("\nTrigonometric Functions Test:\n");
    printf("Angle: %.1f degrees (%.6f radians)\n", angle, rad);
    printf("sin(%.1f°) = %.6f\n", angle, sin(rad));
    printf("cos(%.1f°) = %.6f\n", angle, cos(rad));
    printf("tan(%.1f°) = %.6f\n", angle, tan(rad));
}

void atan2_test(void)
{
    volatile double x = 1.0;
    volatile double y = 1.0;

    printf("\nAtan2 Test:\n");
    printf("atan2(%.1f, %.1f) = %.6f radians (%.1f degrees)\n",
           y, x, atan2(y, x), atan2(y, x) * 180.0 / M_PI);
}

void tan_355_226_test(void)
{
    volatile double result = tan(355.0 / 226.0);

    printf("\nTan(355/226) Test:\n");
    printf("tan(355/226) = %.6f\n", result);
}

void inverse_sqrt_test(void)
{
    volatile double numbers[] = {2.0, 3.0, 4.0, 5.0};
    volatile int count = sizeof(numbers) / sizeof(numbers[0]);

    printf("\nInverse Square Root Test:\n");
    for (int i = 0; i < count; i++) {
        printf("1/sqrt(%.1f) = %.6f\n", numbers[i], 1.0 / sqrt(numbers[i]));
    }
}

void floating_point_test(void)
{
    volatile float a = 3.14159f;
    volatile float b = 2.71828f;

    printf("\nFloating Point Arithmetic Test:\n");
    printf("a = %.5f, b = %.5f\n", a, b);
    printf("Addition: %.5f + %.5f = %.5f\n", a, b, a + b);
    printf("Subtraction: %.5f - %.5f = %.5f\n", a, b, a - b);
    printf("Multiplication: %.5f * %.5f = %.5f\n", a, b, a * b);
    printf("Division: %.5f / %.5f = %.5f\n", a, b, a / b);
}

double calculate_pi_gauss_legendre(int iterations)
{
    volatile double a = 1.0;
    volatile double b = 1.0 / sqrt(2.0);
    volatile double t = 0.25;
    volatile double p = 1.0;

    for (int i = 0; i < iterations; i++) {
        double a_next = (a + b) / 2.0;
        double b_next = sqrt(a * b);
        double t_next = t - p * (a - a_next) * (a - a_next);
        double p_next = 2.0 * p;

        a = a_next;
        b = b_next;
        t = t_next;
        p = p_next;
    }

    return (a + b) * (a + b) / (4.0 * t);
}

// 円周率計算用のラッパー関数
void pi_calculation_wrapper(void)
{
    for (int i = 1; i <= 5; i++) {
        double pi = calculate_pi_gauss_legendre(i);
        printf("Iteration %d: π ≈ %.15f\n", i, pi);
    }
}

/**
 * @brief 関数の実行時間を計測する
 * 
 * @param func 計測対象の関数ポインタ
 * @param func_name 関数名（表示用）
 * @param ... 関数に渡す引数（可変長）
 */
void measure_execution_time(void (*func)(void), const char* func_name, ...)
{
    volatile uint32_t start_time = time_us_32();
    func();
    volatile uint32_t end_time = time_us_32();
    printf("proc time %s: %u us\n", func_name, end_time - start_time);
}

/**
 * @brief CPU Core0のアプリメイン関数
 * 
 */
void app_core_0_main(void)
{
    uint32_t core_num = get_core_num();

    while(1)
    {
        // printf("CPU Core: %d\n", core_num);
        // sleep_ms(1000);
    }
}

/**
 * @brief CPU Core1のアプリメイン関数
 * 
 */
void app_core_1_main(void)
{
    pico_sdk_version_print();

#if defined(PICO_RP2040) && !defined(PICO_RP2350)
    printf("MCU:\tRP2040\n");
#elif !defined(PICO_RP2040) && defined(PICO_RP2350)
    printf("MCU:\tRP2350\n");
#endif

    printf("System Clock:\t%d MHz\n", clock_get_hz(clk_sys) / 1000000);
    printf("USB Clock:\t%d MHz\n", clock_get_hz(clk_usb) / 1000000);

    dbg_com_init();

    // uint32_t core_num = get_core_num();

    while(1)
    {
        dbg_com_process();
#if 0
        printf("CPU Core: %d\n", core_num);
        sleep_ms(2000);
#endif
    }
}
