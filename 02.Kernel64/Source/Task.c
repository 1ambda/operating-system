#include "Task.h"
#include "Descriptor.h"

// Global Variables.

static TASK_SCHEDULER 		gTaskScheduler;
static TASK_POOL_MANAGER 	gTaskPoolManager;

// Functions

void kTaskInitialize(void)
{
	int i;

	// Task
	kMemset( &(gTaskPoolManager), 0, sizeof(TASK_POOL_MANAGER) );
	kMemset( &(gTaskScheduler), 0, sizeof(TASK_SCHEDULER) );

	gTaskPoolManager.pTaskPoolAddress = (TASK *) TASK_ADDRESS_POOL_TCB;

	// allot id.
	for(i = 0; i < TASK_MAX_COUNT; i++)
	{
		gTaskPoolManager.pTaskPoolAddress[i].listNode.qwID = i;
	}

	gTaskPoolManager.nMaxCount = TASK_MAX_COUNT;
	gTaskPoolManager.nAllocatedCount = 1;

	// Scheduler

	for(i = 0; i < TASK_MAX_READYQUEUE_COUNT; i++)
	{
		kListInitialize(&(gTaskScheduler.queueReady[i]));
		gTaskScheduler.vnExecuteCount[i] = 0;
	}

		// Starting task doesn't need to be given stack.
		// So we use the kTaskAllocate function.
	gTaskScheduler.pRunningTask = kTaskAllocate();
	gTaskScheduler.pRunningTask->qwFlags = TASK_PRIOIRTY_HIGHEST;

	gTaskScheduler.qwProcessorLoad = 0;
	gTaskScheduler.qwTimeUsedByIdletask = 0;

}

void kTaskSetRunning(TASK* pTask)
{
	BOOL bPrevFlag;

	// CriticalSection Start
	{
		bPrevFlag = kLockInterrupt();
		gTaskScheduler.pRunningTask = pTask;
		kUnlockInterrupt(bPrevFlag);
	}
}

TASK* kTaskGetRunning(void)
{
	TASK* pTask;
	BOOL bPrevFlag;

	// Critical Section
	{
		bPrevFlag = kLockInterrupt();
		pTask = gTaskScheduler.pRunningTask;
		kUnlockInterrupt(bPrevFlag);
	}

	return pTask;
}

TASK* kTaskGetNextAndRemoveFromSchedule(void)
{
	TASK* pNextTask = NULL;
	int nTaskCount, i, j;

	for ( j = 0; j < 2; j++)
	{
		for ( i = 0; i < TASK_MAX_READYQUEUE_COUNT; i++)
		{
			nTaskCount = kListGetCount(&(gTaskScheduler.queueReady[i]));

			if ( gTaskScheduler.vnExecuteCount[i] < nTaskCount )
			{
				pNextTask = (TASK *) kListRemoveNodeFromHead(&(gTaskScheduler.queueReady[i]));
				gTaskScheduler.vnExecuteCount[i]++;
				break;
			}

			else
			{
				gTaskScheduler.vnExecuteCount[i] = 0;
			}
		}

		if ( pNextTask != NULL )
			break;
	}

	return pNextTask;
}



void kTaskSchedule(void)
{
	TASK* pRunningTask, * pNextTask;
	BOOL bPrevFlag;

	if ( kTaskGetCountAllReadyTask() < 1)
		return;

	// Critical Section Start
	{
		bPrevFlag = kLockInterrupt();

		pNextTask = kTaskGetNextAndRemoveFromSchedule();

		if ( pNextTask == NULL )
		{
			kUnlockInterrupt(bPrevFlag);
			return;
		}

		// Schedule change.
		pRunningTask = kTaskGetRunning();
		gTaskScheduler.pRunningTask = pNextTask;

		// if current task is the idle task
		if ( (pRunningTask->qwFlags & TASK_FLAG_IDLE) == TASK_FLAG_IDLE )
		{
			gTaskScheduler.qwTimeUsedByIdletask +=
					TASK_TIMESLICE - gTaskScheduler.nRemainedTimeslice;
		}

		if ( pRunningTask->qwFlags & TASK_FLAG_ENDTASK )
		{
			kListAddTail(&(gTaskScheduler.queueWait), pRunningTask);
			kSwitchContext(NULL, &(pNextTask->context));
		}
		else
		{
			kTaskAddtoReadyQueue(pRunningTask);
			kSwitchContext(&(pRunningTask->context), &(pNextTask->context));
		}

		gTaskScheduler.nRemainedTimeslice = TASK_TIMESLICE;

		kUnlockInterrupt(bPrevFlag);
	}
	// Critical Section End

}

