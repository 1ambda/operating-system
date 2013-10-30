#ifndef __DESCRIPTOR_H__
#define __DESCRIPTOR_H__

#include "Types.h"


#define	COUNT_GDT_ENTRY8	9
#define COUNT_GDT_ENTRY16	1
#define COUNT_TSS			1
#define COUNT_IDT_ENTRY16	100

#define SIZE_GDT_TABLE		( (sizeof(GDT_ENTRY8) * COUNT_GDT_ENTRY8) + (sizeof(GDT_ENTRY16) * COUNT_GDT_ENTRY16 ))
#define SIZE_IDT_TABLE		( sizeof(IDT_ENTRY16) * COUNT_IDT_ENTRY16)
#define	SIZE_TSS			( sizeof(TSS) )
#define SIZE_IST			0x100000

#define START_ADDRESS_GDTR	0x142000
#define START_ADDRESS_IDTR	( START_ADDRESS_GDTR + (sizeof(GDTR)) )
#define START_ADDRESS_TSS	( START_ADDRESS_IDTR + (sizeof(IDTR)) )
#define START_ADDRESS_GDT	0x152000
#define START_ADDRESS_IDT	0x162000
#define START_ADDRESS_IST	0x700000

// MACRO FOR GDT START

#define	GDT_TYPE_CODE		0x0A
#define GDT_TYPE_DATA		0x02
#define GDT_TYPE_TSS		0x09

#define GDT_LFLAG_S		0x10
#define GDT_LFLAG_DPL0		0x00
#define GDT_LFLAG_DPL1		0x20
#define GDT_LFLAG_DPL2		0x40
#define GDT_LFLAG_DPL3		0x60
#define GDT_LFLAG_P		0x80

#define GDT_HFLAG_L		0x20
#define GDT_HFLAG_DB		0x40
#define GDT_HFLAG_G		0x80


#define GDT_LFLAGS_KERNEL	( GDT_LFLAG_S | GDT_LFLAG_DPL0 | GDT_LFLAG_P )
#define GDT_LFLAGS_USER		( GDT_LFLAG_S | GDT_LFLAG_DPL3 | GDT_LFLAG_P )
#define GDT_LFLAGS_TSS		( GDT_LFLAG_DPL0 | GDT_LFLAG_P )

#define GDT_HFLAGS_64	( GDT_HFLAG_G | GDT_HFLAG_L )
#define GDT_HFLAGS_32	( GDT_HFLAG_G | GDT_HFLAG_DB )
#define	GDT_HFLAGS_TSS	( GDT_HFLAG_G )

#define GDT_OFFSET_NULL			0x00
#define GDT_OFFSET_KERNEL64CODE	0x08
#define GDT_OFFSET_KERNEL64DATA 0x10
#define GDT_OFFSET_USER64CODE	0x18
#define GDT_OFFSET_USER64DATA	0x20
#define GDT_OFFSET_KERNEL32CODE 0x28
#define GDT_OFFSET_KERNEL32DATA 0x30
#define GDT_OFFSET_USER32CODE	0x38
#define GDT_OFFSET_USER32DATA	0x40
#define GDT_OFFSET_TSS			0x48


// MACRO FOR GDT END

// MACRO FOR IDT START

#define IDT_TYPE_INTERRUPT		0x0E
#define IDT_TYPE_TARP			0x0F

#define	IDT_FLAG_DPL0			0x00
#define	IDT_FLAG_DPL1			0x20
#define	IDT_FLAG_DPL2			0x40
#define	IDT_FLAG_DPL3			0x60
#define IDT_FLAG_P				0x80

#define IDT_IST_NO				0
#define IDT_IST_1				1

#define IDT_FLAGS_KERNEL		(IDT_FLAG_DPL0 | IDT_FLAG_P)
#define IDT_FLAGS_USER			(IDT_FLAG_DPL3 | IDT_FLAG_P)

// MACRO FOR IDT END

// STRUCTURE START
#pragma pack(push, 1)

typedef struct kGDTRStruct
{
	WORD	wLimit;
	QWORD	qwBaseAddress;
	WORD	wPadding;
	DWORD	dwPadding;

}GDTR, IDTR;

typedef struct kGDTEntry8Struct
{
	WORD	wLimitLow;
	WORD	wBaseAddressLow;
	BYTE	bBaseAddressHigh1;
	BYTE	bFlagsLow;
	BYTE	bFlagsHigh;
	BYTE	bBaseAddressHigh2;

} GDT_ENTRY8;

typedef struct kGDTEntry16Struct
{

	WORD	wLimitLow;
	WORD	wBaseAddressLow;
	BYTE	bBaseAddressMiddle1;
	BYTE	bFlagsLow;
	BYTE	bFlagsHigh;
	BYTE	bBaseAddressMiddle2;
	DWORD	dwBaseAddressHigh;
	DWORD	dwReserved;

} GDT_ENTRY16;

typedef struct kTSSstruct
{
	DWORD	dwReserved1;
	QWORD	qwRSP[3];
	QWORD	qwReserved2;
	QWORD	qwIST[7];
	WORD	wReserved3;
	WORD	wIOMapBaseAddress;

} TSS;
typedef struct kIDTEntry16Struct
{
	WORD	wOffsetLow;
	WORD	wSelector;
	BYTE	bIST;
	BYTE	bFlags;
	WORD	wOffsetMiddle;
	DWORD	dwOffsetHigh;
	DWORD	dwReserved;

} IDT_ENTRY16;

#pragma pack(pop)
// STRUCTURE END


// FUNCTION LIST START

BOOL kInitializeGDTandTSS(void);
void kSetGDTEntry8(GDT_ENTRY8* pEntry8, DWORD dwBaseAddress,
		DWORD dwLimit, BYTE bFlagsHigh, BYTE bFlagsLow, BYTE bType );
void kSetGDTEntry16(GDT_ENTRY16* pEntry16, QWORD qwBaseAddress,
		DWORD dwLimit, BYTE bFlagsHigh, BYTE bFlagsLow, BYTE bType );
void kInitializeTSS(TSS* pTSS);

BOOL kInitializeIDT(void);
void kSetIDTEntry16(IDT_ENTRY16* pEntry16, void* pHandler, WORD wSelector,
		BYTE bIST, BYTE bFlags, BYTE bType);

// FUNCTION LIST END







#endif
