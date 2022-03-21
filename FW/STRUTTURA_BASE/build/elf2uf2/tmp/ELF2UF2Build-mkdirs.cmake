# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/WORKSPACE/RASPBERRY_PICO/PROGETTI/pico-sdk/tools/elf2uf2"
  "D:/WORKSPACE/RASPBERRY_PICO/PROGETTI/test_prj_folder/build/elf2uf2"
  "D:/WORKSPACE/RASPBERRY_PICO/PROGETTI/test_prj_folder/build/elf2uf2"
  "D:/WORKSPACE/RASPBERRY_PICO/PROGETTI/test_prj_folder/build/elf2uf2/tmp"
  "D:/WORKSPACE/RASPBERRY_PICO/PROGETTI/test_prj_folder/build/elf2uf2/src/ELF2UF2Build-stamp"
  "D:/WORKSPACE/RASPBERRY_PICO/PROGETTI/test_prj_folder/build/elf2uf2/src"
  "D:/WORKSPACE/RASPBERRY_PICO/PROGETTI/test_prj_folder/build/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/WORKSPACE/RASPBERRY_PICO/PROGETTI/test_prj_folder/build/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
