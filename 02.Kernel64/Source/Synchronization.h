#ifndef __SYNCRONIZATION_H__
#define __SYNCRONIZATION_H__

#include "Types.h"

#pragma pack(push, 1)

typedef struct kMutextStruct
{
	volatile QWORD	qwTaskId;
	volatile DWORD	dwLockCount;
	volatile BOOL	bLockFlag;

	// for alignment
	BYTE padding[3];
} MUTEX;

#pragma pack(pop)

// Functions

BOOL kLockInterrupt(void);
void kUnlockInterrupt(BOOL bFlag);

void kLockMutex(MUTEX* pMutex);
void kUnlockMutex(MUTEX* pMutex);



#endif
