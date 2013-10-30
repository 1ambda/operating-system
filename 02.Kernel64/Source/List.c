#include "List.h"

void kListInitialize(LIST* pList)
{
	pList->nItemCount = 0;
	pList->pHead = NULL;
	pList->pTail = NULL;
}

int kListGetCount(const LIST* pList)
{
	return pList->nItemCount;
}

void kListAddTail(LIST* pList, void* node)
{
	LIST_NODE* pNode;

	pNode = (LIST_NODE *) node;
	pNode->pNext = NULL;

	// if a list is empty
	if ( pList->pHead == NULL )
	{
		pList->pHead = (void *)pNode;
		pList->pTail = (void *)pNode;
		pList->nItemCount = 1;

		return;
	}

	((LIST_NODE*) pList->pTail)->pNext = (void *) pNode;
	pList->pTail = (void *) pNode;
	pList->nItemCount++;
}

// return a removed node;
void* kListRemoveNode(LIST* pList, QWORD qwID)
{
	LIST_NODE* pCurrentNode, * pPrevNode;
	pPrevNode = (LIST_NODE *) pList->pHead;

	for ( pCurrentNode = pPrevNode; pCurrentNode != NULL; pCurrentNode = (LIST_NODE *) pCurrentNode->pNext )
	{
		if ( pCurrentNode->qwID == qwID )
		{
			// if the list has a node.
			if ( (pCurrentNode == pList->pHead) && ( pCurrentNode == pList->pTail ) )
			{
				pList->pHead = NULL;
				pList->pTail = NULL;
			}
			// if the node is the first node of the list.
			else if ( pCurrentNode == pList->pHead )
			{
				pList->pHead = pCurrentNode->pNext;
			}
			// if the node is the last node of the list.
			else if ( pCurrentNode == pList->pTail )
			{
				pList->pTail = pPrevNode;
			}
			// if the the node is in middle of the list.
			else
			{
				pPrevNode->pNext = pCurrentNode->pNext;
			}

			pList->nItemCount--;
			return pCurrentNode;
		}

		pPrevNode = pCurrentNode;
	}

	return NULL;
}

void* kListRemoveNodeFromHead(LIST* pList)
{
	LIST_NODE* pNode;

	if ( pList->nItemCount == 0 )
	{
		return NULL;
	}

	pNode = (LIST_NODE *) pList->pHead;

	return kListRemoveNode(pList, pNode->qwID);
}

void* kListFindNode(const LIST* pList, QWORD qwID)
{
	LIST_NODE* pCurrent;

	for(pCurrent = pList->pHead; pCurrent != NULL; pCurrent = pCurrent->pNext)
	{
		if(pCurrent->qwID == qwID)
		{
			return pCurrent;
		}
	}

	return NULL;
}

void* kListGetHeader(const LIST* pList)
{
	return pList->pHead;
}

void* kListGetTail(const LIST* pList)
{
	return pList->pTail;
}

void* kListGetNextNode(const LIST* pList, void* pNode)
{
	LIST_NODE* pCurrent = (LIST_NODE *) pNode;

	return pCurrent->pNext;
}













