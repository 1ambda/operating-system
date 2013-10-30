#ifndef __MODE_SWITCH__
#define __MODE_SWITCH__

#include "Types.h"

void kCpuid(DWORD* eax, DWORD* ebx, DWORD* ecx, DWORD* edx);
void kSwitchModeTo64(void);

extern void kPrintString(int nX, int nY, const char* pString);
extern int gScreenY;

#endif
