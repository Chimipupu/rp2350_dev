/**
 * @file app_math.cpp
 * @author ちみ/Chimi(https://github.com/Chimipupu)
 * @brief 数学計算アプリ
 * @version 0.1
 * @date 2025-07-19
 * 
 * @copyright Copyright (c) 2025 Chimipupu All Rights Reserved.
 * 
 */
#include "app_math.h"
#include "muc_rpxxx_util.h"

#define MATH_PI_CALC_TIME   3
#define FIBONACCI_N         20
#define INVSQRT_N           7
#define WIDTH               80
#define HEIGHT              40
#define MAX_ITER            1000

// 計算精度の表示（期待値:-7497258.185...）
double app_math_calc_accuracy(void)
{
    double angle_a;
    double angle_b;
    double radians;
    double result;

    angle_a = 355.f * MATH_PI / 180;
    angle_b = 226.f * MATH_PI / 180;
    radians = angle_a / angle_b;
    result = tan(radians);

    return result;
}

// エラトステネスのふるいで素数を判定
bool app_math_is_prime_num(uint32_t n)
{
    uint32_t i;

    if (n <= 1) {
        return false;
    }

    for(i = 2; i * i <= n; ++i)
    {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

// ピタゴラスの定理(c = sqrt(a^2 + b^2))
double app_math_pythagoras(double a, double b)
{
    double c = 0;

    c = sqrt((double)(a * a) + (double)(b * b));

    return c;
}

// ガウス・ルジャンドル法による円周率の計算
double app_math_pi_calc(uint32_t cnt)
{
    double a = 1.0;
    double b = 1.0 / sqrt(2);
    double t = 1.0 / 4.0;
    double p = 1.0;

    for (uint32_t i = 0; i < cnt; i++)
    {
        double an = (a + b) / 2.0;
        double bn = sqrt(a * b);
        double tn = t - p * pow(a - an, 2);
        double pn = 2.0 * p;

        a = an;
        b = bn;
        t = tn;
        p = pn;
    }

    double pi = pow(a + b, 2) / (4.0 * t);
    return pi;
}

// フィボナッチ数列の計算
uint32_t app_math_fibonacci_calc(uint32_t cnt)
{
    uint32_t val_a, val_b, fib = 0;

    if (cnt == 0) {
        fib = 0;
    } else if (cnt == 1) {
        fib = 1;  // ここを1に修正
    } else {
        val_a = app_math_fibonacci_calc(cnt - 1);
        val_b = app_math_fibonacci_calc(cnt - 2);
        fib = val_a + val_b;
    }

    return fib;
}

// 黄金比の計算
double app_math_goldenratio_calc(void)
{
    double phi = 0;

    phi = (1 + sqrt(5)) / 2.0; // 黄金比の値

    return phi;
}

// ネイピア(マイコンでの計算は無理やから定数出力)
double app_math_napier_calc(void)
{
    return MATH_E;
}

// 高速逆平方根
float app_math_fast_inv_sqrt(float num)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = num * 0.5F;
    y = num;
    i = *(long *)&y;                     // 浮動小数点数をビットパターンとして解釈
    i = 0x5f3759df - (i >> 1);           // ビット操作による初期推定値
    y = *(float *)&i;                    // 初期推定値を浮動小数点数に戻す
    y = y * (threehalfs - (x2 * y * y)); // ニュートン法による補正

    return y;
}

void app_math_fibonacci(uint32_t n)
{
    uint32_t i, fib = 0;

    printf("Fibonacci : ");
    for(i = 1; i < n; i++)
    {
        fib = app_math_fibonacci_calc(i);
        printf("%d ", fib);
    }
    printf("\n");
}

void app_math_prime(uint32_t n)
{
    printf("Prime Numbers: ");
    uint32_t count = 0;
    for (int i = 2; count < n; i++)
    {
        if (app_math_is_prime_num(i))
        {
            printf("%d ", i);
            count++;
        }
    }
    printf("\n");
}

// ガウス・ルジャンドル法で円周率を計算
void app_math_pi_show(uint32_t n)
{
    _DI();
    uint32_t start_time = time_us_32();
    _EI();

    double pi = app_math_pi_calc(n);

    _DI();
    uint32_t end_time = time_us_32();
    _EI();

    printf("pi = %.15f\n", pi);
    printf("proc time : %d usec\n", end_time - start_time);
}

// マンデルブロ集合の描画
void app_math_mandelbrot(void)
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            double c_re = (x - WIDTH / 2.0) * 4.0 / WIDTH;   // xのスケーリング
            double c_im = (y - HEIGHT / 2.0) * 4.0 / HEIGHT; // yのスケーリング
            double z_re = c_re, z_im = c_im;
            int iteration;

            for (iteration = 0; iteration < MAX_ITER; iteration++)
            {
                if (z_re * z_re + z_im * z_im > 4.0)
                    break; // 発散判定

                double z_re_new = z_re * z_re - z_im * z_im + c_re;
                z_im = 2.0 * z_re * z_im + c_im;
                z_re = z_re_new;
            }

            char symbol = (iteration == MAX_ITER) ? '#' : ' ';
            printf("%c", symbol);
        }
        printf("\n");
    }
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
    volatile double sin_val, cos_val, tan_val;

    sin_val = sin(rad);
    cos_val = cos(rad);
    tan_val = tan(rad);
}

void atan2_test(void)
{
    volatile double x = 1.0;
    volatile double y = 1.0;
    volatile double result;

    result = atan2(y, x);
}

void tan_355_226_test(void)
{
    volatile double result;
    result = tan(355.0 / 226.0);
}

void inverse_sqrt_test(void)
{
    volatile double numbers[] = {2.0, 3.0, 4.0, 5.0};
    volatile int count = sizeof(numbers) / sizeof(numbers[0]);
    volatile double result;

    for (int i = 0; i < count; i++) {
        result = 1.0 / sqrt(numbers[i]);
    }
}

void app_math_math_test(void)
{
    uint32_t i,fib;
    volatile double pi;
    volatile double phi;
    volatile double napier;
    volatile double invsqrt;
    volatile double result;

    printf("**************************************************************************\n");
    printf("[App Math Test]\n");
    printf("**************************************************************************\n");

    // tan(355/226)の計算（※期待値:-7497258.185...）
    result = app_math_calc_accuracy();
    printf("tan(355/226) = %.3f\n", result);

    // 円周率π
    pi = app_math_pi_calc(MATH_PI_CALC_TIME);
    printf("pi = %.15f\n", pi);

    // ネイピアe
    napier = app_math_napier_calc();
    printf("e = %.15f\n", napier);

    // 黄金比φ
    phi = app_math_goldenratio_calc();
    printf("phi = %.15f\n", phi);

    // フィボナッチ数列
    app_math_fibonacci(FIBONACCI_N);

    // 高速逆平方根
    for(i = 1; i < INVSQRT_N; i++)
    {
        invsqrt = app_math_fast_inv_sqrt(i);
        printf("%d's inv sqrt = %.15f\n", i, invsqrt);
    }

    // マンデルブロ集合の描画
    app_math_mandelbrot();

    printf("**************************************************************************\n");
}