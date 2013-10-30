#ifndef __LIST_H__
#define __LIST_H__

#include "Types.h"

// Structures

#pragma pack(push, 1)

typedef struct kListNodeStruct
{
	void* pNext;
	QWORD qwID;
} LIST_NODE;

typedef struct kListStruct
{
	int nItemCount;

	void* pHead;
	void* pTail;
} LIST;

#pragma pack(pop)

// Functions.

void kListInitialize(LIST* pList);
int kListGetCount(const LIST* pList);

void kListAddTail(LIST* pList, void* node);
void* kListRemoveNode(LIST* pList, QWORD qwID);
void* kListRemoveNodeFromHead(LIST* pList);
void* kListFindNode(const LIST* pList, QWORD qwID);
void* kListGetHeader(const LIST* pList);
void* kListGetTail(const LIST* pList);
void* kListGetNextNode(const LIST* pList, void* pNode);

#endif
