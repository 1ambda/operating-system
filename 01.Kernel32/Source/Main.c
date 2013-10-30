#include "Types.h"
#include "Page.h"
#include "ModeSwitch.h"



int gScreenY = 6;

//	mov eax, 0x4000003B


void kCpuid(DWORD* eax, DWORD* ebx, DWORD* ecx, DWORD* edx);
void kPrintString(int nX, int nY, const char* pString);
BOOL kInitializedKernel64Area(void);
BOOL kCheckMemoryCapacity(void);
void kCopyKernel64To2MbArea(void);


void Main(void)
{
	DWORD eax, ebx, ecx, edx;
	char venderName[13] = { 0 };

	kPrintString(0, gScreenY++, "Loading Kernel32 written by C................[PASS]");

	if ( !kCheckMemoryCapacity() )
	{
		kPrintString(0, gScreenY++, "64MB Memory Available........................[FAIL]");
		while(TRUE);
	}

	kPrintString(0, gScreenY++, "64MB Memory Available........................[PASS]");


	if ( !kInitializedKernel64Area() )
	{
		kPrintString(0, gScreenY++, "Kernel64 Area(6MB) Initialization............[FAIL]");
		while(TRUE);
	}

	kPrintString(0, gScreenY++, "Kernel64 Area(6MB) Initialization............[PASS]");


	// Set Paging Structures for IA32-e Paging
	kInitializePageTables();
	kPrintString(0, gScreenY++, "IA32-e Page Structures Initialization........[PASS]");

	// Check whether our cpu can support 64bit mode
	eax = 0x80000001;
	kCpuid(&eax, &ebx, &ecx, &edx);

	if ( edx & ( 1 << 29 ))
	{
		kPrintString(0, gScreenY++, "Check whether Cpu can provide 64-Bit Mode....[PASS]");
	}

	// Get Cpu vender name using cpuid instruction
	eax = 0x00000000;
	kCpuid(&eax, &ebx, &ecx, &edx);

	*((DWORD*) venderName + 0) =  ebx;
	*((DWORD*) venderName + 1) =  edx;
	*((DWORD*) venderName + 2) =  ecx;

	kPrintString(0, gScreenY++, "CPU Vender Name..............................[");
	kPrintString(46, gScreenY-1, venderName);
	kPrintString(58, gScreenY-1, "]");

	// Jump to Kernel64

	kPrintString(0, gScreenY++, "Copy Kernel64 to 2MB Area....................[PASS]");
	kCopyKernel64To2MbArea();
	kSwitchModeTo64();

	while(TRUE);
}

void kCopyKernel64To2MbArea(void)
{
	WORD wKerenl32_SectorCount, wTotal_SectorCount;
	DWORD *pdwSourceAddress, *pdwDestAddress;
	int i;

	wTotal_SectorCount		= *((WORD*) 0x7C05);
	wKerenl32_SectorCount	= *((WORD*) 0x7C07);

	pdwSourceAddress = (DWORD*) (0x10000 + (wKerenl32_SectorCount * 0x200));
	pdwDestAddress = (DWORD *) 0x200000;

	for ( i = 0; i < 0x200 * (wTotal_SectorCount - wKerenl32_SectorCount) / 4; i++ )
	{
		*pdwDestAddress = *pdwSourceAddress;
		pdwDestAddress++;
		pdwSourceAddress++;
	}
}

BOOL kInitializedKernel64Area(void)
{
	// 1MB
	DWORD* pCurrentAddress;
	pCurrentAddress = (DWORD*) 0x100000;

	while ( (DWORD) pCurrentAddress < 0x600000 )
	{
		*pCurrentAddress = 0x00;

		if ( *pCurrentAddress != 0 )
			return FALSE;

		pCurrentAddress++;
	}

	return TRUE;
}

BOOL kCheckMemoryCapacity(void)
{
	DWORD* pCurrentAddress;

	pCurrentAddress = (DWORD*) 0x100000;

	while ( (DWORD) pCurrentAddress < 0x4000000)
	{
		*pCurrentAddress = 0x12345678;

		if ( *pCurrentAddress != 0x12345678 )
		{
			return FALSE;
		}

		pCurrentAddress += (0x100000 / 4);
	}

	return TRUE;
}

void kPrintString(int nX, int nY, const char* pString)
{
	CHARACTER* pScreen = (CHARACTER*) 0xB8000;
	int i;

	pScreen += ( nY * 80 ) + nX;
	for (i = 0; pString[i] != 0; i++)
	{
		pScreen[i].bCharactor = pString[i];
	}
}
