#include "Types.h"
#include "Utility.h"
#include "Descriptor.h"
#include "InlineAssemblyUtility.h"
#include "Keyboard.h"
#include "PIC.h"
#include "Console.h"
#include "ConsoleShell.h"
#include "Task.h"
#include "Timer.h"


void Main(void)
{
	int cursorX, cursorY;

	kConsoleInitialize(0, 15);

	kPrintf("Loading IA32-e Kernel64 written by C.........[PASS]\n");

	if ( kInitializeGDTandTSS() == FALSE )
	{
		kPrintf("Initialization GDT & TSS.....................[FAIL]\n");
		while(1);
	}

	kPrintf("Initialization GDT & TSS.....................[PASS]\n");

	kLoadGDTRAsm(START_ADDRESS_GDTR);

	if ( kInitializeIDT () == FALSE )
	{
		kPrintf("Initialization IDT...........................[FAIL]\n");
		while(1);
	}

	kLoadTRAsm(GDT_OFFSET_TSS);
	kLoadIDTRAsm(START_ADDRESS_IDTR);
	kPrintf("Initialization IDT...........................[PASS]\n");
	kCheckTotalSizeofRAM();
	kPrintf("Available size of RAM........................[");
	kPrintf("%dMB]\n", kGetTotalSizeofRAM());

	kTaskInitialize();
	kPrintf("Task Scheduler Initialization................[PASS]\n");
	kPitInitialize(PIT_MILLI_TO_COUNT(1), 1);

	if ( kKeyboardInitialize() == TRUE ) {
		kPrintf("Keyboard Activation and Key Q Initialization [PASS]\n");
		kChangeKeyboardLED(FALSE, FALSE, FALSE);
	}

	kPicInitialize();
	kPicMaskInterrupt(0);
	kEnableInterrupt();
	kPrintf("PIC Controller and Interrupt Initialization..[PASS]\n");

	kTaskCreate( TASK_PRIOIRTY_LOWEST | TASK_FLAG_IDLE, (QWORD) kTaskIdle);
	kConsoleShellStart();
}