BOOL kTaskScheduleInterrupt(void)
{
	TASK* pRunningTask, * pNextTask;
	char* pContextAddress;
	BOOL bPrevFlag;

	// Critical Section Start
	{
		bPrevFlag = kLockInterrupt();

		pNextTask = kTaskGetNextAndRemoveFromSchedule();

		if ( pNextTask == NULL )
		{
			kUnlockInterrupt(bPrevFlag);
			return FALSE;
		}

		pRunningTask = kTaskGetRunning();
		gTaskScheduler.pRunningTask = pNextTask;

		pContextAddress = (char *) START_ADDRESS_IST + SIZE_IST - sizeof(CONTEXT);

		if ( ( pRunningTask->qwFlags & TASK_FLAG_IDLE ) == TASK_FLAG_IDLE )
		{
			gTaskScheduler.qwTimeUsedByIdletask += TASK_TIMESLICE;
		}

		if ( pRunningTask->qwFlags & TASK_FLAG_ENDTASK )
		{
			kListAddTail(&(gTaskScheduler.queueWait), pRunningTask);
		}
		else
		{
			// Copy current task's context from IST
			kMemcpy(&(pRunningTask->context), pContextAddress, sizeof(CONTEXT));
			kTaskAddtoReadyQueue(pRunningTask);
		}

		kUnlockInterrupt(bPrevFlag);
	}
	// Critical Section End

	// Copy next task's context to IST
	kMemcpy(pContextAddress, &(pNextTask->context), sizeof(CONTEXT));

	gTaskScheduler.nRemainedTimeslice = TASK_TIMESLICE;

	return TRUE;
}

void kTaskDecreaseTimeslice(void)
{
	if ( gTaskScheduler.nRemainedTimeslice > 0 )
		gTaskScheduler.nRemainedTimeslice--;

}

BOOL kTaskIsTimesliceExpired(void)
{
	if ( gTaskScheduler.nRemainedTimeslice <= 0 )
		return TRUE;

	return FALSE;
}



TASK* kTaskAllocate(void)
{
	TASK* pEmptyTask;
	int i;

	if ( gTaskPoolManager.nUseCount == gTaskPoolManager.nMaxCount )
		return NULL;

	for (i = 0; i < gTaskPoolManager.nMaxCount; i++)
	{
		if ( (gTaskPoolManager.pTaskPoolAddress[i].listNode.qwID >> 32 ) == 0 )
		{
			pEmptyTask = &(gTaskPoolManager.pTaskPoolAddress[i]);
			break;
		}
	}

	pEmptyTask->listNode.qwID = (( (QWORD) gTaskPoolManager.nAllocatedCount << 32 ) | i);
	gTaskPoolManager.nUseCount++;
	gTaskPoolManager.nAllocatedCount++;

	if ( gTaskPoolManager.nAllocatedCount == 0 )
		gTaskPoolManager.nAllocatedCount = 1;

	return pEmptyTask;
}

void kTaskFree(QWORD id)
{
	int i;

	i = TASK_GET_OFFSET_TCB(id); // remove allocate-mask

	kMemset(&(gTaskPoolManager.pTaskPoolAddress[i].context), 0, sizeof(CONTEXT));
	gTaskPoolManager.pTaskPoolAddress[i].listNode.qwID = i;

	gTaskPoolManager.nUseCount--;
}

TASK* kTaskCreate(QWORD qwFlags, QWORD entrypoint)
{
	TASK* pTask;
	void* pStackAddress;
	int nTaskIndex;
	BOOL bPrevFlag;

	// Critical Section
	{
		bPrevFlag = kLockInterrupt();

		pTask = kTaskAllocate();
		nTaskIndex = TASK_GET_OFFSET_TCB(pTask->listNode.qwID);

		if ( pTask == NULL )
		{
			kUnlockInterrupt(bPrevFlag);
			return NULL;
		}

		kUnlockInterrupt(bPrevFlag);
	}

	pStackAddress = (void *) (TASK_ADDRESS_POOL_STACK + (TASK_STACK_SIZE * nTaskIndex));
	kTaskSetup(pTask, qwFlags, entrypoint, pStackAddress, TASK_STACK_SIZE);

	// Critical Section
	{
		bPrevFlag = kLockInterrupt();
		kTaskAddtoReadyQueue(pTask);
		kUnlockInterrupt(bPrevFlag);
	}

	return pTask;
}

BOOL kTaskAddtoReadyQueue(TASK* pTask)
{
	BYTE bPriority;

	bPriority = TASK_GET_PRIORITY(pTask->qwFlags);

	if ( bPriority >= TASK_MAX_READYQUEUE_COUNT )
		return FALSE;

	kListAddTail(&(gTaskScheduler.queueReady[bPriority]), pTask);

	return TRUE;
}

