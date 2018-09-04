#include <gba_interrupt.h>
#include <gba_console.h>
#include <gba_systemcalls.h>

#include "dump.h"
#include "irq.arm.h"

int main() {
	// REG_WSCNT = 0x4018;
	//IrqInit();
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
