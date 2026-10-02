#include <stdint.h>

#define MMIO_BASE 0x3F000000

#define UART0_DR  ((volatile uint32_t*)(MMIO_BASE + 0x00201000)) // Data Register
#define UART0_FR  ((volatile uint32_t*)(MMIO_BASE + 0x00201018)) // Flag Register

#define FR_TXFF   (1 << 5)                                       // Transmit FIFO Full

#define SD_ADDR   0x3F300000                                     // SD card physical memory addr

void uart_putc(unsigned char c) {
    // Wait until the transmit buffer has space
    while(*UART0_FR & FR_TXFF) {
        asm volatile("nop");
    }

    *UART0_DR = c;
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
