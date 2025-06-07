#include "app_main.h"
#include "pico/version.h"

static void pico_sdk_version_print(void);

static void pico_sdk_version_print(void)
{
    printf("Pico SDK version: %d.%d.%d\n",
        PICO_SDK_VERSION_MAJOR,
        PICO_SDK_VERSION_MINOR,
        PICO_SDK_VERSION_REVISION);
}

/**
 * @brief アプリメイン
 * 
 */
void app_main(void)
{
    pico_sdk_version_print();

    #if defined(PICO_RP2040) && !defined(PICO_RP2350)
    printf("MCU:\tRP2040\n");
#elif !defined(PICO_RP2040) && defined(PICO_RP2350)
    printf("MCU:\tRP2350\n");
#endif

    printf("System Clock:\t%d MHz\n", clock_get_hz(clk_sys) / 1000000);
    printf("USB Clock:\t%d MHz\n", clock_get_hz(clk_usb) / 1000000);

    sleep_ms(1000);
}