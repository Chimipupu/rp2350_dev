#include "app_main.h"
#include "dbg_com.h"

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

/**
 * @brief i2Cスレーブデバイスのスキャン関数
 * 
 * @param port I2Cポート番号 (0 or 1)
 */
void i2c_slave_scan(uint8_t port)
{
    int32_t ret = 0xFF;
    uint8_t addr, dummy = 0x00;
    uint8_t slave_count = 0;
    uint8_t slave_addr_buf[128] = {0};

    memset(&slave_addr_buf[0], 0x00, sizeof(slave_addr_buf));
    i2c_inst_t *i2c_port = (port == 0) ? I2C_0_PORT : I2C_1_PORT;

    // 7bitのI2Cスレーブアドレス(0x00～0x7F)をスキャン
    printf("Scanning I2C%d bus...\n", port);
    printf("       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    for (addr = 0; addr <= 0x7F; addr++)
    {
        if ((addr & 0x0F) == 0) {
            printf("0x%02X: ", addr & 0xF0);
        }

        ret = i2c_write_blocking(i2c_port, addr, &dummy, 1, false);
        if (ret >= 0) {
            printf(" * ");
            slave_addr_buf[slave_count] = addr;
            slave_count++;
        } else {
            printf(" - ");
        }

        if ((addr & 0x0F) == 0x0F) {
            printf("\n");
        }
    }

    printf("\nI2C Scan complete! (Slave:%d", slave_count);
    for (uint8_t i = 0; i < slave_count; i++) {
        printf(", 0x%02X", slave_addr_buf[i]);
    }
    printf(")\n");
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
