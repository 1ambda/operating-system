#include "InterruptHandler.h"

void kTimerHandler(int nVector)
{
	char buffer[] = "[INT:  , ]";
	static int gCountTimerInterrupt = 0;

	buffer[5] = '0' + nVector / 10;
	buffer[6] = '0' + nVector % 10;
	buffer[8] = '0' + gCountTimerInterrupt;

	gCountTimerInterrupt = (gCountTimerInterrupt + 1) % 10;
	kPrintStringXY(70, 0, buffer);

	kPicSendEOI(nVector - PIC_MASTER_IRQ_VECTOR_BASE);

	gTickCount++;

	kTaskDecreaseTimeslice();

	if ( kTaskIsTimesliceExpired() )
	{
		kTaskScheduleInterrupt();
	}
}

void kHandlerCommonException(int nVector, QWORD qwErrorCode)
{
	char buffer[3] = { 0 };

	buffer[0] = '0' + nVector / 10;
	buffer[1] = '0' + nVector % 10;

	kPrintStringXY(0, 0, "=================");
	kPrintStringXY(0, 1, "Exception Occured");
	kPrintStringXY(0, 2, buffer);
	kPrintStringXY(0, 3, "=================");

	while(1);
}

void kHandlerCommonInterrupt(int nVector)
{
	char buffer[] = "[INT:  , ]";
	static int gCountCommonInterrupt = 0;

	buffer[5] = '0' + nVector / 10;
	buffer[6] = '0' + nVector % 10;
	buffer[8] = '0' + gCountCommonInterrupt;

	gCountCommonInterrupt = (gCountCommonInterrupt + 1) % 10;

	kPrintStringXY(70, 0, buffer);

	kPicSendEOI(nVector - PIC_MASTER_IRQ_VECTOR_BASE);
}

void kHandlerKeyboardInterrupt(int nVector)
{
	char buffer[] = "[INT:  , ]";
	BYTE bTemp;
	static int gCountKeyboardInterrupt= 0;

	buffer[5] = '0' + nVector / 10;
	buffer[6] = '0' + nVector % 10;
	buffer[8] = '0' + gCountKeyboardInterrupt;

	gCountKeyboardInterrupt = (gCountKeyboardInterrupt + 1) % 10;

	if (kIsOutputBufferFull() == TRUE)
	{
		bTemp = kGetKeyboardScanCode();
		kConvertScanCodeAndPutQueue(bTemp);
	}

	kPrintStringXY(0, 0, buffer);

	kPicSendEOI(nVector - PIC_MASTER_IRQ_VECTOR_BASE);
}
