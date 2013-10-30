#include "ConsoleShell.h"
#include "Console.h"
#include "Keyboard.h"
#include "Utility.h"
#include "Timer.h"
#include "Synchronization.h"


SHELL_COMMAND_ENTRY	gCommandEntry[] =
{
		{ "help",			"Show Help",	kCmdHelp	},
		{ "cls",			"Clear Screen",	kCmdCls		},
		{ "totalram",		"Show Total Ram Size",		kCmdShowTotalRamSize },
		{ "strtod",			"String To Decimal/Hex",	kCmdStringToDecimalHex },
		{ "shutdown",		"Shutdown and Reboot",		kCmdShutdown },
		{ "settimer",		"Set PIT counter0, ex) settimer [time(ms)] [periodic=1]",	kCmdSetTimer },
		{ "wait",			"Wait milliseconds using PIT ex) wait [time(ms)]",			kCmdWaitUsingPIT },
		{ "rdtsc",			"Read Time Stamp Counter",									kCmdReadTimeStampCounter},
		{ "cpuspeed",		"Measure Processor Speed", 									kCmdMeasureCpuSpeed},
		{ "date", 			"Show Date And Time",										kCmdShowDateAndTime},
		{ "createtask", 	"Create Tasks ex) createtask [type(1 or 2)] [count]",		kCmdCreateTask },
		{ "changepriority", "Change Task Priority ex) changepriority [id] [priority]",  kCmdChangePriority },
		{ "tasklist",		"Show Task List",											kCmdShowTaskList },
		{ "killtask",		"Kill Task ex) killtask [id:0xffffffff means all task]",	kCmdKillTask },
		{ "cpuload",		"Show Processor Load", 										kCmdCpuLoad },
		{ "testmutex",		"Test Mutex Function",										kCmdTestMutex }
};

// Global Variables

static MUTEX gMutex;
static volatile QWORD gMutexCount;



// Command Function

static void kPrintNumberTask(void)
{
	int i;
	int j;
	QWORD qwTickCount;

	// Wait 50ms
	qwTickCount = kGetTickCount();
	while ( (kGetTickCount() - qwTickCount) < 50 )
	{
		kTaskSchedule();
	}

	for ( i = 0; i < 5; i ++ )
	{
		kLockMutex(&gMutex);

		// Critical Section Start
		{
			kPrintf("Task ID [0x%Q] Value [%d]\n", kTaskGetRunning()->listNode.qwID, gMutexCount);
			gMutexCount++;
		}
		// Critical Section End

		kUnlockMutex(&gMutex);

		for(j = 0; j < 30000; j++);
	}

	// Wait 1000ms
	qwTickCount = kGetTickCount();

	while((kGetTickCount() - qwTickCount) < 1000)
	{
		kTaskSchedule();
	}

	kTaskExit();
}

void kCmdTestMutex(const char* pParam)
{
	int i;

	gMutexCount = 1;

	kMutexInitialize(&gMutex);

	for( i = 0; i < 3; i ++ )
	{
		kTaskCreate(TASK_PRIOIRTY_LOW, (QWORD)kPrintNumberTask);
	}

	kPrintf("Wait until %d Task End...\n", i);

	kGetch();
}

void kCmdChangePriority(const char* pParam)
{
	PARAMETER_LIST pl;
	char vcId[30];
	char vcPriority[30];
	QWORD qwID;
	BYTE bPriority;

	kInitializeParameter(&pl, pParam);
	kGetNextParameter(&pl, vcId);
	kGetNextParameter(&pl, vcPriority);

	// ID
	if ( kMemcmp(vcId, "0x", 2) == 0 )
		qwID = kAtoi(vcId+2, 16);
	else
		qwID = kAtoi(vcId, 10);

	bPriority = kAtoi(vcPriority, 10);

	kPrintf("Change Priority - Task ID [0x%q], Priority [%d] ", qwID, bPriority);

	if(kTaskChangePriority(qwID, bPriority))
		kPrintf("Success\n");
	else
		kPrintf("Fail\n");
}

