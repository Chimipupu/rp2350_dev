/**
 * @file cpm.cpp
 * @author Chimipupu（https://github.com/Chimipupu）
 * @brief 評価モニタプログラム
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "cpm.hpp"
#include "app_util.hpp"

#define DEBUG_CMD
#ifdef DEBUG_CMD
#define DBG_CMD_ARG 4
#endif

#include "math_uc.hpp"
#define MATH_PI_CALC_TIME   3
#define FIBONACCI_N         23
#define INVSQRT_N           9

#define MAX_CMD_LEN 100
#define CALC_CMD_ARG 4
#define WIDTH 80
#define HEIGHT 40
#define MAX_ITER 1000

static void math_test(void);

void help()
{
    DEBUG_PRINTF("Available commands:\n");
    DEBUG_PRINTF("  DIR    - List directory\n");
    DEBUG_PRINTF("  CLS    - Clear screen\n");
    DEBUG_PRINTF("  EXIT   - Exit the monitor\n");
    DEBUG_PRINTF("  HELP   - Show this help message\n");
    DEBUG_PRINTF("  CALC   - Perform basic arithmetic (e.g., CALC 3 + 4, CALC 2 ^ 3)\n");
    DEBUG_PRINTF("  FIB    - Display Fibonacci series (e.g., FIB 10)\n");
    DEBUG_PRINTF("  PRIME  - Display prime numbers (e.g., PRIME 10)\n");
    DEBUG_PRINTF("  PI     - Calculate Pi using Gauss-Legendre method (e.g., PI <terms>)\n");
    DEBUG_PRINTF("  MANDEL - Display Mandelbrot set (e.g., MANDEL)\n");
    DEBUG_PRINTF("  REGR   - Register Read (e.g., REGR <Address>)\n");
    DEBUG_PRINTF("  REGW   - Register Write (e.g., REGW <Address> <Val>)\n");
    DEBUG_PRINTF("  TEST   - Performance test cmd\n");
#ifdef DEBUG_CMD
    DEBUG_PRINTF("  DBG    - Develop cmd\n");
#endif
}

void dir()
{
    DEBUG_PRINTF("Directory listing:\n");
    DEBUG_PRINTF("File1.txt\nFile2.txt\nFile3.txt\n");
}

void cls()
{
    // system("clear"); // UNIX/Linux系の場合
    // // system("cls");  // Windowsの場合
}

void calculate(char *p_cmd)
{
    char op;
    double num1, num2;

    if (sscanf(p_cmd, "%lf %c %lf", &num1, &op, &num2) == 3)
    {
        DEBUG_PRINTF("Debug: num1 = %.2lf, op = %c, num2 = %.2lf\n", num1, op, num2);
        switch (op)
        {
        case '+':
            DEBUG_PRINTF("Result: %.2lf\n", num1 + num2);
            break;
        case '-':
            DEBUG_PRINTF("Result: %.2lf\n", num1 - num2);
            break;
        case '*':
            DEBUG_PRINTF("Result: %.2lf\n", num1 * num2);
            break;
        case '/':
            if (num2 != 0)
            {
                DEBUG_PRINTF("Result: %.2lf\n", num1 / num2);
            }
            else
            {
                DEBUG_PRINTF("Error: Division by zero!\n");
            }
            break;
        case '%':
            DEBUG_PRINTF("Result: %.0lf\n", fmod(num1, num2));
            break;
        case '^':
            DEBUG_PRINTF("Result: %.2lf\n", pow(num1, num2));
            break;
        default:
            DEBUG_PRINTF("Unknown op: %c\n", op);
            break;
        }
    }
    else
    {
        DEBUG_PRINTF("Invalid format. Use: CALC <num1> <op> <num2>\n");
    }
}

void fibonacci(int n)
{
    int t1 = 0, t2 = 1, nextTerm;
    DEBUG_PRINTF("Fibonacci Series: ");

    for (int i = 1; i <= n; ++i)
    {
        DEBUG_PRINTF("%d ", t1);
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    }
    DEBUG_PRINTF("\n");
}

int isPrime(int num)
{
    if (num <= 1)
        return 0; // 1以下は素数ではない
    for (int i = 2; i <= sqrt(num); i++)
    {
        if (num % i == 0)
            return 0; // 割り切れる数があれば素数ではない
    }
    return 1; // 素数
}

void prime(int n)
{
    DEBUG_PRINTF("Prime Numbers: ");
    int count = 0;
    for (int i = 2; count < n; i++)
    {
        if (isPrime(i))
        {
            DEBUG_PRINTF("%d ", i);
            count++;
        }
    }
    DEBUG_PRINTF("\n");
}

void calculatePi(int terms)
{
    double a = 1.0;             // 初期値
    double b = 1.0 / sqrt(2.0); // 初期値
    double t = 0.25;            // 初期値
    double p = 1.0;             // 初期値
    double pi;

    for (int i = 0; i < terms; i++)
    {
        double a_next = (a + b) / 2.0;
        double b_next = sqrt(a * b);
        t -= p * (a - a_next) * (a - a_next);
        a = a_next;
        b = b_next;
        p *= 2.0;
    }

    pi = (a + b) * (a + b) / (4.0 * t);
    DEBUG_PRINTF("Pi calculated to %d terms: %.15f\n", terms, pi);
}

void mandelbrot()
{
    DEBUG_PRINTF("Mandelbrot Set:\n");

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

                // zの更新
                double z_re_new = z_re * z_re - z_im * z_im + c_re;
                z_im = 2.0 * z_re * z_im + c_im;
                z_re = z_re_new;
            }

            // 文字を使った出力
            char symbol = (iteration == MAX_ITER) ? '#' : ' ';
            DEBUG_PRINTF("%c", symbol);
        }
        DEBUG_PRINTF("\n");
    }
}

static void math_test(void)
{
    uint32_t i,fib;
    volatile double pi;
    volatile double phi;
    volatile double napier;
    volatile double invsqrt;
    volatile double result;

    // tan(355/226)の計算（※期待値:-7497258.185...）
    result = math_calc_accuracy();
    DEBUG_PRINTF("tan(355/226)=%.3f\n", result);

    // 円周率π
    for(i=1; i<=MATH_PI_CALC_TIME; i++)
    {
        pi = math_pi_calc(i);
    }
    DEBUG_PRINTF("pi=%.15f\n", pi);

    // ネイピアe
    napier = math_napier_calc();
    DEBUG_PRINTF("e=%.15f\n", napier);

    // 黄金比φ
    phi = math_goldenratio_calc();
    DEBUG_PRINTF("phi=%.15f\n", phi);

    // フィボナッチ数列
    DEBUG_PRINTF("Fn = ");
    for(i = 1; i < FIBONACCI_N; i++)
    {
        fib = math_fibonacci_calc(i);
        DEBUG_PRINTF("%d ", fib);
    }
    DEBUG_PRINTF("\n");

    // 高速逆平方根
    for(i = 1; i < INVSQRT_N; i++)
    {
        invsqrt = math_fast_inv_sqrt(i);
        DEBUG_PRINTF("%d's inv sqrt = %.15f\n", i, invsqrt);
    }
    DEBUG_PRINTF("\n");
}

#ifdef DEBUG_CMD
void dbg_cmd(char *p_cmd)
{
    DEBUG_PRINTF("DEBUG Command\r\n");
    app_util_system_reg_read();

    // NOP
}
#endif

void cpm_op_msg(void)
{
    DEBUG_PRINTF("Loading CPM.SYS...\r\n");

    DEBUG_PRINTF("CP/M(RP2350) by Chimi\r\n");
    DEBUG_PRINTF("Version 1.0.0\r\n");
    DEBUG_PRINTF("Copyright(C) 2024, Chimi\r\n");

    // DEBUG_PRINTF("Type HELP for a list of commands\r\n");
}

void cpm_main(void)
{
    char command[MAX_CMD_LEN];
    uint32_t idx = 0;

    // Serial.begin(115200);
    while (!Serial)
        ;

    cpm_op_msg();
    help();

    while (1)
    {
        DEBUG_PRINTF("\n> ");
        idx = 0; // 入力インデックスの初期化
        while (true)
        {
            if (Serial.available())
            {
                char ch = Serial.read();

                if (ch == '\n') // エンターキーが押された
                {
                    command[idx] = '\0'; // 文字列の終端
                    break; // 入力完了
                }
                else if (idx < MAX_CMD_LEN - 1) // バッファオーバーフローを防ぐ
                {
                    command[idx++] = ch; // 文字をコマンドに追加
                }
            }
        }

        // コマンドの処理
        if (strcmp(command, "EXIT") == 0)
        {
            break;
        }
        else if (strcmp(command, "DIR") == 0)
        {
            dir();
        }
        else if (strcmp(command, "CLS") == 0)
        {
            cls();
        }
        else if (strcmp(command, "HELP") == 0)
        {
            help();
        }
        else if (strstr(command, "CALC") == command)
        {
            char *p_cmd = command + CALC_CMD_ARG;
            while (*p_cmd == ' ')
                p_cmd++;
            // DEBUG_PRINTF("Debug: Expression = '%s'\n", p_cmd);
            calculate(p_cmd);
        }
        else if (strstr(command, "FIB") == command)
        {
            int n = atoi(command + 4);
            fibonacci(n);
        }
        else if (strstr(command, "PRIME") == command)
        {
            int n = atoi(command + 6);
            prime(n);
        }
        else if (strstr(command, "PI") == command)
        {
            int terms = atoi(command + 3);
            calculatePi(terms);
        }
        else if (strcmp(command, "MANDEL") == 0)
        {
            mandelbrot();
        }
        else if (strcmp(command, "TEST") == 0)
        {
            math_test();
        }
#ifdef DEBUG_CMD
        else if (strstr(command, "DBG") == command)
        {
            char *p_cmd = command + DBG_CMD_ARG;
            while (*p_cmd == ' ')
                p_cmd++;
            // DEBUG_PRINTF("Debug: Expression = '%s'\n", p_cmd);
            dbg_cmd(p_cmd);
        }
#endif
        else
        {
            DEBUG_PRINTF("Bad command: %s\n", command);
        }
    }

    DEBUG_PRINTF("rebooting ...\n");
}