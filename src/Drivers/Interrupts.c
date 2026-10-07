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

#include "../Include/Interrupts.h"
#include <stdbool.h>
#include <stdint.h>

void __attribute__((interrupt("IRQ"))) irq_handler(void)
{
    uint64_t iar = GICC_IAR;
    uint64_t irq_id = iar & 0x3FF;

    switch (irq_id)
    {
    case IRQ_ID_UART_PRIMARY:
    {
        break;
    }

    case IRQ_ID_UART_SECONDARY:
    {
        break;
    }

    case IRQ_ID_SPURIOUS:
    {
        break;
    }
    case IRQ_ID_SYSTIME_0:
    {
        break;
    }
    case IRQ_ID_SYSTIME_1:
    {
        break;
    }
    case IRQ_ID_SYSTIME_2:
    {
        break;
    }
    case IRQ_ID_SYSTIME_3:
    {
        break;
    }
    case IRQ_ID_GPIO_0:
    {
        break;
    }
    case IRQ_ID_GPIO_1:
    {
        break;
    }
    case IRQ_ID_I2C:
    {
        break;
    }
    case IRQ_ID_SPI:
    {
        break;
    }
    case IRQ_ID_ARM_T:
    {
        break;
    }
    }
}