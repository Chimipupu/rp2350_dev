#ifndef APP_MAIN_H
#define APP_MAIN_H

#include "rp2350_dev.h"
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "pico/version.h"
#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"

// 四則演算の回数（整数、float,double）100万回
#define TEST_LOOP_CNT 1000000

// NOP
static inline void NOP(void)
{
    __asm__ __volatile__("nop");
}

static inline void WDT_RST(void)
{
#ifdef _WDT_ENABLE_
        watchdog_update();
#else
        NOP();
#endif // _WDT_ENABLE_
}

void app_core_0_main(void);
void app_core_1_main(void);
void i2c_slave_scan(uint8_t i2c_port);
void measure_execution_time(void (*p_func)(void), const char* p_func_name, ...);
double calculate_pi_gauss_legendre(int iterations);
void trig_functions_test(void);
void atan2_test(void);
void tan_355_226_test(void);
void inverse_sqrt_test(void);
void int_add_test(void);
void int_sub_test(void);
void int_mul_test(void);
void int_div_test(void);
void float_add_test(void);
void float_sub_test(void);
void float_mul_test(void);
void float_div_test(void);
void double_add_test(void);
void double_sub_test(void);
void double_mul_test(void);
void double_div_test(void);

#endif // APP_MAIN_H