#include <gba_console.h>
#include <gba_types.h>
#include <gba_base.h>
#include <string.h>
#include <stdio.h>
#include "dump.thumb.h"

DumpHolder dump;

void dumpInit() {
    memset(&dump, 0x00, sizeof(DumpHolder));
    u16 *ptr_start = (u16 *)DUMP_ROM_START;
    u16 *ptr_end = (u16 *)DUMP_ROM_END - 1;
    u16 hex = 0xffff;


    while (ptr_start < ptr_end) {
        if (*ptr_end-- != hex--) {
            break;
        } // if
    } // while

    dump.size = (((u32)ptr_end + 0xff) & 0xffffff00) - DUMP_ROM_START;
    
    dumpResetCRC();
    dump.size_crc = dumpCalcCRC(dump.size);

    dump.state = DUMP_STATE_LINK;
} // dumpInit

u32 dumpStep() {
    switch(dump.state) {
        case DUMP_STATE_LINK:
            dump.state = DUMP_STATE_SIZE;
            return 0x1234abcd;

        case DUMP_STATE_SIZE:
            dump.state = DUMP_STATE_SIZE_CRC;
            return dump.size;

        case DUMP_STATE_SIZE_CRC:
            dumpResetCRC();
            dump.state = DUMP_STATE_DATA;
            return dump.size_crc;

        case DUMP_STATE_DATA: ;
            u32 data ALIGN(4);
            data = *(u32 *)(DUMP_ROM_START + dump.send_count);
            dump.send_count += 4;
            dump.data_crc = dumpCalcCRC(data);

            if (dump.send_count >= dump.size) {
                dump.state = DUMP_STATE_DATA_CRC;
            } // if
            return data;

        case DUMP_STATE_DATA_CRC:
            dump.state = DUMP_STATE_END;
            return dump.data_crc;
        default:
            break;
    } // switch

    return 0xdeadbeef;

} // dumpStep

void dumpResetCRC() {
    dump.crc = 0x0000c387;
} // dumpResetCRC

u32 dumpCalcCRC(u32 data) {
    u32 i;
    for (i = 0; i < 32; i++) {
        if ((dump.crc & data) & 0x01) {
            dump.crc >>= 1;
            dump.crc ^= 0x0000c37b;
        } else {
            dump.crc >>= 1;
        } // else
        data >>= 1;
    } // for

    return dump.crc;
} // dumpCalcCRC

void dumpPrintInfo(void) {
	iprintf("\x1b[0;0HGBA ROM Dumper\n");

    iprintf("\x1b[2;0H size       = 0x%08lx", dump.size);
    iprintf("\x1b[3;0H size_crc   = 0x%04lx", dump.size_crc);

    iprintf("\x1b[5;0H send_count = 0x%08lx", dump.send_count);
    iprintf("\x1b[6;0H data_crc   = 0x%04lx", dump.data_crc);
} // dumpPrintInfo


