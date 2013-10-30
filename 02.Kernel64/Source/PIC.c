#include "PIC.h"


void kPicInitialize(void)
{
	// Master PIC
	// ICW1
	kWritePortByte(PIC_MASTER_PORT1, 0x11);
	// ICW2
	kWritePortByte(PIC_MASTER_PORT2, PIC_MASTER_IRQ_VECTOR_BASE);
	// ICW3
	kWritePortByte(PIC_MASTER_PORT2, 0x04); // Bit 2, Slave PIC is connected with Master's IRQ2
	// ICW4
	kWritePortByte(PIC_MASTER_PORT2, 0x01); // Bit 0, uPM = 8086/88

	// Slave PIC
	// ICW1
	kWritePortByte(PIC_SLAVE_PORT1, 0x11);
	// ICW2
	kWritePortByte(PIC_SLAVE_PORT2, PIC_SLAVE_IRQ_VECTOR_BASE);
	// ICW3
	kWritePortByte(PIC_SLAVE_PORT2, 0x02);
	// ICW4
	kWritePortByte(PIC_SLAVE_PORT2, 0x01);

	return;
}

void kPicMaskInterrupt(WORD wIRQs)
{
	// Master PIC
	// OCW1
	kWritePortByte(PIC_MASTER_PORT2, (BYTE) wIRQs);

	// Slave PIC
	// OCW1
	kWritePortByte(PIC_SLAVE_PORT2, (BYTE) (wIRQs >> 8));

	return;
}

void kPicSendEOI(int nIRQ)
{
	// to Master PIC
	// OCW2
	kWritePortByte(PIC_MASTER_PORT1, 0x20); // Non-specific EOI

	if ( nIRQ >= 8 )
	{
		// to Slave PIC
		// OCW2
		kWritePortByte(PIC_SLAVE_PORT1, 0x20);
	}

	return;
}
