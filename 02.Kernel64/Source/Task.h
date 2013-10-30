#ifndef __TASK_H__
#define __TASK_H__

#include "Types.h"
#include "List.h"

#define TASK_ADDRESS_POOL_TCB		0x800000 // 8M ~
#define TASK_MAX_COUNT				1024
#define TASK_ADDRESS_POOL_STACK		(TASK_ADDRESS_POOL_TCB + sizeof(TCB) * TASK_MAX_COUNT)
#define TASK_STACK_SIZE				8192 // 8k

#define TASK_MAX_READYQUEUE_COUNT	5

#define TASK_PRIOIRTY_HIGHEST		0
#define TASK_PRIOIRTY_HIGH			1
#define TASK_PRIOIRTY_MEDIUM		2
#define TASK_PRIOIRTY_LOW			3
#define TASK_PRIOIRTY_LOWEST		4
#define TASK_PRIOIRTY_WAIT			0xFF

#define TASK_FLAG_ENDTASK			0x8000000000000000
#define TASK_FLAG_IDLE				0x0800000000000000

#define TASK_GET_PRIORITY(x)				( (x) & 0xFF )
#define TASK_SET_PRIORITY(x, priority)		( (x) = ( (x) & 0xFFFFFFFFFFFFFF00 ) | (priority) )
#define TASK_GET_OFFSET_TCB(x)				( (x) & 0xFFFFFFFF )


#define TASK_INVALID_ID				0xFFFFFFFFFFFFFFFF
#define TASK_TIMESLICE				5 // 5ms


#define TASK_REGISTER_COUNT	( 5 + 19 ) // SS, RSP, RFLAGS, CS, RIP + 19
#define	TASK_REGISTER_SIZE	8

#define TASK_OFFSET_GS		0
#define TASK_OFFSET_FS		1
#define TASK_OFFSET_ES		2
#define TASK_OFFSET_DS		3
#define TASK_OFFSET_R15		4
#define TASK_OFFSET_R14		5
#define TASK_OFFSET_R13		6
#define TASK_OFFSET_R12		7
#define TASK_OFFSET_R11		8
#define TASK_OFFSET_R10		9
#define TASK_OFFSET_R19		10
#define TASK_OFFSET_R18		11
#define TASK_OFFSET_RSI		12
#define TASK_OFFSET_RDI		13
#define TASK_OFFSET_RDX		14
#define TASK_OFFSET_RCX		15
#define TASK_OFFSET_RBX		16
#define TASK_OFFSET_RAX		17
#define TASK_OFFSET_RBP		18
#define TASK_OFFSET_RIP		19
#define TASK_OFFSET_CS		20
#define TASK_OFFSET_RFLAGS	21
#define TASK_OFFSET_RSP		22
#define TASK_OFFSET_SS		23

#pragma pack(push, 1)

typedef struct kContextStruct
{
	QWORD vqRegisters[TASK_REGISTER_COUNT];
} CONTEXT;

typedef struct kTaskControlBlockStruct
{
	LIST_NODE listNode;

	CONTEXT context;
	QWORD qwFlags;

	void* pStackAddress;
	QWORD qwStackSize;
} TASK, TCB;

typedef struct kTcbPoolManagerStruct
{
	TASK* pTaskPoolAddress;
	int nMaxCount;
	int nUseCount;

	int nAllocatedCount; // this variable is mask used for making unique id.
						 // it is differ from useCount.

} TASK_POOL_MANAGER;

typedef struct kTaskScheduleStruct
{
	TASK* pRunningTask;

	int nRemainedTimeslice;

	LIST queueReady[TASK_MAX_READYQUEUE_COUNT];
	LIST queueWait;

	int vnExecuteCount[TASK_MAX_READYQUEUE_COUNT];

	QWORD qwProcessorLoad;
	QWORD qwTimeUsedByIdletask;


} TASK_SCHEDULER;





#pragma pack(pop)

// Functions

void 	kTaskInitialize(void);

void	kTaskSetup(TASK* pTCB, QWORD flags, QWORD entrypoint, void* pStackAddress, QWORD stacksize);
TASK*	kTaskAllocate(void);
void	kTaskFree(QWORD id);
TASK*	kTaskCreate(QWORD qwFlags, QWORD entrypoint);

BOOL	kTaskAddtoReadyQueue(TASK* pTask);
TASK*	kTaskRemoveFromReadyQueue(QWORD qwTaskID);
TASK*	kTaskGetNextAndRemoveFromSchedule(void);
TASK*	kTaskGetRunning(void);
void	kTaskSetRunning(TASK* pTask);

void	kTaskSchedule(void);
BOOL	kTaskScheduleInterrupt(void);
void	kTaskDecreaseTimeslice(void);
BOOL	kTaskIsTimesliceExpired(void);

BOOL	kTaskChangePriority(QWORD qwTaskID, BYTE bPriority);
TASK*	kTaskGetFromPool(int nOffset);
int		kTaskGetCountAllReadyTask(void);

void	kTaskExit(void);
BOOL	kTaskKill(QWORD qwTaskID);
BOOL	kTaskisExist(QWORD qwID);
QWORD	kTaskGetProcessorLoad(void);
void	kTaskIdle(void);
void	kTaskHalt(void);

#endif
