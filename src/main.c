#include <stdint.h>

#define MMIO_BASE 0xFE000000

//UART register offsets
#define AUX_ENABLES    ((volatile uint32_t*)(MMIO_BASE + 0x00215004))
#define AUX_MU_IO_REG  ((volatile uint32_t*)(MMIO_BASE + 0x00215040))
#define AUX_MU_LSR_REG ((volatile uint32_t*)(MMIO_BASE + 0x00215054))
//MCP23017 button interrupts
#define MCP23017_PIN   17   //pin on which the chip is connected
#define MCP23017_ADDR  0x20 //I2C bus address of the mcp23017 chip
#define IOCON          0x0A //main settings register
#define GPINTENA       0x04 //the bitmask to enable interrupts
#define INTFA          0x0E //the "who triggered interrupt" register
#define INTCAPA        0x08 // a snapshot state which holds the state of the button on the exact millisecond it was pressed
#define GPIOA          0x12

//memory addresses which store current button states

#define LOWER_BUTTONS ((volative uint32_t*)(0xFF))//TODO: replace with actual address
#define HIGHER_BUTTONS ((volatile uint32_t*)(0xFF))//TODO: replace with actual address

void mcp23017_write(uint8_t reg, uint8_t data);
uint8_t mcp23017_read(uint8_t reg);

void setup_mcp23017_interrupts(void){
    //configure IOCON to output trigger through both interrupt pins
    mcp23017_write(IOCON, 0x40);

    //trigger an interrupt if any pin on chip A switches state
    mcp23017_write(GPINTENA, 0xFF);

    //trigger interrupt whenever button goes from high to low or low to high.
    mcp23017_write(0x08, 0x00);

    //read the event buffer to clear out garbage data
    mcp23017_read(INTCAPA);
}


void __attribute__((interrupt("IRQ"))) irq_handler(void){
    uint8_t int_flag = mcp23017_read(INTFA);

    if(int_flag != 0){//make sure there was an interrupt from the button GPIO
        uint8_t captured_gpio = mcp23017_read(INTCAPA);
        (void)captured_gpio;
    }
}


void uart_putc(char c) {
    // Wait until transmitter buffer is empty
    while(!(*AUX_MU_LSR_REG & 0x20));
    *AUX_MU_IO_REG = c;
}

void uart_puts(const char* str) {
    while (*str) {
        if (*str == '\n') uart_putc('\r');
        uart_putc(*str++);
    }
}

void kernel_main(void) {
    // Basic string output to UART
    uart_puts("Hello world from ARM64 Bare-Metal Kernel!\n");

    while (1) {
        // Infinite loop
    }
}