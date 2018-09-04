#include <gba.h>

#include "dump.thumb.h"
#include "irq.arm.h"
 
#define	REG_WAITCNT *(vu16 *)(REG_BASE + 0x204)

int main() {
	//IrqInit();
    REG_WAITCNT = 0x4018;
    irqInit();
    irqSet(IRQ_SERIAL, irqSerialHandler);
    irqEnable(IRQ_VBLANK | IRQ_SERIAL);

    consoleDemoInit();
    dumpInit();

    while (1) {
        VBlankIntrWait();
        dumpPrintInfo();
    } // while
    return 0; 
} // main
