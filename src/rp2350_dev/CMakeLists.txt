# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(rp2350_dev C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(rp2350_dev
            rp2350_dev.c
            drv_neopixel.c
            app_cpu_core_0.c
            app_cpu_core_1.c
            app_main.c
            app_math.c
            dbg_com.c
            mcu_util.c
            )

pico_set_program_name(rp2350_dev "rp2350_dev")
pico_set_program_version(rp2350_dev "0.1.0")

# Generate PIO header
pico_generate_pio_header(rp2350_dev
                        ${CMAKE_CURRENT_LIST_DIR}/blink.pio
                        ${CMAKE_CURRENT_LIST_DIR}/neopixel.pio
                        )

# 【コンパルオプション】
# 浮動小数はH/WのFPUを使用(-mfloat-abi=hard)
# 浮動小数はS/W(-mfloat-abi=softfp)
add_compile_options(-mfloat-abi=hard)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(rp2350_dev 0)
pico_enable_stdio_usb(rp2350_dev 1)

# Add the standard library to the build
target_link_libraries(rp2350_dev
            pico_stdlib
            pico_multicore
            pico_rand
            hardware_sha256
        )

# Add the standard include files to the build
target_include_directories(rp2350_dev PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
# ※Pico2W以外のRP2350の基板のとき用
target_link_libraries(rp2350_dev
            hardware_spi
            hardware_i2c
            hardware_dma
            hardware_pio
            hardware_interp
            hardware_timer
            hardware_watchdog
            hardware_clocks
        )

# ※Pico2Wのとき用
# target_link_libraries(rp2350_dev 
            # hardware_spi
            # hardware_i2c
            # hardware_dma
            # hardware_pio
            # hardware_interp
            # hardware_timer
            # hardware_watchdog
            # hardware_clocks
            # pico_cyw43_arch_none
#         )

pico_add_extra_outputs(rp2350_dev)

