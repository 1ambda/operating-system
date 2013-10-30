#ifndef	__UTILITY_H__
#define __UTILITY_H__

#include "Types.h"
#include <stdarg.h>



extern volatile QWORD gTickCount;
QWORD kGetTickCount(void);


void kMemset(void* pvDest, BYTE bData, int Size);
int	 kMemcpy(void* pvDest, const void* pvSrc, int Size);
int	 kMemcmp(const void* pvDest, const void* pvSrc, int Size);
void kMemSetRange(void* pvStart, void* pvEnd, BYTE bData);
BOOL kMemCheckRange(void* pvStart, void* pvEnd, BYTE bData);



void kPrintStringXY(int x, int y, const char* pString);
BOOL kSetInterruptFlag(BOOL bEnable);

int kStrlen(const char* string);
int kSprintf(char* buffer, const char* pFormatString, ...);
int kVsprintf(char* buffer, const char* pFormatString, va_list vl);

int kItoa(long lValue, char* buffer, int radix);
int kDecimaltoa(long lValue, char* buffer);
int kHextoa(QWORD qwValue, char* buffer);
QWORD kGetTotalSizeofRAM(void);
void kCheckTotalSizeofRAM(void);

long kAtoi(const char* pBuffer, int radix);
QWORD kHexstringToQword(const char* buffer);
long kDecimalstringToLong(const char* buffer);


#endif
