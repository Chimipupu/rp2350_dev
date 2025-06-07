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

void floating_point_test(void)
{
    float a = 3.14159f;
    float b = 2.71828f;

    printf("\nFloating Point Arithmetic Test:\n");
    printf("a = %.5f, b = %.5f\n", a, b);
    printf("Addition: %.5f + %.5f = %.5f\n", a, b, a + b);
    printf("Subtraction: %.5f - %.5f = %.5f\n", a, b, a - b);
    printf("Multiplication: %.5f * %.5f = %.5f\n", a, b, a * b);
    printf("Division: %.5f / %.5f = %.5f\n", a, b, a / b);
}

double calculate_pi_gauss_legendre(int iterations)
{
    double a = 1.0;
    double b = 1.0 / sqrt(2.0);
    double t = 0.25;
    double p = 1.0;

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
    uint64_t start_time = time_us_64();
    func();
    uint64_t end_time = time_us_64();
    printf("proc time %s: %llu us\n", func_name, end_time - start_time);
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
        printf("CPU Core: %d\n", core_num);
        sleep_ms(1000);
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

    uint32_t core_num = get_core_num();

    while(1)
    {
        // dbg_com_process();

        printf("CPU Core: %d\n", core_num);
        sleep_ms(2000);
    }
}