cmake_minimum_required(VERSION 3.12)

#configuring to compile a kernel
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(Axion LANGUAGES C CXX ASM)

if(NOT CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")
    message(FATAL_ERROR "Target processor must be 64 bit ARM!")
endif()

add_executable(Kernel src/main.c src/start.S)

target_compile_options(Kernel PRIVATE -ffreestanding -nostdlib)

target_link_options(Kernel PRIVATE -Wl,-T,${CMAKE_CURRENT_SOURCE_DIR}/src/linker.ld -nostdlib -static)
