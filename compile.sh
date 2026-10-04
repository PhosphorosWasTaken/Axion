#!/usr/bin/env bash

set -e

BUILD_DIR="build"

cmake -B "${BUILD_DIR}" -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=Generic -DCMAKE_SYSTEM_PROCESSOR=aarch64 -DCMAKE_C_COMPILER=aarch64-none-elf-gcc -DCMAKE_CXX_COMPILER=aarch64-none-elf-g++

cmake --build "${BUILD_DIR}" --parallel "$(nproc)"


