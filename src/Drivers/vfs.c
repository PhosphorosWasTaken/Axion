#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#define SD_DUMMY_BYTE 0xFF
#define SD_START_TOKEN 0xFE

#define MMIO_BASE 0xFE000000
#define SPI0_BASE (MMIO_BASE + 0x204000)

#define SPI_CS (*(volatile uint32_t *)(SPI0_BASE + 0x00))
#define SPI_FIFO (*(volatile uint32_t *)(SPI0_BASE + 0x04))

// Register bit masks
#define SPI_CS_TA       (1 << 7)   // Transfer Active
#define SPI_CS_CLEAR_RX (1 << 5)   // Clear RX FIFO
#define SPI_CS_CLEAR_TX (1 << 4)   // Clear TX FIFO
#define SPI_CS_DONE     (1 << 16)  // Transfer Done
#define SPI_CS_RXR      (1 << 17)  // RX FIFO contains data
#define SPI_CS_TXD      (1 << 18)  // TX FIFO can accept data

void *memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;

    while (n--) {
        *p-- = (unsigned char)c;
    }

    return s;
}

char *strncpy(char *dest, const char *src, size_t n) {
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    for (; i < n; i++) {
        dest[i] = '\0';
    }

    return dest;
}

void sd_chip_select(bool enable) {
    if (enable) {
        SPI_CS |= (1 << 0);
    } else {
        SPI_CS &= ~(1 << 0);
    }
}

uint8_t spi_transfer_byte(uint8_t data) {
    SPI_CS |= SPI_CS_TA | SPI_CS_CLEAR_RX | SPI_CS_CLEAR_TX;

    while (!(SPI_CS & SPI_CS_TXD));

    SPI_FIFO = data;

    while (!(SPI_CS & SPI_CS_DONE));

    return (uint8_t)(SPI_FIFO & 0xFF);
}

void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms *10000; i++) {
        __asm volatile("nop");
    }
}

static uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc) {
    sd_chip_select(true);

    spi_transfer_byte(0x40 | cmd);
    spi_transfer_byte((arg >> 24) & 0xFF);
    spi_transfer_byte((arg >> 16) & 0xFF);
    spi_transfer_byte((arg >> 8) & 0xFF);
    spi_transfer_byte(arg & 0xFF);
    spi_transfer_byte(crc);

    uint8_t res = 0xFF;

    for (int i = 0; i < 10; i++) {
        res = spi_transfer_byte(SD_DUMMY_BYTE);

        if ((res & 0x80) == 0) break;
    }

    return res;
}

bool is_sdhc_sdxc = false;

int sd_init(void) {
    sd_chip_select(false);

    uart_putc('q');

    for (int i = 0; i < 10; i++) {
        spi_transfer_byte(SD_DUMMY_BYTE);
    }

    sd_chip_select(true);

    uart_putc('w');

    uint8_t res = sd_send_cmd(0, 0, 0x95);

    sd_chip_select(false);

    spi_transfer_byte(SD_DUMMY_BYTE);

    if (res != 0x01) {
        return -1;
    }

    sd_chip_select(true);

    res = sd_send_cmd(8, 0x000001AA, 0x87);

    uart_putc('e');

    if (res == 0x01) {
        uint32_t r7 = 0;

        for (int i = 0; i < 4; i++) {
            r7 = (r7 << 8) | spi_transfer_byte(SD_DUMMY_BYTE);
        }

        sd_chip_select(false);

        if ((r7 & 0xFF) != 0xAA) {
            return -2;
        }
    } else {
        sd_chip_select(false);
    }

    uart_putc('r');

    uint32_t timeout = 1000;

    while (timeout--) {
        sd_chip_select(true);
        sd_send_cmd(55, 0, 0xFF);
        sd_chip_select(false);

        sd_chip_select(true);
        res = sd_send_cmd(41, 0x40000000, 0xFF);
        sd_chip_select(false);

        if (res == 0x00) break;

        delay_ms(1);
    }

    uart_putc('t');

    if (res != 0x00) return -3;

    sd_chip_select(true);

    res = sd_send_cmd(58, 0, 0xFF);

    if (res == 0x00) {
        uint8_t ocr_byte0 = spi_transfer_byte(SD_DUMMY_BYTE);

        spi_transfer_byte(SD_DUMMY_BYTE);
        spi_transfer_byte(SD_DUMMY_BYTE);
        spi_transfer_byte(SD_DUMMY_BYTE);

        is_sdhc_sdxc = (ocr_byte0 & 0x40) != 0;
    }

    sd_chip_select(false);

    return 0;
}

