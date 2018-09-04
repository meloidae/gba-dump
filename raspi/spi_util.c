#include <stdio.h>
#include "spi_util.h"

uint32_t writeSPI32NoMessage(uint32_t write_bits) {
    uint8_t buffer[4];

    // Construct bits to write
    buffer[3] = write_bits & 0x000000ff;
    buffer[2] = (write_bits & 0x0000ff00) >> 8;
    buffer[1] = (write_bits & 0x00ff0000) >> 16;
    buffer[0] = (write_bits & 0xff000000) >> 24;

    // write/read
    wiringPiSPIDataRW(0, &buffer, 4);

    uint32_t read_bits = 0;
    read_bits |= buffer[0] << 24;
    read_bits |= buffer[1] << 16;
    read_bits |= buffer[2] << 8;
    read_bits |= buffer[3];

    return read_bits;
} // writeSPI32NoMessage

uint32_t writeSPI32(uint32_t write_bits, char *message) {
    uint32_t read_bits = writeSPI32NoMessage(write_bits);

    fprintf(stdout, "sent: 0x%08x, received: 0x%08x; %s\n", write_bits, read_bits, message);

    return read_bits;
} // writeSPI32

uint32_t waitSPI32(uint32_t write_bits, uint32_t compare_bits, uint32_t sleep_us, char *message) {
    fprintf(stdout, "%s 0x%08x\n", message, compare_bits); 
    uint32_t read_bits;

    while (1) {
        read_bits = writeSPI32NoMessage(write_bits);
        if (read_bits != compare_bits) {
            fprintf(stdout, "%s 0x%08x\n", "read_bits:", read_bits); 
            usleep(sleep_us);
            continue;
        } // if
        break;
    } // while

} // waitSPI32