void kCmdShowTaskList(const char* pParam)
{
	int i;
	TASK* pTask;
	int nCount = 0;

	kPrintf(" Task Total Count = %d\n", kTaskGetCountAllTask() );
	for ( i = 0; i < TASK_MAX_COUNT; i ++)
	{
		pTask = kTaskGetFromPool(i);
		if ( (pTask->listNode.qwID >> 32) != 0 )
		{
			kPrintf("[%d] Task ID[0x%Q], Priority[%d], Flags[0x%Q]\n",
					1 + nCount++, pTask->listNode.qwID, TASK_GET_PRIORITY(pTask->qwFlags), pTask->qwFlags);

			if ( (nCount != 0 ) && ((nCount % 10 ) == 0) )
			{
				kPrintf("Press any key to continue...('q' is exit)  ");
				if ( kGetch() == 'q')
				{
					kPrintf("\n");
					break;
				}
				kPrintf("\n");
			}
		}
	}

}

void kCmdKillTask(const char* pParam)
{
	PARAMETER_LIST pl;
	char vcId[30];
	QWORD qwID;
	int i;
	TASK* pTask;

	kInitializeParameter(&pl, pParam);
	kGetNextParameter(&pl, vcId);

	if ( kMemcmp(vcId, "0x", 2) == 0 )
	{
		qwID = kAtoi(vcId+2, 16);
	}
	else
	{
		qwID = kAtoi(vcId, 10);
	}

	if ( qwID != 0xFFFFFFFF )
	{
		kPrintf("Kill Task ID [0x%q] ", qwID);

		if ( kTaskKill(qwID) )
			kPrintf("Success\n");
		else
			kPrintf("Fail\n");
	}
	else
	{
		for (i = 2; i < TASK_MAX_COUNT; i++)
		{
			pTask = kTaskGetFromPool(i);
			qwID = pTask->listNode.qwID;

			// if the task is running
			if ( (qwID >> 32) != 0 )
			{
				if ( kTaskKill(qwID) )
					kPrintf("Success\n");
				else
					kPrintf("Fail\n");
			}
		}
	}

	kTaskSchedule();
}

void kCmdCpuLoad(const char* pParam)
{
	kPrintf("Processor Load : %d%%\n", kTaskGetProcessorLoad());
}


void kTestTask1(void)
{
	BYTE bData;
	int i = 0, x = 0, y = 0, nMargin;
	CHARACTER* pScreen = (CHARACTER *) CONSOLE_SCREEN_ADDRESS;
	TASK* pRunningTask;

	pRunningTask = kTaskGetRunning();
	nMargin = (pRunningTask->listNode.qwID & 0xFFFFFFFF) % 10;

	while (1)
	{
		switch(i)
		{
		case 0:
			x++;
			if ( x >= ( CONSOLE_SCREEN_WIDTH - nMargin ) )
			{
				i = 1;
			}

			break;

		case 1:
			y++;
			if ( y >= (CONSOLE_SCREEN_HEIGHT - nMargin) )
			{
				i = 2;
			}
			break;

		case 2:
			x--;
			if ( x < nMargin )
			{
				i = 3;
			}
			break;

		case 3:
			y--;
			if ( y < nMargin )
			{
				i = 0;
			}
			break;

		}

		pScreen[y * CONSOLE_SCREEN_WIDTH + x].bCharactor = bData;
		pScreen[y * CONSOLE_SCREEN_WIDTH + x].bAttribute = bData & 0x0F;
		bData++;

//		kTaskSchedule();
	}

	kTaskExit();
}

