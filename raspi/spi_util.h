#ifndef SPI_UTIL_H
#define SPI_UTIL_H

#include <stdint.h>

uint32_t writeSPI32NoMessage(uint32_t write_bits);
uint32_t writeSPI32(uint32_t write_bits, char *message);
uint32_t waitSPI32(uint32_t write_bits, uint32_t compare_bits, uint32_t sleep_us, char *message);

#endif
