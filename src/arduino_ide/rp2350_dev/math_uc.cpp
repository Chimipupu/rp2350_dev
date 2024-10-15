/**
 * @file math_uc.cpp
 * @author Chimipupu（https://github.com/Chimipupu）
 * @brief 数学計算アプリ
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "math_uc.hpp"

// 計算精度の表示（期待値:-7497258.185...）
double math_calc_accuracy(void)
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
bool math_is_prime_num(uint32_t n)
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
double math_pythagoras(double a, double b)
{
    double c = UNKNOWN_VAL;

    c = sqrt((double)(a * a) + (double)(b * b));

    return c;
}

// ガウス・ルジャンドル法による円周率の計算
double math_pi_calc(uint32_t cnt)
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
uint32_t math_fibonacci_calc(uint32_t cnt)
{
    if (cnt <= 1) {
        return cnt;
    } else {
        return math_fibonacci_calc(cnt - 1) + math_fibonacci_calc(cnt - 2);
    }
}

// 黄金比の計算
double math_goldenratio_calc(void)
{
    double phi = (1 + sqrt(5)) / 2.0; // 黄金比の値

    return phi;
}

// ネイピア(マイコンでの計算は無理やから定数出力)
double math_napier_calc(void)
{
    return MATH_E;
}

// 高速逆平方根
float math_fast_inv_sqrt(float num)
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