void kTestTask2(void)
{
	int i = 0, nOffset;
	CHARACTER* pScreen = (CHARACTER *) CONSOLE_SCREEN_ADDRESS;
	TASK* pRunningTask;
	char vcData[4] = { '-', '\\', '|', '/' };

	pRunningTask = kTaskGetRunning();
	nOffset = (pRunningTask->listNode.qwID & 0xFFFFFFFF) * 2;
	nOffset = CONSOLE_SCREEN_WIDTH * CONSOLE_SCREEN_HEIGHT -
			(nOffset % (CONSOLE_SCREEN_HEIGHT * CONSOLE_SCREEN_WIDTH) );

	while(TRUE)
	{
		pScreen[nOffset].bCharactor = vcData[ i % 4];
		pScreen[nOffset].bAttribute = (nOffset % 15) + 1;
		i++;

//		kTaskSchedule();
	}

}

void kCmdCreateTask(const char* pParam)
{
	PARAMETER_LIST pl;
	char strType[30];
	char strCount[30];
	int i = 0, nCount, nType;


	kInitializeParameter(&pl, pParam);
	kGetNextParameter(&pl, strType);
	kGetNextParameter(&pl, strCount);

	nCount = kAtoi(strCount, 10);
	nType = kAtoi(strType, 10);

	switch( nType )
	{
	case 1:
		for(i = 0; i < nCount; i++)
		{
			if ( kTaskCreate(TASK_PRIOIRTY_LOW, (QWORD) kTestTask1) == NULL )
				break;
		}

		kPrintf("Task1 %d created\n", i);

		break;
	case 2:
		for(i = 0; i < nCount; i++)
		{
			if ( kTaskCreate(TASK_PRIOIRTY_LOW, (QWORD) kTestTask2) == NULL )
				break;
		}

		kPrintf("Task2 %d created\n", i);

		break;
	}
}



void kCmdWaitUsingPIT(const char* pParam)
{
	char buffer[100];
	int nLength;
	PARAMETER_LIST pl;
	long lMillisecond;
	int i;

	kInitializeParameter(&pl, pParam);

	if ( kGetNextParameter(&pl, buffer) == 0 )
	{
		kPrintf("ex)wait [time(ms)]\n");
		return;
	}

	lMillisecond = kAtoi(buffer, 10);
	kPrintf("%d ms Sleep\n", lMillisecond);

	kDisableInterrupt();

	// PIT only can measure about 50ms at most.
	for(i = 0; i < lMillisecond / 30; i++)
	{
		kPitWait(PIT_MILLI_TO_COUNT(30));
	}
	kPitWait(PIT_MILLI_TO_COUNT(lMillisecond % 30));
	kEnableInterrupt();

	kPitInitialize(PIT_MILLI_TO_COUNT(1), TRUE);
}

void kCmdSetTimer(const char* pParam)
{
	char buffer[100];
	PARAMETER_LIST pl;
	long lValue;
	BOOL bPeriodic;

	kInitializeParameter(&pl, pParam);

	// millisecond

	if ( kGetNextParameter(&pl, buffer) == 0 )
	{
		kPrintf("ex)settimer [time[ms] [periodic=1\n");
		return;
	}

	lValue = kAtoi(buffer, 10);

	if ( kGetNextParameter(&pl, buffer) == 0 )
	{
		kPrintf("ex)settimer [time[ms] [periodic=1\n");
		return;
	}

	bPeriodic = kAtoi(buffer, 10);

	kPitInitialize(PIT_MILLI_TO_COUNT(lValue), bPeriodic);
	kPrintf("Time		= %d ms\n", lValue);
	kPrintf("Periodic	= %d\n", bPeriodic);
}

void kCmdReadTimeStampCounter(const char* pParam)
{
	QWORD qwTSC;
	qwTSC = kReadTscAsm();
	kPrintf("Time Stamp Counter = %q\n", qwTSC);
}

