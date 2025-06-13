/**
 * @file rp2350_dev.c
 * @author Chimipupu(https://github.com/Chimipupu)
 * @brief Pico SDK生成ファイル
 * @version 0.1
 * @date 2025-06-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "rp2350_dev.h"
#include "app_main.h"
#include "pico/multicore.h"

const char src[] = "Hello, world! (from DMA)";
char dst[count_of(src)];

#include "blink.pio.h"

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq) {
    blink_program_init(pio, sm, offset, pin);
    pio_sm_set_enabled(pio, sm, true);

    printf("Blinking pin %d at %d Hz\n", pin, freq);

    // PIO counter program takes 3 more cycles in total than we pass as
    // input (wait for n + 1; mov; jmp)
    pio->txf[sm] = (125000000 / (2 * freq)) - 3;
}

/**
 * @brief 2000msタイマー割り込みコールバック関数
 * 
 * @param id 
 * @param p_user_data 
 * @return int64_t 
 */
int64_t alarm_callback(alarm_id_t id, void *p_user_data)
{
    NOP();

    return 0;
}

int main()
{
    stdio_init_all();

    // CPU Core1を起動
    multicore_launch_core1(core_1_main);

    // SPI初期化(@1MHz)
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    // I2C0初期化 (@100KHz)
    i2c_init(I2C_0_PORT, 100 * 1000);
    gpio_set_function(I2C_0_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_0_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_0_SDA);
    gpio_pull_up(I2C_0_SCL);

    // I2C1初期化 (@100KHz)
    i2c_init(I2C_1_PORT, 100 * 1000);
    gpio_set_function(I2C_1_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_1_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_1_SDA);
    gpio_pull_up(I2C_1_SCL);

    // DMA初期化(@8bit ブロッキング)
    int chan = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(chan);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, true);
    dma_channel_configure(
        chan,          // Channel to be configured
        &c,            // The configuration we just created
        dst,           // The initial write address
        src,           // The initial read address
        count_of(src), // Number of transfers; in this case each is 1 byte.
        true           // Start immediately.
    );
    dma_channel_wait_for_finish_blocking(chan);
    // DMAで転送
    puts(dst);

    // PIO初期化
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &blink_program);
    printf("Loaded program at %d\n", offset);

#ifdef PICO_DEFAULT_LED_PIN
    blink_pin_forever(pio, 0, offset, PICO_DEFAULT_LED_PIN, 3);
#else
    blink_pin_forever(pio, 0, offset, 6, 3);
#endif

    // 割り込み初期化
    interp_config cfg = interp_default_config();
    interp_set_config(interp0, 0, &cfg);

    // タイマー割り込み @2000ms
    add_alarm_in_ms(2000, alarm_callback, NULL, false);

#ifdef _WDT_ENABLE_
    if (watchdog_caused_reboot()) {
        printf("Rebooted by Watchdog!\n");
    }

    watchdog_enable(_WDT_OVF_TIME_MS_, 1);
    WDT_RST();
#endif // _WDT_ENABLE_

printf("System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys));
    printf("USB Clock Frequency is %d Hz\n", clock_get_hz(clk_usb));

    // UART初期化(@115200bps,8N1)
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_puts(UART_ID, " Hello, UART!\n");

    core_0_main();
}