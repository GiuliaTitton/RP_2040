# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/WORKSPACE/PSE2_RP2040/RP_2040/FW/pico-sdk/tools/elf2uf2"
  "D:/WORKSPACE/PSE2_RP2040/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2"
  "D:/WORKSPACE/PSE2_RP2040/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2"
  "D:/WORKSPACE/PSE2_RP2040/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2/tmp"
  "D:/WORKSPACE/PSE2_RP2040/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2/src/ELF2UF2Build-stamp"
  "D:/WORKSPACE/PSE2_RP2040/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2/src"
  "D:/WORKSPACE/PSE2_RP2040/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/WORKSPACE/PSE2_RP2040/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
