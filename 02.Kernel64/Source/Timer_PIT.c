#include "Timer.h"

void kPitInitialize(WORD wCount, BOOL bPeriodic)
{
	kWritePortByte(PIT_PORT_CONTROL, PIT_COUNTER0_ONCE);

	if ( bPeriodic == TRUE )
	{
		kWritePortByte(PIT_PORT_CONTROL, PIT_COUNTER0_PERIODIC);
	}

	kWritePortByte(PIT_PORT_COUNTER0, wCount); // LSB
	kWritePortByte(PIT_PORT_COUNTER0, wCount >> 8 ); // MSB
}

WORD kPitReadCounter0(void)
{
	BYTE bHigh, bLow;
	WORD wTemp = 0;

	kWritePortByte(PIT_PORT_CONTROL, PIT_COUNTER0_LATCH);

	bLow = kReadPortByte(PIT_PORT_COUNTER0);
	bHigh = kReadPortByte(PIT_PORT_COUNTER0);

	wTemp = bHigh;
	wTemp = (wTemp << 8) | bLow;

	return wTemp;
}


// This function can measure about 50ms at most
void kPitWait(WORD wCount)
{
	WORD wLast, wCurrent;

	// 0xFFFF, Periodic
	kPitInitialize(0, TRUE);

	wLast = kPitReadCounter0();

	while(TRUE)
	{
		wCurrent = kPitReadCounter0();

		if ( ((wLast - wCurrent) & 0xFFFF ) >= wCount )
		{
			break;
		}
	}
}
