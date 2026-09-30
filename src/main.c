#include <stdint.h>

#define MMIO_BASE 0xFE000000

//UART register offsets
#define AUX_ENABLES ((volatile uint32_t*)(MMIO_BASE + 0x00215004))
#define AUX_MU_IO_REG ((volatile uint32_t*)(MMIO_BASE + 0x00215040))
#define AUX_MU_LSR_REG ((volatile uint32_t*)(MMIO_BASE + 0x00215054))


void uart_putc(char c) {
    // Wait until transmitter buffer is empty
    while(!(*AUX_MU_LSR_REG & 0x20));
    *AUX_MU_IO_REG = c;
}

void uart_puts(const char* str) {
    while (*str) {
        if (*str == '\n') uart_putc('\r');
        uart_putc(*str++);
    }
}

void kernel_main(void) {
    // Basic string output to UART
    uart_puts("Hello world from ARM64 Bare-Metal Kernel!\n");

    while (1) {
        // Infinite loop
    }
}