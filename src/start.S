.section ".text.boot"

.global _start

_start:
    // Check CPU core ID; only allow Core 0 to proceed
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbz     x0, master_core

    // Secondary cores halt here
halt:
    wfe
    b       halt

master_core:
    // Set stack pointer right before the kernel start address
    ldr     x0, =_start
    mov     sp, x0

    // Clear BSS section
    ldr     x0, =__bss_start
    ldr     x1, =__bss_size
clear_bss:
    cbz     x1, run_kernel
    str     xzr, [x0], #8
    sub     x1, x1, #1
    cbnz    x1, clear_bss

run_kernel:
    // Jump to C code
    bl      kernel_main

    // If main returns, halt
    b       halt