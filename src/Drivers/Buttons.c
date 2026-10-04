#pragma once

#include <stdint.h>
#include "../Include/Buttons.h"


void mcp23017_write(uint8_t reg, uint8_t data);
uint8_t mcp23017_read(uint8_t reg);

void setup_mcp23017_interrupts(void)
{
    // configure IOCON to output trigger through both interrupt pins
    mcp23017_write(IOCON, 0x40);

    // trigger an interrupt if any pin on chip A switches state
    mcp23017_write(GPINTENA, 0xFF);

    // trigger interrupt whenever button goes from high to low or low to high.
    mcp23017_write(0x08, 0x00);

    // read the event buffer to clear out garbage data
    mcp23017_read(INTCAPA);
}