cmake_minimum_required(VERSION 3.12)

#configuring to compile a kernel
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER x86_64-elf-g++)

project(Axion LANGUAGES C CXX ASM)

add_executable(Kernel src/main.c src/start.S)

add_library(Buttons SHARED src/Drivers/Buttons.c)
target_compile_options(Buttons PRIVATE -ffreestanding)
target_link_libraries(Kernel PRIVATE Buttons)

add_library(Interrupts src/Drivers/Interrupts.c)
target_compile_options(Interrupts PRIVATE -ffreestanding)
target_link_libraries(Kernel PRIVATE Interrupts)

add_library(FileSystem src/Drivers/vfs.c)
target_compile_options(FileSystem PRIVATE -ffreestanding)
target_link_libraries(Kernel PRIVATE FileSystem)

target_compile_options(Kernel PRIVATE -ffreestanding -nostdlib)

target_link_options(Kernel PRIVATE -Wl,-T,${CMAKE_CURRENT_SOURCE_DIR}/src/linker.ld -nostdlib -static)
