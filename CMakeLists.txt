cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(main C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# No need define when use Pico
#add_definitions(-DPICO_BOARD=pico)

pico_sdk_init()

add_executable(${PROJECT_NAME}
  main.c
)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

target_link_libraries(${PROJECT_NAME} pico_stdlib)

pico_add_extra_outputs(main)
