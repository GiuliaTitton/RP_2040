# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/matteo/Documents/GitHub/RP_2040/FW/pico-sdk/tools/elf2uf2"
  "/Users/matteo/Documents/GitHub/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2"
  "/Users/matteo/Documents/GitHub/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2"
  "/Users/matteo/Documents/GitHub/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2/tmp"
  "/Users/matteo/Documents/GitHub/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2/src/ELF2UF2Build-stamp"
  "/Users/matteo/Documents/GitHub/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2/src"
  "/Users/matteo/Documents/GitHub/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/matteo/Documents/GitHub/RP_2040/FW/TEST_RTC_GPS/build/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
