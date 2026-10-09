# Copyright (C) 2026  PhosphorosWasTaken
# Copyright (C) 2026  p123o215
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://gnu.org>.
#
# See the LICENSE file in the project root for additional terms 
# appended under GPL v3 Section 7 regarding attribution screens.

.section ".text.boot"

.global _start

_start:
    // Check CPU core ID; only allow Core 0 to proceed
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbz     x0, master_core

hang:
    // Hang cores 1-3
    wfe
    b       hang

master_core:
    // Set stack pointer right before the kernel start address
    ldr     x0, =_start
    mov     sp, x0

    // Clear BSS section
    ldr     x5, =__bss_start
    ldr     x6, =__bss_end

clear_bss:
    cbz     w6, run_kernel
    str     xzr, [x5], #8
    sub     w6, w6, #1
    cbnz    w6, clear_bss

run_kernel:
    // Jump to C code
    bl      kernel_main

halt:
    // If main returns, halt
    wfe

    b       halt
