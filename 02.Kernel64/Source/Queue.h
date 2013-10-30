#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "Types.h"

#pragma pack(push, 1)

typedef struct kQueueManagerStruct
{
	int nDataSize;
	int nMaxDataCount;

	void* pvQueueArray;
	int nPutIndex;
	int nGetIndex;

	BOOL bIsLastOperationPut;



} QUEUE;

#pragma pack(pop)

BOOL kQueueIsFull(const QUEUE* pQ);
BOOL kQueueIsEmpry(const QUEUE* pQ);
BOOL kQueuePut(QUEUE* pQ, const void* pData);
BOOL kQueueGet(QUEUE* pQ, void* pData);


#endif

