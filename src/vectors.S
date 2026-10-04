.global irq_handler
irq_handler:
    // Save register state
    stp     x0, x1, [sp, #-16]!
    stp     x2, x3, [sp, #-16]!
    stp     x29, x30, [sp, #-16]!

    // Call the C handler
    bl      irq_handler

    // Restore register state
    ldp     x29, x30, [sp], #16
    ldp     x2, x3, [sp], #16
    ldp     x0, x1, [sp], #16
    eret