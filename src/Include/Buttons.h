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



/*
A couple of design decisions have been outlined in this comment to ensure future edits keep those decisions
in mind.

- Do not create structures or duplicates of button data. We know there isn't a lot of data from it, but
we ideally want to minimize the memory footprint as much as possible for maximum room, and choosing to 
deduplicate the button data is just a small part of keeping that footprint small while providing maximum
functionality.

- these functions have to be extremely fast, because they will be called every frame that a game is running.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#ifndef BUTTONS_H
#define BUTTONS_H
// MCP23017 button interrupts
#define mcp23s17_MOSI 19
#define mcp23s17_MISO 21
#define mcp23s17_CS   24

//raspberry pi 4 SPI registers

#define SPI0_BASE 0x3F204000
#define GPIO_BASE 0x3F000000

//SPI register offsets
#define SPI0_CS         0x00
#define SPI0_FIFO       0x04
#define SPI0_CLK        0x08
#define SPI0_DLEN       0x0C
#define SPI0_LTOH       0x10
#define SPI0_DC         0x14

//GPIO register offsets 
#define GPIO_FSEL0      0x00
#define GPIO_SET0       0x1C
#define GPIO_CLR0       0x28

//SPI control bits
#define SPI_CS_TA       (1 << 7)
#define SPI_CS_CLEAR    (1 << 4)
#define SPI_CS_CSPOL0   (1 << 2)
#define SPI_CS_CSPOL1   (1 << 3)
#define SPI_CS_CSPOL2   (1 << 5)

//MCP23s17 register addresses
#define MCP_IODIRA      0x00    // I/O Direction Register A
#define MCP_IODIRB      0x01    // I/O Direction Register B
#define MCP_IPOLA       0x02    // Input Polarity Register A
#define MCP_IPOLB       0x03    // Input Polarity Register B
#define MCP_GPINTENA    0x04    // Interrupt-on-Change Enable A
#define MCP_GPINTENB    0x05    // Interrupt-on-Change Enable B
#define MCP_DEFVALA     0x06    // Default Value Register A
#define MCP_DEFVALB     0x07    // Default Value Register B
#define MCP_INTCONA     0x08    // Interrupt Configuration A
#define MCP_INTCONB     0x09    // Interrupt Configuration B
#define MCP_IOCONA      0x0A    // I/O Configuration Register A
#define MCP_IOCONB      0x0B    // I/O Configuration Register B
#define MCP_GPPUA       0x0C    // Pull-up Resistor Register A
#define MCP_GPPUB       0x0D    // Pull-up Resistor Register B
#define MCP_INTFA       0x0E    // Interrupt Flag Register A
#define MCP_INTFB       0x0F    // Interrupt Flag Register B
#define MCP_INTCAPA     0x10    // Interrupt Capture Register A
#define MCP_INTCAPB     0x11    // Interrupt Capture Register B
#define MCP_GPIOA       0x12    // Port A Register
#define MCP_GPIOB       0x13    // Port B Register
#define MCP_OLATA       0x14    // Output Latch Register A
#define MCP_OLATB       0x15    // Output Latch Register B

// MCP23S17 specific bits
#define MCP_IOCON_HAEN  (1 << 3)  // Hardware Address Enable
#define MCP_IOCON_ODR   (1 << 2)  // Open Drain Output
#define MCP_IOCON_INTPOL (1 << 1) // Interrupt Polarity

// SPI device address (0x40 for MCP23S17, chip select = 0)
#define MCP_ADDR        0x40

volatile uint32_t *spi0_regs;
volatile uint32_t *gpio_regs;

// button memory locations

// TODO: change with actual location later
#define BUTTONSTATES 0xFF // stores the current button states(pressed, rising, falling) in memory

// defined memory offsets for buttons. used by & it with the button states to get a binary value
#define W             0x000000000001
#define A             W << 1
#define S             A << 1
#define D             S << 1
#define DPAD_UP       D << 1
#define DPAD_DOWN     DPAD_UP << 1
#define DPAD_LEFT     DPAD_DOWN << 1
#define DPAD_RIGHT    DPAD_LEFT << 1
#define LEFT_TRIGGER  DPAD_RIGHT << 1
#define RIGHT_TRIGGER LEFT_TRIGGER << 1

#define W_RISING             RIGHT_TRIGGER << 1
#define A_RISING             W_RISING << 1d
#define S_RISING             A_RISING << 1
#define D_RISING             S_RISING << 1
#define DPAD_UP_RISING       D_RISING << 1
#define DPAD_DOWN_RISING     DPAD_UP_RISING << 1
#define DPAD_LEFT_RISING     DPAD_DOWN_RISING << 1
#define DPAD_RIGHT_RISING    DPAD_LEFT_RISING << 1
#define LEFT_TRIGGER_RISING  DPAD_RIGHT_RISING << 1
#define RIGHT_TRIGGER_RISING LEFT_TRIGGER_RISING <<1

#define W_FALLING             RIGHT_TRIGGER_RISING << 1
#define A_FALLING             W_FALLING << 1
#define S_FALLING             A_FALLING << 1
#define D_FALLING             S_FALLING << 1
#define DPAD_UP_FALLING       D_FALLING << 1
#define DPAD_DOWN_FALLING     DPAD_UP_FALLING << 1
#define DPAD_LEFT_FALLING     DPAD_DOWN_FALLING << 1
#define DPAD_RIGHT_FALLING    DPAD_LEFT_FALLING << 1
#define LEFT_TRIGGER_FALLING  DPAD_RIGHT_FALLING << 1
#define RIGHT_TRIGGER_FALLING LEFT_TRIGGER_FALLING << 1

/*
we do not currently have other buttons that need to be pressed, so i will not add headers for them.
*/

void initialize_mcp23s17();

void delay_us(int us);

void spi_init();

void spi_transfer();

uint8_t mcp23s17_read_port(uint8_t port_reg);

void mcp23s17_write_port(uint8_t port_reg, uint8_t value);

uint8_t mcp23s17_read_interrupt_flags();

bool get_button_state(uint32_t button);

#endif