TASK* kTaskRemoveFromReadyQueue(QWORD qwTaskID)
{
	TASK* pRemovedTask;
	BYTE bPriority;

	if ( TASK_GET_OFFSET_TCB(qwTaskID) >= TASK_MAX_COUNT )
		return NULL;

	pRemovedTask = &(gTaskPoolManager.pTaskPoolAddress[TASK_GET_OFFSET_TCB(qwTaskID)]);

	if ( pRemovedTask->listNode.qwID != qwTaskID )
		return NULL;

	bPriority = TASK_GET_PRIORITY(pRemovedTask->qwFlags);

	pRemovedTask = kListRemoveNode(&(gTaskScheduler.queueReady[bPriority]), qwTaskID);

	return pRemovedTask;
}

void kTaskSetup(TASK* pTask, QWORD flags, QWORD entrypoint, void* pStackAddress, QWORD stacksize)
{

//	kMemset(pTask->context.vqRegisters, 0, sizeof(pTask->context.vqRegisters));
	kMemset(&(pTask->context), 0, sizeof(CONTEXT));


	// Stack
	pTask->context.vqRegisters[TASK_OFFSET_RSP] = (QWORD) pStackAddress + stacksize;
	pTask->context.vqRegisters[TASK_OFFSET_RBP] = (QWORD) pStackAddress + stacksize;

	// Segments
	pTask->context.vqRegisters[TASK_OFFSET_CS] = GDT_OFFSET_KERNEL64CODE;
	pTask->context.vqRegisters[TASK_OFFSET_DS] = GDT_OFFSET_KERNEL64DATA;
	pTask->context.vqRegisters[TASK_OFFSET_ES] = GDT_OFFSET_KERNEL64DATA;
	pTask->context.vqRegisters[TASK_OFFSET_FS] = GDT_OFFSET_KERNEL64DATA;
	pTask->context.vqRegisters[TASK_OFFSET_GS] = GDT_OFFSET_KERNEL64DATA;
	pTask->context.vqRegisters[TASK_OFFSET_SS] = GDT_OFFSET_KERNEL64DATA;

	// RIP
	pTask->context.vqRegisters[TASK_OFFSET_RIP] = entrypoint;

	// RFLAGs.IF = 1
	pTask->context.vqRegisters[TASK_OFFSET_RFLAGS] |= 0x0200;


	pTask->pStackAddress = pStackAddress;
	pTask->qwStackSize = stacksize;
	pTask->qwFlags = flags;

	return;
}

BOOL kTaskChangePriority(QWORD qwTaskID, BYTE bPriority)
{
	TASK*	pTargetTask;
	BOOL	bPrevFlag;

	if ( bPriority > TASK_MAX_READYQUEUE_COUNT )
		return FALSE;

	// Critical Section
	{
		bPrevFlag = kLockInterrupt();

		// If it is running
		pTargetTask = gTaskScheduler.pRunningTask;

		if ( qwTaskID == pTargetTask->listNode.qwID )
		{
			TASK_SET_PRIORITY(pTargetTask->qwFlags, bPriority);
		}

		else
		{
			pTargetTask = kTaskRemoveFromReadyQueue(qwTaskID);
			// if we can't find it from ready queue
			if ( pTargetTask == NULL )
			{
				pTargetTask = kTaskGetFromPool(qwTaskID);

				if ( pTargetTask != NULL )
					TASK_SET_PRIORITY(pTargetTask->qwFlags, bPriority);
			}
			else
			{
				TASK_SET_PRIORITY(pTargetTask->qwFlags, bPriority);
				kTaskAddtoReadyQueue(pTargetTask);
			}
		}

		kUnlockInterrupt(bPrevFlag);
	}

	return TRUE;
}

TASK* kTaskGetFromPool(int nOffset)
{
	if ( ( nOffset < -1 ) && ( nOffset > TASK_MAX_COUNT ) )
		return NULL;

	return &(gTaskPoolManager.pTaskPoolAddress[nOffset]);
}

int kTaskGetCountAllReadyTask(void)
{
	int nTotal = 0;
	int i;
	BOOL bPrevFlag;

	bPrevFlag = kLockInterrupt();

	// Critical Section
	{
		for(i = 0; i < TASK_MAX_READYQUEUE_COUNT; i++)
		{
			nTotal += kListGetCount(&(gTaskScheduler.queueReady[i]));
		}
	}
	// Critical Section

	kUnlockInterrupt(bPrevFlag);


	return nTotal;
}

int kTaskGetCountAllTask(void)
{
	int nTotal = 0;
	BOOL bPrevFlag;

	nTotal = kTaskGetCountAllReadyTask();


	bPrevFlag = kLockInterrupt();
	// Critical Section Start
	{
		nTotal += kListGetCount(&(gTaskScheduler.queueWait));
	}
	// Critical Section End
	kUnlockInterrupt(bPrevFlag);

	nTotal++; // current task;

	return nTotal;
}

