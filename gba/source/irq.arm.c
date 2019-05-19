#include <gba.h>
#include "irq.arm.h"
#include "dump.thumb.h"

#define REG_IRQ_WAITFLAGS		*(volatile u16*)0x3007FF8
#define REG_WAITCNT				*(volatile u16*)0x4000204

// IWRAM_CODE void irqSerialHandler() {
//     REG_IME = 0;
//     u16 flag = REG_IF; 
//     REG_SIODATA32 = dumpStep();
//     REG_SIOCNT = SIO_IRQ | SIO_32BIT | SIO_START;
//     REG_IF = flag;
//     REG_IME = 1;
// } // irqSerialHandler
//---------------------------------------------------------------------------
void IrqInit(void) {
    REG_IME = 0;

    INT_VECTOR    = (IntFn)IrqHandler;
    REG_IE        = IRQ_SERIAL | IRQ_VBLANK;
    REG_SIOCNT    = SIO_IRQ | SIO_32BIT | SIO_START;
    REG_SIODATA32 = 0;
    REG_DISPSTAT  = LCDC_VBL;

    REG_IME = 1;
}
void IrqHandler(void) {
    REG_IME  = 0;
    u16 flag = REG_IF;
    
    if(flag & IRQ_SERIAL)
    {
        IrqHandlerSerial();
    }
    
    if(flag & IRQ_VBLANK)
    {
        IrqHandlerVblank();
    }
    
    REG_IF  = flag;
    REG_IME = 1;
}
void IrqHandlerSerial(void) {
    REG_SIODATA32 = dumpStep();

    REG_SIOCNT = SIO_IRQ | SIO_32BIT | SIO_START;
}
void IrqHandlerVblank(void) {
    // EMPTY

    REG_IRQ_WAITFLAGS |= IRQ_VBLANK;
}
