# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(quadruped CXX C ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD   11)

pico_sdk_init()

add_executable(quadruped
    src/main.cpp
    lib/Servo/Servo.cpp
    lib/Joint/Joint.cpp
    lib/Leg/Leg.cpp
    lib/Quadruped/Quadruped.cpp
)

target_include_directories(quadruped PRIVATE
    lib/Servo
    lib/Joint
    lib/Leg
    lib/Quadruped
)

target_link_libraries(quadruped
    pico_stdlib
    hardware_pwm
    hardware_uart
    pico_float
)


pico_enable_stdio_usb(quadruped  1)
pico_enable_stdio_uart(quadruped 0)

pico_add_extra_outputs(quadruped)
