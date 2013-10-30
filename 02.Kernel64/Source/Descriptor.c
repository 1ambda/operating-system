#include "Descriptor.h"
#include "Utility.h"
#include "ISR.h"

BOOL kInitializeIDT(void)
{
	IDTR* pIDTR;
	IDT_ENTRY16* pEntry16;
	int i;

	// Reset
	kMemset((char *)START_ADDRESS_IDT, 0, SIZE_IDT_TABLE);
	if ( kMemCheckRange( (char *) START_ADDRESS_IDT,
			(char *) START_ADDRESS_IDT + SIZE_IDT_TABLE - 1, 0) == FALSE )
	{
		return FALSE;
	}


	pIDTR = (IDTR *) START_ADDRESS_IDTR;
	pEntry16 = (IDT_ENTRY16 *) START_ADDRESS_IDT;
	pIDTR->wLimit = SIZE_IDT_TABLE - 1;
	pIDTR->qwBaseAddress = (QWORD) pEntry16;

	// Set Exception ISR

	kSetIDTEntry16(&(pEntry16[0]), kIsrDivideError, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[1]), kIsrDebug, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[2]), kIsrNMI, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[3]), kIsrBreakpoint, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[4]), kIsrOverflow, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[5]), kIsrBoundRangeExceeded, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[6]), kIsrInvalidOpcode, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[7]), kIsrDeviceNotAvailable, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[8]), kIsrDoubleFault, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[9]), kIsrCorprocessorSegmentOverrun, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[10]), kIsrInvalidTSS, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[11]), kIsrSegmentNotPresent, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[12]), kIsrStackSegmentFault, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[13]), kIsrGeneralProtection, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[14]), kIsrPageFault, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[15]), kIsr15, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[16]), kIsrFPU, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[17]), kIsrAlignmentCheck, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[18]), kIsrMachineCheck, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[19]), kIsrSIMD, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[20]), kIsrEtcException, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);

	for ( i = 21; i < 32; i ++)
	{
		kSetIDTEntry16(&(pEntry16[i]), kIsrEtcException, 0x08, IDT_IST_1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	}

	kSetIDTEntry16(&(pEntry16[32]), kIsrTimer, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[33]), kIsrKeyboard, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[34]), kIsrSlavePIC, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[35]), kIsrSerial2, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[36]), kIsrSerial1, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[37]), kIsrParallel2, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[38]), kIsrFloppy, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[39]), kIsrParallel1, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[40]), kIsrRTC, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[41]), kIsrReserved, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[42]), kIsrNotUsed1, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[43]), kIsrNotUsed2, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[44]), kIsrMouse, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[45]), kIsrCoprocessor, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[46]), kIsrHDD1, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	kSetIDTEntry16(&(pEntry16[47]), kIsrHDD2, 0x08, IDT_IST_1,
			IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);

	for ( i = 48; i < COUNT_IDT_ENTRY16; i++)
	{
		kSetIDTEntry16(&(pEntry16[i]), kIsrEtcInterrupt, 0x08, IDT_IST_1,
				IDT_FLAGS_KERNEL, IDT_TYPE_INTERRUPT);
	}

	// Check Memory
	if ( kMemCheckRange( (char *) START_ADDRESS_IDT,
			(char *) START_ADDRESS_IDT + SIZE_IDT_TABLE - 1, 0) == TRUE )
	{
		return FALSE;
	}

	return TRUE;
}

void kSetIDTEntry16(IDT_ENTRY16* pEntry16, void* pHandler, WORD wSelector,
		BYTE bIST, BYTE bFlags, BYTE bType)
{
	pEntry16->wOffsetLow = (QWORD) pHandler & 0xFFFF;
	pEntry16->wSelector = wSelector;
	pEntry16->bIST = bIST & 0x7;
	pEntry16->bFlags = bFlags | bType;
	pEntry16->wOffsetMiddle = ( (QWORD) pHandler >> 16 ) & 0xFFFF;
	pEntry16->dwOffsetHigh = ( (QWORD) pHandler >> 32 );
	pEntry16->dwReserved = 0;
}


