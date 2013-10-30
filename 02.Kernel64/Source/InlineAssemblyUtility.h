#ifndef __ASSEMBLY_UTILITY_H__
#define __ASSEMBLY_UTILITY_H__

#include "Types.h"
#include "Task.h"

// InlineAssemblyUtility.c

BYTE kReadPortByte(WORD wPort);
void kWritePortByte(WORD wPort, BYTE bData);



// AssemblyUtility.asm

BYTE kReadPortByteAsm(WORD wPort);
void kWritePortByteAsm(WORD wPort, BYTE bData);

void kLoadGDTRAsm(QWORD qwAddress);
void kLoadIDTRAsm(QWORD qwAddress);
void kLoadTRAsm(WORD wOffsetTSS);

void kEnableInterrupt(void);
void kDisableInterrupt(void);
QWORD kReadRFlags(void);

QWORD kReadTscAsm(void);
void kSwitchContext(CONTEXT* pCurrent, CONTEXT* pNext);


BOOL kAtomicCmpSet(volatile BYTE* pDest, BYTE bCmp, BYTE Src);


#endif

