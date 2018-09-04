#include <gba_sio.h>
#include <gba_types.h>
#include <gba_interrupt.h>
#include "irq.arm.h"
#include "dump.h"

void irqSerialHandler() {
    REG_IME = 0;
    u16 flag = REG_IF; 
    REG_SIODATA32 = dumpStep();
	REG_SIOCNT = SIO_IRQ | SIO_32BIT | SIO_START;
    REG_IF = flag;
    REG_IME = 1;
} // irqSerialHandler
