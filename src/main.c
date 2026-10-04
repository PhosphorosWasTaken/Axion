/*
Notes:
- firmware will pass control to kerne
l at EL2 or EL4 in 64-bit mode, which we are targeting.
*/

#include <stdint.h>

#include "Buttons/Buttons.h"

#define MMIO_BASE 0x3F000000

#define UART0_DR \
    ((volatile uint32_t*)(MMIO_BASE + 0x00201000)) // Data Register
#define UART0_FR \
    ((volatile uint32_t*)(MMIO_BASE + 0x00201018)) // Flag Register

#define FR_TXFF (1 << 5) // Transmit FIFO Full

#define SD_ADDR \
    ((volatile uint32_t*)0x3F300000) // SD card physical memory addr

void __attribute__((interrupt("IRQ"))) irq_handler(void)
{
    uint8_t int_flag = mcp23017_read(INTFA);

    if (int_flag != 0)
    { // make sure there was an interrupt from the button GPIO
        uint8_t captured_gpio = mcp23017_read(INTCAPA);
        (void)captured_gpio;
    }
}

void uart_putc(unsigned char c)
{
    // Wait until the transmit buffer has space
    while (*UART0_FR & FR_TXFF)
    {
        asm volatile("nop");
    }

    *UART0_DR = c;
}

void uart_puts(const char* str)
{
    while (*str)
    {
        if (*str == '\n')
            uart_putc('\r');
        uart_putc(*str++);
    }
}

void kernel_main(void)
{
    // Basic string output to UART
    uart_puts("Hello world from ARM64 Bare-Metal Kernel!\n");

    while (1)
    {
        // Infinite loop
    }
}
