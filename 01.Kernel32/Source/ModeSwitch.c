#include "ModeSwitch.h"


void kCpuid(DWORD* eax, DWORD* ebx, DWORD* ecx, DWORD* edx)
{
	__asm__ __volatile__ ("cpuid"
							:"=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
							:"a"(*eax) );
}


void setCR4(void)
{
	int eax;
	eax = 0x20;

	__asm__ __volatile__( "or $0x20, %%eax\n"
						"mov %%eax, %%cr4"
						:
						:"a"(eax));
}

void setCR3(DWORD eax)
{
	__asm__ __volatile__("mov %%eax, %%cr3"
						:
						:"a"(eax));
}

void setIA32_EFER(void)
{
	DWORD ecx;
	ecx = 0xC0000080;

	__asm__ __volatile__( "rdmsr\n"
						"or $0x0100, %%eax\n"
						"wrmsr"
						:
						:"c"(ecx));
}

void setCR0(void)
{
	BYTE cs;
	cs = 0x08;
	char* string = "GCC Inline Assembly Operation................[PASS]";

	__asm__ __volatile__( "mov %%cr0, %%eax\n"
						"or $0xE0000000, %%eax\n"
						"xor $0x60000000, %%eax\n"
						"mov %%eax, %%cr0\n"
						"mov $0x20000, %%eax\n"
						//
						"push %%esi\n"
						"push %%edx\n"
						"push %%ecx\n"
						"call *%%edi\n"
						//
						"ljmp $0x08, $0x200000\n"
						:
						:"c"(0x00), "d"(gScreenY++), "S"(string), "D"(kPrintString) );
}



void kSwitchModeTo64(void)
{
	setCR4();
	setCR3(0x100000);
	setIA32_EFER();
	setCR0();
}
