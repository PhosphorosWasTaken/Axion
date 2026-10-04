#!/usr/bin/env bash

set -e

BUILD_DIR="build"

cmake -B "${BUILD_DIR}" -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=aarch64 -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++

cmake --build "${BUILD_DIR}" --parallel "$(nproc)"

## Start the VM

qemu-system-aarch64 -M raspi3b -cpu cortex-a53 -m 1024 -kernel build/Kernel -drive file=build/sd_hat.img,format=raw,id=sd_card1,if=none -device sd-card,drive=sd_card1 -nographic
