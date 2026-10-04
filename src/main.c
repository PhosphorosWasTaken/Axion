#include <stdint.h>

#define MMIO_BASE 0x3F000000

#define UART0_DR  ((volatile uint32_t*)(MMIO_BASE + 0x00201000)) // Data Register
#define UART0_FR  ((volatile uint32_t*)(MMIO_BASE + 0x00201018)) // Flag Register

#define FR_RXFF   (1 << 6)                                       // Recieve FIFO Full
#define FR_RXFE   (1 << 4)                                       // Recieve FIFO Empty
#define FR_TXFF   (1 << 5)                                       // Transmit FIFO Full
#define FR_TXFE   (1 << 7)                                       // Transmit FIFO Empty

#define SD_ADDR   0x3F300000                                     // SD card physical memory address

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
    while (1) {
        char input = uartGetChar(); // Read user input

        uart_putc(input);
    }
}