void kCmdMeasureCpuSpeed(const char* pParam)
{
	int i;
	QWORD qwLastTsc, qwTotalTsc = 0;

	kDisableInterrupt();

	for(i = 0; i < 200; i++)
	{
		qwLastTsc = kReadTscAsm();
		kPitWait(PIT_MILLI_TO_COUNT(50));
		qwTotalTsc += kReadTscAsm() - qwLastTsc;

		kPrintf(".");
	}

	// Recover timer
	kPitInitialize(PIT_MILLI_TO_COUNT(1), TRUE);
	kEnableInterrupt();

	kPrintf("\nCpu speed = %d Mhz\n", qwTotalTsc / 10 / 1000 / 1000 );
}

void kCmdShowDateAndTime(const char* pParam)
{
	BYTE bSecond, bMinute, bHour;
	BYTE bDayofWeek, bDayofMonth, bMonth;
	WORD wYear;

	kRtcReadTime(&bHour, &bMinute, &bSecond);
	kRtcReadDate(&wYear, &bMonth, &bDayofMonth, &bDayofWeek);

	kPrintf("Date: %d/%d/%d %s\n", wYear, bMonth, bDayofMonth, kConvertDayofWeekToString(bDayofWeek));
	kPrintf("Time: %d:%d:%d\n", bHour, bMinute, bSecond);
}



void kCmdHelp(const char* buffer)
{
	int i;
	int nEntryCommandCount;
	int cursorX, cursorY;
	int nLength, nMaxCommandLength = 0;

	kPrintf("================================================================\n");
	kPrintf("                      CLKH64 Shell Help\n");
	kPrintf("================================================================\n");

	nEntryCommandCount = sizeof(gCommandEntry) / sizeof(SHELL_COMMAND_ENTRY);
	// get the longest command length;
	for (i = 0; i < nEntryCommandCount; i++)
	{
		nLength = kStrlen(gCommandEntry[i].pCommand);
		if ( nLength > nMaxCommandLength )
		{
			nMaxCommandLength = nLength;
		}
	}

	for(i = 0; i < nEntryCommandCount; i++)
	{
		kPrintf("%s", gCommandEntry[i].pCommand);
		kGetCursor(&cursorX, &cursorY);
		kSetCursor(nMaxCommandLength, cursorY);
		kPrintf(" - %s\n", gCommandEntry[i].pHelpForCommand );
	}
}

void kCmdCls(const char* pParam)
{
	kClearScreen();
	kSetCursor(0, 1);
}

void kCmdShowTotalRamSize(const char* pParam)
{
	kPrintf("Total RAM size = %d MB\n", kGetTotalSizeofRAM());
}

void kCmdStringToDecimalHex(const char* pParam)
{
	char buffer[100];
	int nLength;
	PARAMETER_LIST pl;
	int nCount = 0;
	long lValue;

	kInitializeParameter(&pl, pParam);

	while(1)
	{
		nLength = kGetNextParameter(&pl, buffer);

		if ( nLength == 0)
			return;

		kPrintf("Param %d = %s, Legnth = %d ", nCount+1, buffer, nLength);

		if (kMemcmp(buffer, "0x", 2) == 0)
		{
			lValue = kAtoi(buffer + 2, 16);
			kPrintf("Hex Value = %q\n", lValue);
		}
		else
		{
			lValue = kAtoi(buffer, 10);
			kPrintf("Decimal Value = %d\n", lValue);
		}

		nCount++;
	}
}

void kCmdShutdown(const char* pParam)
{
	kPrintf("System Shutdown Start...\n");
	kPrintf("Press any key to reboot");
	kGetch();
	kRebootByKeyboardController();
}









// Core Shell Function


