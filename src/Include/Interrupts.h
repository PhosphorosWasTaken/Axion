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

#pragma once

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdbool.h>
#include <stdint.h>

#define GIC_CPU_BASE 0xFF842000

#define GICC_IAR  (*(volatile uint64_t*)(GIC_CPU_BASE + 0x000C))
#define GICC_EOIR (*(volatile uint64_t*)(GIC_CPU_BASE + 0x0010))

#define IRQ_ID_UART_PRIMARY 153
#define IRQ_ID_SPURIOUS     1023 // there was an interrupt, but it was cleared before the CPU could manage it

// system timer interrupts
#define IRQ_ID_SYSTIME_0 128
#define IRQ_ID_SYSTIME_1 129
#define IRQ_ID_SYSTIME_2 130
#define IRQ_ID_SYSTIME_3 131

// GPIO interrupts
#define IRQ_ID_GPIO_0 145 // GPIO pin interrupts from pins 0-27
#define IRQ_ID_GPIO_1 146 // GPIO pin interrupts from pins 28-45

// communication interrupts
#define IRQ_ID_I2C            149
#define IRQ_ID_SPI            157
#define IRQ_ID_UART_SECONDARY 161

#define IRQ_ID_ARM_T 96

// variables and functions for registering callback functions

#define SYSTIME0_CALLBACK 0xFF // TODO: replace all these values with their actual location in RAM later
#define SYSTIME1_CALLBACK 0xFF // TODO: keep in mind this location must align with an 8-byte offset because of direct writing issues.
#define SYSTIME2_CALLBACK 0XFF
#define SYSTIME3_CALLBACK 0XFF

#define IRQ_I2C_CALLBACK      0XFF
#define IRQ_SPI_CALLBACK      0XFF
#define IRQ_ARM_T_CALLBACK    0XFF
#define IRQ_UART_SEC_CALLBACK 0XFF
#define IRQ_UART_PRI_CALLBACK 0XFF

void __attribute__((interrupt("IRQ"))) irq_handler(void);

#endif