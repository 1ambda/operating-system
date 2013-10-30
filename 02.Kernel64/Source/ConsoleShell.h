#ifndef __CONSOLE_SHELL_H__
#define __CONSOLE_SHELL_H__

#include "Types.h"

#define	CONSOLE_SHELL_MAX_BUFFER_COUNT		300
#define	CONSOLE_SHELL_PROMPT				"CLKH64 > "

typedef void (* CommandFunction) (const char* string);

#pragma pack(push, 1)

typedef struct kShellCommandEntryStruct
{
	char* pCommand;
	char* pHelpForCommand;
	CommandFunction	pFunction;
}SHELL_COMMAND_ENTRY;

typedef struct kParameterListStruct
{
	const char* pParam;
	int nLength;
	int nCurrentPosition;

}PARAMETER_LIST;

#pragma pack(pop)

// for shell
void kConsoleShellStart(void);
void kExecuteCommand(const char* buffer);
void kInitializeParameter(PARAMETER_LIST* pParamList, const char* pParam);
int kGetNextParameter(PARAMETER_LIST* pList, char* pParam);

// for command

void kCmdHelp(const char* buffer);
void kCmdCls(const char* pParam);
void kCmdShowTotalRamSize(const char* pParam);
void kCmdStringToDecimalHex(const char* pParam);
void kCmdShutdown(const char* pParam);

void kCmdSetTimer(const char* pParam);
void kCmdReadTimeStampCounter(const char* pParam);
void kCmdMeasureCpuSpeed(const char* pParam);
void kCmdShowDateAndTime(const char* pParam);
void kCmdWaitUsingPIT(const char* pParam);

void kCmdCreateTask(const char* pParam);
void kTestTask1(void);
void kTestTask2(void);

void kCmdChangePriority(const char* pParam);
void kCmdShowTaskList(const char* pParam);
void kCmdCpuLoad(const char* pParam);
void kCmdKillTask(const char* pParam);

void kCmdTestMutex(const char* pParam);
static void kPrintNumberTask(void);


// for Timer;
char* kConvertDayofWeekToString(BYTE bDayofWeek);

#endif
