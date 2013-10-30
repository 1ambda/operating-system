#include "Synchronization.h"
#include "Utility.h"
#include "InlineAssemblyUtility.h"
#include "Task.h"


BOOL kLockInterrupt(void)
{
	return kSetInterruptFlag(FALSE);
}

void kUnlockInterrupt(BOOL bFlag)
{
	kSetInterruptFlag(bFlag);
}

void kMutexInitialize(MUTEX* pMutex)
{
	pMutex->dwLockCount = 0;
	pMutex->bLockFlag = FALSE;
	pMutex->qwTaskId = TASK_INVALID_ID;
}

void kLockMutex(MUTEX* pMutex)
{
	// if the mutex is already locked
	if ( kAtomicCmpSet( &(pMutex->bLockFlag), FALSE, TRUE) == FALSE )
	{
		// if current task locked the mutex before
		if ( (pMutex->qwTaskId == (kTaskGetRunning()->listNode.qwID)) )
		{
			pMutex->dwLockCount++;
			return;
		}

		while ( kAtomicCmpSet( &(pMutex->bLockFlag), FALSE, TRUE) == FALSE )
		{
			kTaskSchedule();
		}
	}

	// current task got the mutex
	pMutex->dwLockCount = 1;
	pMutex->qwTaskId = kTaskGetRunning()->listNode.qwID;
}

void kUnlockMutex(MUTEX* pMutex)
{
	if ( (pMutex->bLockFlag == FALSE) || (pMutex->qwTaskId != (kTaskGetRunning()->listNode.qwID)) )
	{
		return;
	}

	if ( pMutex->dwLockCount > 1)
	{
		pMutex->dwLockCount--;
		return;
	}

	pMutex->qwTaskId = TASK_INVALID_ID;
	pMutex->dwLockCount = 0;
	pMutex->bLockFlag = FALSE;
}