void kConsoleShellStart(void)
{
	char commandBuffer[CONSOLE_SHELL_MAX_BUFFER_COUNT];
	int nCommandBufferIndex = 0;
	BYTE bKey;
	int cursorX, cursorY;

	kPrintf(CONSOLE_SHELL_PROMPT);

	while (1)
	{
		bKey = kGetch();

		// Backspace
		if ( bKey == KEY_BACKSPACE )
		{
			if ( nCommandBufferIndex > 0 )
			{
				kGetCursor(&cursorX, &cursorY);
				kPrintStringXY(cursorX - 1, cursorY, " ");
				kSetCursor(cursorX - 1, cursorY);
				nCommandBufferIndex--;
			}
		}
		// Enter
		else if ( bKey == KEY_ENTER )
		{
			kPrintf("\n");

			if ( nCommandBufferIndex > 0 )
			{
				commandBuffer[nCommandBufferIndex] = '\0';
				kExecuteCommand(commandBuffer);
			}

			kPrintf("%s", CONSOLE_SHELL_PROMPT);
			kMemset(commandBuffer, '\0', CONSOLE_SHELL_MAX_BUFFER_COUNT);
			nCommandBufferIndex = 0;
		}

		else if ( (bKey == KEY_LSHIFT) || (bKey == KEY_RSHIFT) || (bKey == KEY_CAPSLOCK)
				|| (bKey == KEY_NUMLOCK) || (bKey == KEY_SCROLLLOCK) )
		{

		}

		// Tab
		else
		{
			if ( bKey == KEY_TAB )
				bKey = ' ';

			if ( nCommandBufferIndex < CONSOLE_SHELL_MAX_BUFFER_COUNT )
			{
				commandBuffer[nCommandBufferIndex++] = bKey;
				kPrintf("%c", bKey);
			}
		}
	}
}

void kExecuteCommand(const char* buffer)
{
	int i, nSpaceIndex;
	int nBufferLength, nExistCommandLength;
	int nEntryCommandCount;

	nBufferLength = kStrlen(buffer);
	for(nSpaceIndex = 0; nSpaceIndex < nBufferLength; nSpaceIndex++)
	{
		if (buffer[nSpaceIndex] == ' ')
			break;
	}

	nEntryCommandCount = sizeof(gCommandEntry) / sizeof(SHELL_COMMAND_ENTRY);

	for(i = 0; i < nEntryCommandCount; i++)
	{
		nExistCommandLength = kStrlen(gCommandEntry[i].pCommand);

		if ( (nExistCommandLength == nSpaceIndex) &&
			 (kMemcmp(gCommandEntry[i].pCommand, buffer, nSpaceIndex) == 0) )
		{
			gCommandEntry[i].pFunction(buffer + nSpaceIndex + 1);
			break;
		}
	}

	// There is no command like the buffer
	if ( i >= nEntryCommandCount )
	{
		kPrintf("%s is not found.\n", buffer);
	}

	return;
}

void kInitializeParameter(PARAMETER_LIST* pParamList, const char* pParam)
{
	pParamList->pParam = pParam;
	pParamList->nLength = kStrlen(pParam);
	pParamList->nCurrentPosition = 0;
}

int kGetNextParameter(PARAMETER_LIST* pList, char* pParam)
{
	int i;
	int nLength;

	if ( pList->nLength <= pList->nCurrentPosition )
	{
		return 0;
	}

	for ( i = pList->nCurrentPosition; i < pList->nLength; i++)
	{
		if (pList->pParam[i] == ' ')
			break;
	}

	kMemcpy(pParam, pList->pParam + pList->nCurrentPosition, i);
	nLength = i - pList->nCurrentPosition;
	pParam[nLength] = '\0';

	//update Parameter
	pList->nCurrentPosition += nLength + 1;

	return nLength;
}

char* kConvertDayofWeekToString(BYTE bDayofWeek)
{
	static char* dayofWeekStrings[] = {
			"Error",
			"Sunday",
			"Monday",
			"Tuesday",
			"Wednesday",
			"Thursday",
			"Friday",
			"Saturday"
	};

	if ( bDayofWeek >= 8 )
		return dayofWeekStrings[0];

	return dayofWeekStrings[bDayofWeek];
}





































