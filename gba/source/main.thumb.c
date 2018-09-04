#include <gba.h>

#include "dump.thumb.h"
#include "irq.arm.h"

int main() {
	//IrqInit();
    irqInit();
    irqEnable(IRQ_VBLANK | IRQ_SERIAL);
    irqSet(IRQ_SERIAL, irqSerialHandler);

    consoleDemoInit();
    dumpInit();

    while (1) {
        VBlankIntrWait();
        dumpPrintInfo();
    } // while
    return 0; 
} // main
