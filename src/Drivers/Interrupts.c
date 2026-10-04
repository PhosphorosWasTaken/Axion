#include "../Include/Interrupts.h"

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