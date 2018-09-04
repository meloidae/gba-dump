#include <gba_sio.h>
#include "irq.arm.h"
#include "dump.h"

void irqSerialHandler() {
    REG_SIODATA32 = dumpStep();
	REG_SIOCNT = SIO_IRQ | SIO_32BIT | SIO_START;
} // irqSerialHandler
