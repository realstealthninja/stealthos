cmake_minimum_required(VERSION 3.30)

set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER /usr/local/cross/bin/x86_64-elf-gcc)
set(CMAKE_LINKER /usr/local/cross/bin/x86_64-elf-ld)

set(CMAKE_C_COMPILER_WORKS true)
set(CMAKE_CXX_COMPILER_WORKS true)

set(CMAKE_FIND_ROOT_PATH /usr/local/cross/)
set(CMAKE_SYSROOT /usr/local/cross/)

set(TARGET_COMPILE_OPTIONS -mcmodel=kernel -mno-red-zone)
