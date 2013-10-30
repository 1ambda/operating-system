#ifndef __PAGE_H__
#define __PAGE_H__

#include "Types.h"

#define PAGE_FLAG_P			0x00000001 // Present
#define PAGE_FLAG_RW		0x00000002 // Read/Write
#define PAGE_FLAG_US		0x00000004 // User/Supervisor
#define PAGE_FLAG_PWT		0x00000008 // Page-Level Write-Through
#define PAGE_FLAG_PCD		0x00000010 // Page-Level Cache Disable
#define PAGE_FLAG_A			0x00000020 // Accessed
#define PAGE_FLAG_D			0x00000040 // Dirty
#define PAGE_FLAG_PS		0x00000080 // Page Size
#define PAGE_FLAG_G			0x00000100 // Global
#define PAGE_FLAG_PAT		0x00001000 // Present
#define PAGE_FLAG_EXB		0x80000000 // Present

#define PAGE_FLAG_DEFAULT				(PAGE_FLAG_P | PAGE_FLAG_RW)
#define PAGE_STRUCTURE_SIZE				0x1000 // 4092
#define PAGE_STRUCTURE_ENTRY_NUMBER		0x200 // 512
#define PAGE_DEFAULT_SIZE				0x200000 // 2MB

#define CURRENT_PDPTE_NUMBER	64 // 64GB
#define CURRENT_PML4E_NUMBER 	1

#pragma pack(push, 1)


typedef struct kPageStructEntry
{
	DWORD dwLower;
	DWORD dwUpper;
} PML4Entry, PDPTEntry, PDEntry, PTEntry, PStEntry;

void kInitializePageTables(void);
void kSetPageStructureEntry(PStEntry* pPStE, DWORD dwUpperContent, DWORD dwLowerContent,
		DWORD dwLowerFlags, DWORD dwUpperFlags );

#pragma pack(pop)


#endif
