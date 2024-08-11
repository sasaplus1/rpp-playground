cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(main C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# No need define when use Pico
#add_definitions(-DPICO_BOARD=pico)

pico_sdk_init()

include(FetchContent)
FetchContent_Declare(
  pico_ssd1306
  GIT_REPOSITORY https://github.com/daschr/pico-ssd1306.git
  GIT_TAG 8467f5b06ebd7bfa7b53e4ebc83dfdc6f396e4eb
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(pico_ssd1306)

add_executable(${PROJECT_NAME}
  main.c
  ${CMAKE_BINARY_DIR}/_deps/pico_ssd1306-src/ssd1306.c
)

# set 0 when use tinyusb
pico_enable_stdio_usb(main 0)
pico_enable_stdio_uart(main 0)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_BINARY_DIR}/_deps/pico_ssd1306-src
)

target_link_libraries(${PROJECT_NAME}
  pico_stdlib
  hardware_i2c
  tinyusb_device
  tinyusb_board
)

pico_add_extra_outputs(main)