int sd_write_block(uint32_t sector_num, const uint8_t *buffer) {
    uint32_t addr = is_sdhc_sdxc ? sector_num : (sector_num * 512);

    sd_chip_select(true);

    if (sd_send_cmd(24, addr, 0xFF) != 0x00) {
        sd_chip_select(false);

        return -1;
    }

    spi_transfer_byte(SD_START_TOKEN);

    for (int i = 0; i < 512; i++) {
        spi_transfer_byte(buffer[i]);
    }

    spi_transfer_byte(0xFF);
    spi_transfer_byte(0xFF);

    uint8_t resp = spi_transfer_byte(SD_DUMMY_BYTE);

    if ((resp & 0x1F) != 0x05) {
        sd_chip_select(false);

        return -2;
    }

    int timeout = 500000;

    while (spi_transfer_byte(SD_DUMMY_BYTE) == 0x00 && --timeout);

    sd_chip_select(false);
    spi_transfer_byte(SD_DUMMY_BYTE);

    return (timeout > 0) ? 0 : -3;
}

int sd_read_block(uint32_t sector_num, uint8_t *buffer) {
    uint32_t addr = is_sdhc_sdxc ? sector_num : (sector_num *512);

    sd_chip_select(true);

    if (sd_send_cmd(17, addr, 0xFF) != 0x00) {
        sd_chip_select(false);

        return -1;
    }

    uint8_t token = 0xFF;
    uint32_t timeout = 100000;

    while (timeout--) {
        token = spi_transfer_byte(SD_DUMMY_BYTE);

        if (token != 0xFF) break;
    }

    if (token != SD_START_TOKEN) {
        sd_chip_select(false);

        return -2;
    }

    for (int i = 0; i < 512; i++) {
        buffer[i] = spi_transfer_byte(SD_DUMMY_BYTE);
    }

    spi_transfer_byte(SD_DUMMY_BYTE);
    spi_transfer_byte(SD_DUMMY_BYTE);

    sd_chip_select(false);
    spi_transfer_byte(SD_DUMMY_BYTE);

    return 0;
}

// The actual pointer to the file on the storage media
typedef struct {
    uint64_t start_addr;  // Address to the start of the contents
    uint64_t end_addr;    // Address to the end of the contents

    char name[64];        // Name of the file

    int type;             // file (0) or folder (1)
    uint8_t padding[424]; // Padding

    int valid;            // If the struct is valid
} Inode;

Inode inode_table[1024]; // List of each files inodes

unsigned char start_addr = 0x58; // Starting address of the file to write

int read(const char* name, uint8_t *read_buf[512]) {
    int status = sd_read_block(100, read_buf);

    if (status != 0) {
        return 3;
    }

    read_buf[512 -1] = '\0';

    return 0;
}

int write(const char* name, const char* data, int type) {
    uart_putc('k');

    if (sd_init() != 0) {
        return 1;
    }

    uart_putc('u');

    uint8_t write_buf[512] = {0};

    const char *message = "hello";

    strncpy((char *)write_buf, message, sizeof(write_buf) -1 );

    uart_putc('y');

    int status = sd_write_block(100, write_buf);

    if (status != 0) {
        return 2;
    }

    return 0;
}

void readInodeTable(Inode new_inode_table[1024]) {

}