BOOL kTaskKill(QWORD qwTaskID)
{
	TASK* pTargetTask;
	BYTE bPriority;
	BOOL bPrevFlag;

	bPrevFlag = kLockInterrupt();

	// Critical Section Start
	{

		pTargetTask = kTaskGetRunning();

		if ( pTargetTask->listNode.qwID == qwTaskID )
		{
			pTargetTask->qwFlags |= TASK_FLAG_ENDTASK;
			TASK_SET_PRIORITY(pTargetTask->qwFlags, TASK_PRIOIRTY_WAIT);

			kUnlockInterrupt(bPrevFlag);

			kTaskSchedule();


			// logic can't reach here.
			while(TRUE);
		}
		else
		{
			pTargetTask = kTaskRemoveFromReadyQueue(qwTaskID);
			if( pTargetTask == NULL )
			{
				pTargetTask = kTaskGetFromPool(TASK_GET_OFFSET_TCB(qwTaskID));

				if ( pTargetTask != NULL )
				{
					pTargetTask->qwFlags |= TASK_FLAG_ENDTASK;
					TASK_SET_PRIORITY(pTargetTask->qwFlags, TASK_PRIOIRTY_WAIT);
				}

				kUnlockInterrupt(bPrevFlag);
				return FALSE;
			}

			pTargetTask->qwFlags |= TASK_FLAG_ENDTASK;
			TASK_SET_PRIORITY(pTargetTask->qwFlags, TASK_PRIOIRTY_WAIT);
			kListAddTail(&(gTaskScheduler.queueWait), pTargetTask);
		}


	}
	// Critical Section End

	kUnlockInterrupt(bPrevFlag);
	return TRUE;
}

void kTaskExit(void)
{
	kTaskKill(gTaskScheduler.pRunningTask->listNode.qwID);
}

BOOL kTaskisExist(QWORD qwID)
{
	TASK* pTask;

	pTask = kTaskGetFromPool(TASK_GET_OFFSET_TCB(qwID));

	if ( (pTask == NULL) || (pTask->listNode.qwID != qwID ) )
		return FALSE;

	return TRUE;
}

QWORD kTaskGetProcessorLoad(void)
{
	return gTaskScheduler.qwProcessorLoad;
}

// Idle Task

void kTaskIdle(void)
{
	TASK* pTask;
	QWORD qwLastMeasureTickCount, qwLastSpendTickInIdleTask;
	QWORD qwCurrentMeasureTickCount, qwCurrentSpendTickInIdleTask;
	BOOL bPrevFlag;
	QWORD qwTaskId;

	qwLastSpendTickInIdleTask = gTaskScheduler.qwTimeUsedByIdletask;
	qwLastMeasureTickCount = kGetTickCount();

	while(1)
	{
		qwCurrentMeasureTickCount = kGetTickCount();
		qwCurrentSpendTickInIdleTask = gTaskScheduler.qwTimeUsedByIdletask;

		if (( qwCurrentMeasureTickCount - qwLastMeasureTickCount) == 0)
			gTaskScheduler.qwProcessorLoad = 0;
		else
		{
			gTaskScheduler.qwProcessorLoad = 100 -
					(qwCurrentSpendTickInIdleTask - qwLastSpendTickInIdleTask) * 100 /
					(qwCurrentMeasureTickCount - qwLastMeasureTickCount);
		}

		qwLastMeasureTickCount = qwCurrentMeasureTickCount;
		qwLastSpendTickInIdleTask = qwCurrentSpendTickInIdleTask;

		kTaskHalt();

		if ( kListGetCount(&(gTaskScheduler.queueWait)) >= 0 )
		{
			while(1)
			{
				bPrevFlag = kLockInterrupt();
				// Critical Section Start
				{
					pTask = kListRemoveNodeFromHead(&(gTaskScheduler.queueWait));

					kUnlockInterrupt(bPrevFlag);

					if( pTask == NULL )
					{
						kUnlockInterrupt(bPrevFlag);
						break;
					}

					kTaskFree(pTask->listNode.qwID);
				}
				// Critical Section End
				kUnlockInterrupt(bPrevFlag);

				kPrintf("IDLE: task ID[0x%q] is completely killed\n", pTask->listNode.qwID);
			}
		}

		kTaskSchedule();
	}
}

void kTaskHalt(void)
{
	if ( gTaskScheduler.qwProcessorLoad < 40 )
	{
		kHaltAsm();
		kHaltAsm();
		kHaltAsm();
	}
	else if ( gTaskScheduler.qwProcessorLoad < 80 )
	{
		kHaltAsm();
		kHaltAsm();
	}
	else if ( gTaskScheduler.qwProcessorLoad < 95 )
	{
		kHaltAsm();
	}

}















