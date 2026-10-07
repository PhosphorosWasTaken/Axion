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

#!/usr/bin/env bash

set -e

BUILD_DIR="build"

cmake -B "${BUILD_DIR}" -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=aarch64 -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++

cmake --build "${BUILD_DIR}" --parallel "$(nproc)"

## Start the VM

qemu-system-aarch64 -M raspi3b -cpu cortex-a53 -m 1024 -kernel build/Kernel -drive file=build/sd_hat.img,format=raw,id=sd_card1,if=none -device sd-card,drive=sd_card1 -nographic
