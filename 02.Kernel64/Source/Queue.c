#include "Queue.h"

void kQueueInitialize(QUEUE* pQ, void* pQBuf, int nMaxDataCount, int nDataSize)
{
	pQ->nMaxDataCount = nMaxDataCount;
	pQ->nDataSize = nDataSize;

	pQ->pvQueueArray = pQBuf;
	pQ->nGetIndex = 0;
	pQ->nPutIndex = 0;
	pQ->bIsLastOperationPut = FALSE;
}

BOOL kQueueIsFull(const QUEUE* pQ)
{
	if ( (pQ->nGetIndex == pQ->nPutIndex) &&
			(pQ->bIsLastOperationPut == TRUE) )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL kQueueIsEmpry(const QUEUE* pQ)
{
	if ( (pQ->nGetIndex == pQ->nPutIndex) &&
			(pQ->bIsLastOperationPut == FALSE) )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL kQueuePut(QUEUE* pQ, const void* pData)
{
	if ( kQueueIsFull(pQ) == TRUE )
		return FALSE;

	kMemcpy((char*) pQ->pvQueueArray + (pQ->nDataSize * pQ->nPutIndex), pData, pQ->nDataSize);
	pQ->nPutIndex = (pQ->nPutIndex + 1) % pQ->nMaxDataCount;
	pQ->bIsLastOperationPut = TRUE;

	return TRUE;
}

BOOL kQueueGet(QUEUE* pQ, void* pData)
{

	if ( kQueueIsEmpry(pQ) == TRUE )
	{
		return FALSE;
	}

	kMemcpy(pData, (char*) pQ->pvQueueArray + (pQ->nDataSize * pQ->nGetIndex), pQ->nDataSize);
	pQ->nGetIndex = (pQ->nGetIndex + 1) % pQ->nMaxDataCount;
	pQ->bIsLastOperationPut = FALSE;

	return TRUE;
}




















