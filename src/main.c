/*
Notes:
- firmware will pass control to kernel at EL2 or EL4 in 64-bit mode, which we are targeting.
*/

#include <stdint.h>

#include "Include/Buttons.h"
#include "Include/Interrupts.h"

#define MMIO_BASE 0x3F000000

#define UART0_DR \
    ((volatile uint64_t*)(MMIO_BASE + 0x00201000)) // Data Register
#define UART0_FR \
    ((volatile uint64_t*)(MMIO_BASE + 0x00201018)) // Flag Register

#define SD_ADDR \
    ((volatile uint64_t*)0x3F300000) // SD card physical memory addr

#define UART0_IBRD  ((volatile uint32_t*)(MMIO_BASE + 0x00201024)) // Integer Baud Rate Divider
#define UART0_FBRD  ((volatile uint32_t*)(MMIO_BASE + 0x00201028)) // Fractional Baud Rate Divider
#define UART0_LCRH  ((volatile uint32_t*)(MMIO_BASE + 0x0020102C)) // Line Control Register
#define UART0_CR    ((volatile uint32_t*)(MMIO_BASE + 0x00201030)) // Control Register
#define UART0_ICR   ((volatile uint32_t*)(MMIO_BASE + 0x00201044)) // Interrupt Clear Register

#define FR_RXFF   (1 << 6)                                       // Recieve FIFO Full
#define FR_RXFE   (1 << 4)                                       // Recieve FIFO Empty
#define FR_TXFF   (1 << 5)                                       // Transmit FIFO Full
#define FR_TXFE   (1 << 7)                                       // Transmit FIFO Empty

// Get the UART ready for use
void uartInit(void) {
    *UART0_CR = 0; // Disable UART while editting commands

    *UART0_ICR = 0x7FF; // Clear all pending interrupts

    // Set Baud Rate to 115200
    *UART0_IBRD = 26;
    *UART0_FBRD = 3;

    *UART0_LCRH = (1 << 4) | (1 << 5) | (1 << 6); // Enable FIFOs (bit 4) and set 8-bit word length (bits 5 and 6)

    *UART0_CR = (1 << 0) | (1 << 8) | (1 << 9); // Enable UART0. TXE, and RXE
}

// Output character to serial connection
void uart_putc(unsigned char c) {
    // Wait until the transmit buffer has space
    while(*UART0_FR & FR_TXFF) {
        asm volatile("nop"); // Do nothing until then
    }

    *UART0_DR = c; // Write the character to the UART buffer
}

// Output string to serial connection
void uart_puts(const char* str) {
    // Get every character in the dereferenced string
    while (*str) {
        if (*str == '\n') uart_putc('\r'); // If there is a return line, move the cursor to the beginning of the line before moving down a line
        uart_putc(*str++); // Write the dereferenced character in the string to the serial (UART) buffer
    }
}

// Get the typed character
char uartGetChar(void) {
    // Wait until the UART buffer is full
    while (*UART0_FR & FR_RXFE) {
        asm volatile("nop"); // Do nothing until then
    }

    return (char)(*UART0_DR & 0xFF); // Return the typed character
}

// Main loop
void kernel_main(void) {
    uartInit(); // Configure UART

    while (1) {
        char input = uartGetChar(); // Read user input

        uart_putc(input);
    }
}
