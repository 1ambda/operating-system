#include "InlineAssemblyUtility.h"

BYTE kReadPortByte(WORD wPort)
{
	BYTE bData;

	__asm__ __volatile__ (
							"mov	$0, %%rax\n"
							"in	%%dx, %%al\n"
							:"=a"(bData)
							:"d"(wPort)
	);

	return bData;

}

void kWritePortByte(WORD wPort, BYTE bData)
{
	__asm__ __volatile__ (
							"out	%%al,	%%dx\n"
							:
							:"d"(wPort), "a"(bData)
	);
}
