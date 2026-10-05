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
