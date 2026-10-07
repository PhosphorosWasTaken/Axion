#Copyright (C) 2026  PhosphorosWasTaken
#Copyright (C) 2026  p123o215
#
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <https://gnu.org>.
#
#See the LICENSE file in the project root for additional terms 
#appended under GPL v3 Section 7 regarding attribution screens.

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
