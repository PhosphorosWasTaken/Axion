#pragma once

#include <stdint.h>

#ifndef BUTTONS_H
#define BUTTONS_H
// MCP23017 button interrupts
#define MCP23017_PIN  17 // pin on which the chip is connected
#define MCP23017_ADDR 0x20 // I2C bus address of the mcp23017 chip
#define IOCON         0x0A // main settings register
#define GPINTENA      0x04 // the bitmask to enable interrupts
#define INTFA         0x0E // the "who triggered interrupt" register
#define INTCAPA \
    0x08 // a snapshot state which holds the state of the button on the exact
         // millisecond it was pressed
#define GPIOA 0x12

// button memory locations

// TODO: change with actual location later
#define BUTTONSTATES  0xFF // stores the current button states in memory
#define BUTTONRISING  0xFF // stores the button rising state in memory (button has been pressed)
#define BUTTONFALLING 0xFF // stores the button falling state in memory (button was released)

void mcp23017_write(uint8_t reg, uint8_t data);
uint8_t mcp23017_read(uint8_t reg);
void setup_mcp23017_interrupts(void);
#endif