BOOL kInitializeGDTandTSS(void)
{
	GDTR* pGDTR;
	GDT_ENTRY8* pGDTEntry;
	TSS* pTSS;

	// Reset
	kMemSetRange((void *) START_ADDRESS_GDTR, (void *) START_ADDRESS_IDT - 1, 0);
	if ( kMemCheckRange((void *) START_ADDRESS_GDTR, (void *) START_ADDRESS_IDT - 1, 0) == FALSE )
	{
		return FALSE;
	}


	// Set GDTR
	pGDTR = (GDTR*) START_ADDRESS_GDTR;
	pGDTEntry = (GDT_ENTRY8*) START_ADDRESS_GDT;
	pGDTR->wLimit = SIZE_GDT_TABLE - 1;
	pGDTR->qwBaseAddress = (QWORD) pGDTEntry;
	// Set TSS
	pTSS = (TSS*) START_ADDRESS_TSS;


	// NULL		Desc.
	kSetGDTEntry8(&(pGDTEntry)[0], 0, 0, 0, 0, 0);
	// KCODE64	Desc.
	kSetGDTEntry8(&(pGDTEntry)[1], 0, 0xFFFFF, GDT_HFLAGS_64,
			GDT_LFLAGS_KERNEL, GDT_TYPE_CODE);
	// KDATA64	Desc.
	kSetGDTEntry8(&(pGDTEntry)[2], 0, 0xFFFFF, GDT_HFLAGS_64,
			GDT_LFLAGS_KERNEL, GDT_TYPE_DATA);
	// UCODE64	Desc.
	kSetGDTEntry8(&(pGDTEntry)[3], 0, 0xFFFFF, GDT_HFLAGS_64,
			GDT_LFLAGS_USER, GDT_TYPE_CODE);
	// UDATA64	Desc.
	kSetGDTEntry8(&(pGDTEntry)[4], 0, 0xFFFFF, GDT_HFLAGS_64,
			GDT_LFLAGS_USER, GDT_TYPE_DATA);
	// KCODE32	Desc.
	kSetGDTEntry8(&(pGDTEntry)[5], 0, 0xFFFFF, GDT_HFLAGS_32,
			GDT_LFLAGS_KERNEL, GDT_TYPE_CODE);
	// KDATA32	Desc.
	kSetGDTEntry8(&(pGDTEntry)[6], 0, 0xFFFFF, GDT_HFLAGS_32,
			GDT_LFLAGS_KERNEL, GDT_TYPE_DATA);
	// UCODE32	Desc.
	kSetGDTEntry8(&(pGDTEntry)[7], 0, 0xFFFFF, GDT_HFLAGS_32,
			GDT_LFLAGS_USER, GDT_TYPE_CODE);
	// UDATA32	Desc.
	kSetGDTEntry8(&(pGDTEntry)[8], 0, 0xFFFFF, GDT_HFLAGS_32,
			GDT_LFLAGS_USER, GDT_TYPE_DATA);
	// TSS		Desc
	kSetGDTEntry16((GDT_ENTRY16*) &(pGDTEntry)[9], (QWORD)pTSS, sizeof(TSS) - 1,
			GDT_HFLAGS_TSS, GDT_LFLAGS_TSS, GDT_TYPE_TSS );

	kInitializeTSS(pTSS);

	// 0x142000 ~ 0x200000 - 1 should not be all-zero area.
	if ( kMemCheckRange((void*) START_ADDRESS_GDTR, (void *)START_ADDRESS_IDT - 1, 0) == TRUE )
	{
		return FALSE;
	}


	return TRUE;
}

void kSetGDTEntry8(GDT_ENTRY8* pEntry8, DWORD dwBaseAddress,
		DWORD dwLimit, BYTE bFlagsHigh, BYTE bFlagsLow, BYTE bType )
{
	pEntry8->wLimitLow = dwLimit & 0xFFFF;
	pEntry8->wBaseAddressLow = dwBaseAddress & 0xFFFF;
	pEntry8->bBaseAddressHigh1 = (dwBaseAddress >> 16) & 0xFF;
	pEntry8->bFlagsLow = bFlagsLow | bType;
	pEntry8->bFlagsHigh = bFlagsHigh | ((dwLimit >> 16) & 0xFF);
	pEntry8->bBaseAddressHigh2 = (dwBaseAddress >> 24) & 0xFF;

	return;
}

void kSetGDTEntry16(GDT_ENTRY16* pEntry16, QWORD qwBaseAddress,
		DWORD dwLimit, BYTE bFlagsHigh, BYTE bFlagsLow, BYTE bType )
{
	pEntry16->wLimitLow = dwLimit & 0xFFFF;
	pEntry16->wBaseAddressLow = qwBaseAddress & 0xFFFF;
	pEntry16->bBaseAddressMiddle1 = (qwBaseAddress >> 16) & 0xFF;
	pEntry16->bFlagsLow = bFlagsLow | bType;
	pEntry16->bFlagsHigh = bFlagsHigh | ((dwLimit >> 16) & 0xFF);
	pEntry16->bBaseAddressMiddle2 = (qwBaseAddress >> 24) & 0xFF;
	pEntry16->dwBaseAddressHigh = (qwBaseAddress >> 32);
	pEntry16->dwReserved = 0;

	return;
}

void kInitializeTSS(TSS* pTSS)
{
	kMemset( pTSS, 0, sizeof(TSS) );
	pTSS->qwIST[0] = START_ADDRESS_IST + SIZE_IST;
	pTSS->wIOMapBaseAddress = 0xFFFF;
	return;
}








