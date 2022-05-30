cmake_minimum_required(VERSION 3.12)

set (PROJECT_NAME test_RTC_GPS)


# rest of your project
add_executable(${PROJECT_NAME}
    main.c
        src/mcp7940.c
        src/pam7q.c
)



project(${PROJECT_NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

set(MYPROJECT_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()



add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )


target_link_libraries(${PROJECT_NAME} pico_stdlib pico_multicore pico_rmii_ethernet)
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_i2c hardware_uart)

target_include_directories (${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})


# enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})
