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
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(tocando-com-buzzers C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(tocando-com-buzzers 
    src/main.c
    src/timers_callback.c
    src/ocarina.c
    src/songs/song_of_storms.c
    src/main_pio.c
    src/led_functions.c
    src/joystick.c
    src/drivers/frames_3x5.c
    src/drivers/frames_5x5.c     
    src/drivers/letras_3x5.c
    src/drivers/letras_5x5.c
    )

pico_set_program_name(tocando-com-buzzers "tocando-com-buzzers")
pico_set_program_version(tocando-com-buzzers "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(tocando-com-buzzers 1)
pico_enable_stdio_usb(tocando-com-buzzers 1)

# Add the standard library to the build
target_link_libraries(tocando-com-buzzers
        pico_stdlib
        pico_stdio_usb
        hardware_clocks
        hardware_pwm
        hardware_gpio
        hardware_dma
        hardware_adc
        hardware_i2c
        hardware_pio
        )

# Add the standard include files to the build
target_include_directories(tocando-com-buzzers PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_generate_pio_header(tocando-com-buzzers ${CMAKE_CURRENT_LIST_DIR}/main.pio)

# Add any user requested libraries
target_link_libraries(tocando-com-buzzers 
        
        )

pico_add_extra_outputs(tocando-com-buzzers)

