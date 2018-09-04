#include <stdio.h>
#include <stdlib.h>
#include "spi_util.h"
#include "multiboot.h"

char default_mb[] = "../gba/gba_dump_mb.gba";
char default_dest[] = "../rom.bin";

static uint32_t crc_mem;

void crcReset() {
    crc_mem = 0x0000c387;
} // crcReset

uint32_t crc32NoReset(uint32_t data) {
    uint32_t i;
    for (i = 0; i < 32; i++) {
        if ((crc_mem ^ data) & 0x01) {
            crc_mem >>= 1;
            crc_mem ^= 0x0000c37b;
        } else {
            crc_mem >>= 1;
        } // else
        data >>= 1;
    } // for
    return crc_mem;
} // crc32NoReset


uint32_t crc32(uint32_t data) {
    crcReset();
    return crc32NoReset(data);
} // crc32


int main(int argc, char *argv[]) {
    char *mb_filename;
    char *dest_filename;

    if (argc < 3) {
        dest_filename = default_dest;
    } else {
        dest_filename = argv[2];
    } // else

    if (argc < 2) {
        mb_filename = default_mb;
    } else {
        mb_filename = argv[1];
    } // if

    // Load multiboot file
    int ret = multiboot(mb_filename);

    if (ret < 0) {
       fprintf(stderr, "Multiboot failed\n");
       exit(1);
    } // if

    waitSPI32(0x00000000, 0x1234abcd, 10000, "Wait for GBA to calculate ROM size");

    uint32_t size = writeSPI32(0x00000000, "size");
    uint32_t size_crc = writeSPI32(0x00000000, "size_crc");
    uint32_t crc = crc32(size);

    if (crc != size_crc) {
        fprintf(stderr, "Error: size_crc mismatch\n");
        exit(1);
    } // if

    fprintf(stdout, "size_crc OK\n");

    FILE *fp = fopen(dest_filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", dest_filename);
        exit(1);
    } // if

    uint32_t table[64];
    uint32_t count = 0;
    int i = 0;

    fprintf(stdout, "Transferring ROM data\n");
    crcReset();

    for (i = 0; i < size / 4; i++) {
        table[count] = writeSPI32NoMessage(0x00000000);
        crc = crc32NoReset(table[count++]);
        if (count >= 64) {
            fwrite(&table, sizeof(uint32_t), 64, fp);
            count = 0;
        } // if
        usleep(100);
    } // for

    fclose(fp);

    uint32_t data_crc = writeSPI32(0x00000000, "data_crc");
    if (crc != data_crc) {
        fprintf(stderr, "Error: data_crc mismatch\n");
        exit(1);
    } // if

    fprintf(stdout, "data_crc OK\n");
    fprintf(stdout, "All done!\n");

} // main
