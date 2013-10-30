/*
 * Page.c

 *
 *  Created on: 2013. 1. 11.
 *      Author: Administrator
 */

#include "Page.h"

void kInitializePageTables(void)
{
	PML4Entry*	pPML4E;
	PDPTEntry*	pPDPTE;
	PDEntry*	pPDE;
	DWORD dwMappingAddress;
	DWORD i;

	// Create PML4 Table
	pPML4E = (PML4Entry *) 0x100000; // 1M
	kSetPageStructureEntry( &(pPML4E[0]), 0x00, 0x101000, PAGE_FLAG_DEFAULT, 0 );

	for( i = CURRENT_PML4E_NUMBER; i < PAGE_STRUCTURE_ENTRY_NUMBER; i++ )
	{
		kSetPageStructureEntry( &(pPML4E[i]), 0x00, 0x00, 0, 0);
	}

	// Create Page Directory Pointer Table
	pPDPTE = (PDPTEntry *) 0x101000;

	for ( i = 0; i < CURRENT_PDPTE_NUMBER; i++)
	{
		kSetPageStructureEntry( &(pPDPTE[i]), 0x00, 0x102000 + (i * PAGE_STRUCTURE_SIZE), PAGE_FLAG_DEFAULT, 0);
	}

	for ( i = CURRENT_PDPTE_NUMBER; i < PAGE_STRUCTURE_ENTRY_NUMBER; i++)
	{
		kSetPageStructureEntry( &(pPDPTE[i]), 0x00, 0x00, 0, 0);
	}

	// Create Page Directory

	pPDE = (PDEntry *) 0x102000;
	dwMappingAddress = 0;

	for ( i = 0; i < PAGE_STRUCTURE_ENTRY_NUMBER * CURRENT_PDPTE_NUMBER; i++ )
	{
		kSetPageStructureEntry( &(pPDE[i]), (i * (PAGE_DEFAULT_SIZE >> 20 )) >> 12, dwMappingAddress,
				PAGE_FLAG_DEFAULT | PAGE_FLAG_PS, 0 );

		dwMappingAddress += PAGE_DEFAULT_SIZE;
	}


}

void kSetPageStructureEntry(PStEntry* pPStE, DWORD dwUpperContent, DWORD dwLowerContent,
		DWORD dwLowerFlags, DWORD dwUpperFlags )
{
	pPStE->dwLower = dwLowerContent | dwLowerFlags;
	pPStE->dwUpper = (dwUpperContent & 0xFF) | dwUpperFlags;
}
