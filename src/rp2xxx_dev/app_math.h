/**
 * @file app_math.hpp
 * @author ちみ/Chimi(https://github.com/Chimipupu)
 * @brief 数学計算アプリ
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#ifndef APP_MATH_H
#define APP_MATH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#ifndef M_PI
#define MATH_PI      M_PI
#else
#define MATH_PI      3.14159265358979323846
#endif

#ifndef M_E
#define MATH_E      2.7182818284590452354
#else
#define MATH_E      M_E
#endif

#define UNKNOWN_VAL    0

// 四則演算の回数（整数、float,double）100万回
#define TEST_LOOP_CNT 1000000

#define MATH_DEG_TO_RAD(deg)    (deg * M_PI) / 180.0f      // 度からラジアン
#define MATH_RAD_TO_DEG(rad)    ((rad) * 180.0f / M_PI)    // ラジアンから度

double app_math_calc_accuracy(void);
bool app_math_is_prime_num(uint32_t n);
double app_math_pythagoras(double a, double b);
double app_math_pi_calc(uint32_t cnt);
uint32_t app_math_fibonacci_calc(uint32_t cnt);
double app_math_goldenratio_calc(void);
double app_math_napier_calc(void);
float app_math_fast_inv_sqrt(float num);
void app_math_fibonacci(uint32_t n);
void app_math_prime(uint32_t n);
void app_math_mandelbrot(void);
void app_math_math_test(void);
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

#endif // APP_MATH_H