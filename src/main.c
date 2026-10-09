// Copyright (C) 2026  PhosphorosWasTaken
// Copyright (C) 2026  p123o215
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://gnu.org>.
//
// See the LICENSE file in the project root for additional terms 
// appended under GPL v3 Section 7 regarding attribution screens.

/* Notes: - firmware will pass control to kernel at EL2 or EL4 in 64-bit mode, which we are targeting. */

#include <stdint.h>

#include "Include/Buttons.h"
#include "Include/Interrupts.h"
#include "Include/vfs.h"

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

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outw(uint16_t port, uint16_t val) {
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

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

    char command[128] = {' '};

    int letter = 0;

    uart_putc('h');

    write("name", "hello", 1);

    uart_putc('t');

    while (1) {
        char input = uartGetChar(); // Read user input

        command[letter] = input;

        if (input == '\r') {

            letter = 0;

            uart_putc('|');

            for(int i = 0; i < 128; i++) {
                uart_putc(command[i]);
            }

            for (int t = 0; t <128; t++) {
                command[t] = 0;
            }
        }

        uart_putc(input);

        letter++;
    }
}
