@echo off


REM Copyright (C) 2026  PhosphorosWasTaken
REM Copyright (C) 2026  p123o215
REM
REM This program is free software: you can redistribute it and/or modify
REM it under the terms of the GNU General Public License as published by
REM the Free Software Foundation, either version 3 of the License, or
REM (at your option) any later version.
REM
REM This program is distributed in the hope that it will be useful,
REM but WITHOUT ANY WARRANTY; without even the implied warranty of
REM MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
REM GNU General Public License for more details.
REM
REM You should have received a copy of the GNU General Public License
REM along with this program.  If not, see <https://gnu.org>.
REM
REM See the LICENSE file in the project root for additional terms 
REM appended under GPL v3 Section 7 regarding attribution screens.

if exist "build\" (
    choice /c YN /m "build directory exists. Do you want to delete cached build?"
    if errorlevel 2 goto :No
    if errorlevel 1 goto :Yes
    :Yes
    rmdir /s /q "build"
    mkdir "build"
    attrib +h "build"
    cd "build"
    goto :Continue1

    :No
    goto :Continue1

) else (
    echo "Build directory does not exist. Creating build directory..."
    mkdir "build"
    attrib +h "build"
    cd "build"
    echo "Created build directory."
)

:Continue1

rem diverging logic based on current platform
if /i "%PROCESSOR_ARCHITECTURE%"=="AMD64" (
    echo "AMD64 architecture detected, including cross-compilation files"
    rem we need to use the x86_64 toolchain in order to compile
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchains/toolchain_x86-64.cmake
    cmake --build .
) else if /i "%PROCESSOR_ARCHITECTURE%"=="ARM64" (
    echo "ARM64 architecture detected"
    cmake ..
    cmake --build .
) else (
    echo "cannot build for systems which are not on AMD64 or ARM64."
    echo "if you think your architecture should be able to build this project, then write a toolchain file and submit a pull request to add your architecture."
)
