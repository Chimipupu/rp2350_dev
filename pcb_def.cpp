/**
 * @file pcb_def.cpp
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief RP2350基板固有処理
 * @version 0.1
 * @date 2026-09-06
 * @copyright Copyright (c) 2026 Chimipupu All Rights Reserved.
 */
#include "pcb_def.h"
#include <Arduino.h>
#include <Wire.h>

#include "pico/platform.h"
#include "hardware/clocks.h"
#include "hardware/structs/sysinfo.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

// ---------------------------------------------------
#define CPU_TEMP_AVE         8

// ADC3 VSYS (GPIO 29)
#define VSYS_ADC_AVE         8
#define ADC_3_PIN           29
#define ADC_3_VSYS           3

#ifdef PCB_RPI_PICO_2
static void _adc_3_vsys_init(void);
#endif

// ---------------------------------------------------
// [Static関数]
#ifdef PCB_RPI_PICO_2
static void _adc_3_vsys_init(void)
{
    adc_gpio_init(ADC_3_PIN);
    gpio_disable_pulls(ADC_3_PIN);
    adc_select_input(ADC_3_VSYS);
}
#endif

// ---------------------------------------------------
// [割り込みハンドラ]
#ifdef BUTTON_PIN
/**
 * @brief ボタン用割り込みハンドラ
 */
void btn_ISR()
{
    static uint8_t s_led_val = 0xFF;

    digitalWrite(OB_LED_PIN, s_led_val);
    s_led_val = ~s_led_val;
}
#endif

// ---------------------------------------------------
// [API]
uint32_t get_chip_rev(void)
{
    return (sysinfo_hw->chip_id & SYSINFO_CHIP_ID_REVISION_BITS) >> SYSINFO_CHIP_ID_REVISION_LSB;
}

float get_cpu_temp(void)
{
    uint8_t i;
    float cpu_temp_ave = 0.0f;

    // 移動平均8回で平均化
    for(i = 0; i < CPU_TEMP_AVE; i++)
    {
        cpu_temp_ave += analogReadTemp();
        sleep_us(5);
    }

    cpu_temp_ave = cpu_temp_ave / CPU_TEMP_AVE;

    return cpu_temp_ave;
}

#ifdef PCB_RPI_PICO_2
float get_vsys_voltage(void)
{
    uint8_t i;
    uint32_t raw_sum = 0;
    float vsys_ave = 0.0f;

    _adc_3_vsys_init();
    sleep_us(100); // ADC Ch切り替え待ち 100us

    // 移動平均8回で平均化
    for (i = 0; i < VSYS_ADC_AVE; i++)
    {
        raw_sum += adc_read();
        sleep_us(5);
    }

#ifdef PCB_RPI_PICO_2_RED_CLONE
    // ★ 中華互換機の赤色のPico2: 「ADC3のVSYS読み取り回路問題」
    //  -> 中華の互換機だとRP2350内部のADCインピーダンス(50kオーム)になる？
    //  互換機Pico2のADC3の回路で R5(100kオーム)、R6//R16(50kオーム)
    //  なのでRP2350の内部ADCのインピーダンスが並列接続されたら分圧抵抗の下側の抵抗値が約25kΩになる
    //  となると分圧比は 1/5 になるから逆算係数として 5.0f を乗算して対策
    vsys_ave = ((float)raw_sum / (float)VSYS_ADC_AVE) * (3.3f / 4095.0f) * 5.0f;
#else
    // 純正のPico2
    vsys_ave = ((float)raw_sum / (float)VSYS_ADC_AVE) * (3.3f / 4095.0f) * 3.0f;
#endif

    // ★期待値
    //  -> VBUSが5.0Vなら、ショットキーの電圧降下 0.2V を差し引いた 4.7V
    return vsys_ave;
}
#endif

void pcb_gpio_init(void)
{
#ifdef OB_LED_PIN
    pinMode(OB_LED_PIN, OUTPUT);
    digitalWrite(OB_LED_PIN, HIGH);
#endif

#ifdef BUTTON_PIN
    pinMode(BUTTON_PIN, INPUT); // 基板ボタン
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), // IRQ
                    btn_ISR,                           // 割り込みハンドラ
                    FALLING);                          // Lowで割り込み
#endif
}

void pcb_adc_init(void)
{
    adc_init();

#ifdef PCB_RPI_PICO_2
    _adc_3_vsys_init(); // ADC3初期化
#endif
}

void pcb_uart_init(void)
{
    Serial.begin(115200); // UART初期化（115200bps 8N1）
}

void pcb_i2c_init(void)
{
    Wire.setSDA(I2C_SDA_PIN);
    Wire.setSCL(I2C_SCL_PIN);
    Wire.begin();
}

unsigned int DBG_PRINTF(const char *p_fmt, ...)
{
    unsigned int ret;
    char buf[256];
    va_list args;

    va_start(args, p_fmt);
    ret = vsnprintf(buf, sizeof(buf), p_fmt, args);
    Serial.print(buf);
    va_end(args);

    return ret;
}

void pcb_info(void)
{
    uint32_t chip_rev;
    uint32_t cpu_freq_Mhz;
    uint32_t core_num;
    float cpu_temp;

    // CPUアーキテクチャ
    DBG_PRINTF("RP2350: ARM Cortex-M33 x2 Core\n");

    // クロック
    cpu_freq_Mhz = clock_get_hz(clk_sys) / 1000000;
    DBG_PRINTF("CPU Clock: %dMHz\n", cpu_freq_Mhz);

    // CPU温度
    cpu_temp = get_cpu_temp();
    DBG_PRINTF("CPU Temp: %.03f C\n", cpu_temp);

    // 関数が動作中のCPUコア
    core_num = get_core_num();
    DBG_PRINTF("Running CPU: Core %u\n", core_num);

    // チップリビジョンRP2350: A2 or A4
    chip_rev = get_chip_rev();
    if (chip_rev == 2) {
        DBG_PRINTF("Chip Rev: A2\n");
    } else if (chip_rev == 3) {
        DBG_PRINTF("Chip Rev: A4\n");
    } else {
        DBG_PRINTF("Chip Rev: Unknown (%d)\n", chip_rev);
    }

#ifdef PCB_RPI_PICO_2
    // 基板 VSYS電圧
    float vsys;
    vsys = get_vsys_voltage();
    DBG_PRINTF("VSYS: %.03f V\n", vsys);
#endif
}
// ---------------------------------------------------