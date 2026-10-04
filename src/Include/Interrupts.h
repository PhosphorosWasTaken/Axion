#pragma once

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

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

void __attribute__((interrupt("IRQ"))) irq_handler(void);

#endif