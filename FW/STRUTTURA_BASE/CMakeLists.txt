cmake_minimum_required(VERSION 3.12)


#message("/********/  MODIFICARE IL PERCORSO DI PICO_SDK_PATH IN FILE CMAKELISTSTXT CON IL PROPRIO PERCORSO  /******/")

set(PICO_SDK_PATH D:/WORKSPACE/RASPBERRY_PICO/LIBRERIE/pico-sdk)#MODIFICARE QUESTO PERCORSO CON CARTELLA pico-sdk
# Pull in SDK (must be before project)

include(pico_sdk_import.cmake)
#todo: PICO_SDK_FETCH_FROM_GIT set to true to pull from git??? (ANDREA)
# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
#set(PICO_SDK_FETCH_FROM_GIT on)

set (PROJECT_NAME test1)



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


add_executable(${PROJECT_NAME}
        main.c
	src/file.c
        )

# pull in common dependencies
target_link_libraries(${PROJECT_NAME} pico_stdlib)

target_include_directories (${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

# add url via pico_set_program_url
#example_auto_set_url(test_prj_folder)


#added from digikey example!
# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
