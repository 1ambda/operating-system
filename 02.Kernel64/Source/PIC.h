#ifndef __PIC_H__
#define __PIC_H__

#include "Types.h"

#define PIC_MASTER_PORT1		0x20
#define PIC_MASTER_PORT2		0x21
#define PIC_SLAVE_PORT1			0xA0
#define PIC_SLAVE_PORT2			0xA1

#define PIC_MASTER_IRQ_VECTOR_BASE		0x20 // from 0x20 ~ 0x27
#define PIC_SLAVE_IRQ_VECTOR_BASE		0x28 // from 0x28 ~ 0x2F

void kPicMaskInterrupt(WORD wIRQs);
void kPicSendEOI(int nIRQ);
void kPicInitialize(void);

#endif
