#ifndef DUMP_H
#define DUMP_H

#include <gba_types.h>

#define DUMP_ROM_START		0x08000000
#define DUMP_ROM_END		0x0a000000

enum {
    DUMP_STATE_LINK,
    DUMP_STATE_SIZE,
    DUMP_STATE_SIZE_CRC,
    DUMP_STATE_DATA,
    DUMP_STATE_DATA_CRC,
    DUMP_STATE_END,
};

typedef struct {
    u32 state;

    u32 size_crc;
    u32 size;

    u32 send_count;
    u32 data_crc;

    u32 crc;
} DumpHolder;

void dumpInit();
u32 dumpStep();
void dumpResetCRC();
u32 dumpCalcCRC(u32 data);
void dumpPrintInfo();


#endif
