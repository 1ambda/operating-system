#include <stdarg.h>
#include "Console.h"
#include "InlineAssemblyUtility.h"
#include "Keyboard.h"
#include "Utility.h"
#include "Task.h"

CONSOLE_MANAGER gConsoleManager = { 0 };

void kConsoleInitialize(int x, int y)
{
	kMemset(&gConsoleManager, 0, sizeof(gConsoleManager));

	kSetCursor(x, y);
}

void kSetCursor(int x, int y)
{
	int nCursorLinearOffset;

	nCursorLinearOffset = (y * CONSOLE_SCREEN_WIDTH) + x;

	kWritePortByte(CRTC_ADR, CRTC_ADR_CURSOR_UPPER);
	kWritePortByte(CRTC_CDR, nCursorLinearOffset >> 8);

	kWritePortByte(CRTC_ADR, CRTC_ADR_CURSOR_LOWER);
	kWritePortByte(CRTC_CDR, nCursorLinearOffset & 0xFF);

	gConsoleManager.nCurrentPrintOffset = nCursorLinearOffset;
}

void kGetCursor(int* x, int* y)
{

	*x = gConsoleManager.nCurrentPrintOffset % CONSOLE_SCREEN_WIDTH;
	*y = gConsoleManager.nCurrentPrintOffset / CONSOLE_SCREEN_WIDTH;

	return;
}

void kPrintf(const char* pFormatString, ...)
{
	va_list vl;
	char buffer[1024];
	int nNextPrintOffset;

	va_start(vl, pFormatString);
	kVsprintf(buffer, pFormatString, vl);
	va_end(vl);

	nNextPrintOffset = kInternalPrintf(buffer);

	kSetCursor(nNextPrintOffset % CONSOLE_SCREEN_WIDTH, nNextPrintOffset / CONSOLE_SCREEN_WIDTH);
}

int kInternalPrintf(const char* string)
{
	CHARACTER* pScreen = (CHARACTER *) CONSOLE_SCREEN_ADDRESS;
	int i, j;
	int nLength;
	int nPrintOffset;

	nPrintOffset = gConsoleManager.nCurrentPrintOffset;
	nLength = kStrlen(string);

	for (i = 0; i < nLength; i++)
	{
		if ( string[i] == '\n' )
		{
			nPrintOffset += (CONSOLE_SCREEN_WIDTH - (nPrintOffset % CONSOLE_SCREEN_WIDTH));
		}
		else if ( string[i] == '\t' )
		{
			nPrintOffset += (8 - (nPrintOffset % 8));
		}
		else
		{
			pScreen[nPrintOffset].bCharactor = string[i];
			pScreen[nPrintOffset].bAttribute = CONSOLE_DEFAULT_TEXTCOLOR;
			nPrintOffset++;
		}

		// Scrolling
		if ( nPrintOffset >= (CONSOLE_SCREEN_WIDTH * CONSOLE_SCREEN_HEIGHT))
		{
			// first line is used for printnig debug inforamtion.
			kMemcpy((void *)CONSOLE_SCREEN_ADDRESS + CONSOLE_SCREEN_WIDTH * sizeof(CHARACTER),
					(void *)CONSOLE_SCREEN_ADDRESS + CONSOLE_SCREEN_WIDTH * sizeof(CHARACTER) * 2,
					(CONSOLE_SCREEN_HEIGHT - 1) * CONSOLE_SCREEN_WIDTH * sizeof(CHARACTER) );

			for ( j = (CONSOLE_SCREEN_HEIGHT - 1) * CONSOLE_SCREEN_WIDTH;
					j < (CONSOLE_SCREEN_HEIGHT * CONSOLE_SCREEN_WIDTH); j++ )
			{
				pScreen[j].bCharactor = ' ';
				pScreen[j].bAttribute = CONSOLE_DEFAULT_TEXTCOLOR;
			}

			nPrintOffset = (CONSOLE_SCREEN_HEIGHT -1) * CONSOLE_SCREEN_WIDTH;
		}
	}

	return nPrintOffset;
}

void kClearScreen(void)
{
	CHARACTER* pScreen = (CHARACTER *) CONSOLE_SCREEN_ADDRESS;
	int i;

	for (i = 0; i < CONSOLE_SCREEN_WIDTH * CONSOLE_SCREEN_HEIGHT; i++)
	{
		pScreen[i].bAttribute = CONSOLE_DEFAULT_TEXTCOLOR;
		pScreen[i].bCharactor = ' ';
	}

	kSetCursor(0, 0);
}

BYTE kGetch(void)
{
	KEYDATA kd;

	while(1)
	{
		while ( kGetKeyFromKeyQueue(&kd) == FALSE )
		{
			kTaskSchedule();
		}

		if ( kd.bFlags & KEY_FLAGS_DOWN )
		{
			return kd.bASCII;
		}
	}

	return kd.bASCII;
}










