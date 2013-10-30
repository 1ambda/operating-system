#include "Utility.h"
#include "InlineAssemblyUtility.h"

static QWORD gTotalSizeofRAM = 0;
volatile QWORD gTickCount = 0;


QWORD kGetTickCount(void)
{
	return gTickCount;
}

// this function should be used only once.
void kCheckTotalSizeofRAM(void)
{
	DWORD* pCurrentAddress;
	DWORD dwPrevValue;

	// from 64MB
	pCurrentAddress = (DWORD *) 0x4000000;

	while (TRUE)
	{
		dwPrevValue = *pCurrentAddress;

		*pCurrentAddress = 0x12345678;

		if ( *pCurrentAddress != 0x12345678 )
		{
			break;
		}

		*pCurrentAddress = dwPrevValue;
		// move into next 4MB area
		pCurrentAddress += (0x400000 / 4);

	}

	gTotalSizeofRAM = (QWORD) pCurrentAddress / 0x100000;
}

QWORD kGetTotalSizeofRAM(void)
{
	return gTotalSizeofRAM;
}


BOOL kSetInterruptFlag(BOOL bEnable)
{
	QWORD RFlags;

	RFlags = kReadRFlags();

	if ( bEnable )
		kEnableInterrupt();
	else
		kDisableInterrupt();

	if (RFlags & 0x0200)
		return TRUE;

	return FALSE;
}

int kStrlen(const char* string)
{
	int i;

	for(i = 0; ; i++)
	{
		if (string[i] == '\0')
			break;
	}

//	while( string[i] != '\0' )
//	{
//		i++;
//	}

	return i;
}

int kSprintf(char* buffer, const char* pFormatString, ...)
{
	va_list vl;
	int nReturn;

	va_start(vl, pFormatString);
	nReturn = kVsprintf(buffer, pFormatString, vl);
	va_end(vl);

	return nReturn;
}

int kVsprintf(char* buffer, const char* pFormatString, va_list vl)
{
	QWORD j;
	int nBufferIndex = 0, nFormatIndex = 0;
	int nFormatLength, nCopyLength;
	char* pCopyString;
	QWORD qwValue;
	int nValue;

	nFormatLength = kStrlen(pFormatString);

	for (nFormatIndex = 0; nFormatIndex < nFormatLength; nFormatIndex++)
	{
		if ( pFormatString[nFormatIndex] == '%' )
		{
			// move into next character
			nFormatIndex++;

			switch(pFormatString[nFormatIndex])
			{
			case	's':
				// String
				pCopyString = (char*) va_arg(vl, char*);
				nCopyLength = kStrlen(pCopyString);
				kMemcpy(buffer + nBufferIndex, pCopyString, nCopyLength);
				nBufferIndex += nCopyLength;
				break;

			case	'c':
				buffer[nBufferIndex] = (char) va_arg(vl, int);
				nBufferIndex++;
				break;

				// 4bytes Decimal
			case	'd':
			case	'i':
				nValue = va_arg(vl, int);
				nBufferIndex += kItoa(nValue, buffer + nBufferIndex, 10);
				break;
				// 4bytes Hex
			case	'x':
			case	'X':
				nValue = va_arg(vl, DWORD) & 0xFFFFFFFF;
				nBufferIndex += kItoa(nValue, buffer + nBufferIndex, 16);
				break;
				// 8bytes Hex
			case	'q':
			case	'Q':
			case	'p':
				qwValue = va_arg(vl, QWORD);
				nBufferIndex += kItoa(qwValue, buffer + nBufferIndex, 16);
				break;
			default:
				buffer[nBufferIndex] = pFormatString[nFormatIndex];
				nBufferIndex++;
				break;
			}

		}
		else
		{
			buffer[nBufferIndex] = pFormatString[nFormatIndex];
			nBufferIndex++;
		}
	}

	buffer[nBufferIndex] = '\0';
	return nBufferIndex;
}

void kReverseString(char* string)
{
	char temp;
	int i, nLength;

	nLength = kStrlen(string);

	for(i = 0; i < nLength / 2; i++)
	{
		temp = string[i];
		string[i] = string[nLength - 1 - i];
		string[nLength - 1 - i] = temp;
	}
}

int kHextoa(QWORD qwValue, char* buffer)
{
	QWORD i, qwCurrentValue;

	if ( qwValue == 0 )
	{
		buffer[0] = '0';
		buffer[1] = '\0';
		return 1;
	}

	for(i = 0; qwValue > 0; i++)
	{
		qwCurrentValue = qwValue % 16;

		if ( qwCurrentValue >= 10 )
		{
			buffer[i] = 'A' + (qwCurrentValue - 10);
		}
		else // qwCurrentValue < 10
		{
			buffer[i] = '0' + qwCurrentValue;
		}

		qwValue /= 16;
	}

	buffer[i] = '\0';
	kReverseString(buffer);

	return i;
}

int kDecimaltoa(long lValue, char* buffer)
{
	long i;

	if ( lValue == 0 )
	{
		buffer[0] = '0';
		buffer[1] = '\0';
		return 1;
	}

	if ( lValue < 0 )
	{
		buffer[0] = '-';
		i = 1;
		lValue = -lValue;
	}
	else
	{
		 i = 0;
	}

	for( ; lValue > 0; i++)
	{
		buffer[i] = '0' + lValue % 10;
		lValue /= 10;
	}

	buffer[i] = '\0';

	if ( buffer[0] == '-' )
		kReverseString(&(buffer[1]));
	else
		kReverseString(buffer);

	return i;
}

long kAtoi(const char* pBuffer, int radix)
{
	long lReturn;

	switch(radix)
	{
	case 16:
		lReturn = kHexstringToQword(pBuffer);
		break;
	case 10:
	default:
		lReturn = kDecimalstringToLong(pBuffer);
		break;
	}

	return lReturn;
}

QWORD kHexstringToQword(const char* buffer)
{
	QWORD qwValue = 0;
	int i;

	for ( i = 0; buffer[i] != '\0'; i++)
	{
		qwValue *= 16;

		if ( ('A' <= buffer[i]) && (buffer[i] <='F') )
		{
			qwValue += buffer[i] - 'A' + 10;
		}
		else if ( ('a' <= buffer[i]) && (buffer[i] <='f') )
		{
			qwValue += buffer[i] - 'a' + 10;
		}
		else if ( ('0' <= buffer[i]) && (buffer[i] <='9') )
		{
			qwValue += buffer[i] - '0';
		}
	}

	return qwValue;
}

long kDecimalstringToLong(const char* buffer)
{
	int i;
	long lValue = 0;

	if (buffer[0] == '-')
		i = 1;
	else
		i = 0;

	for(; buffer[i] != '\0'; i++)
	{
		lValue *= 10;
		lValue += buffer[i] - '0';
	}

	if ( buffer[0] == '-' )
		lValue = -lValue;

	return lValue;
}



int kItoa(long lValue, char* buffer, int radix)
{
	int nReturn;

	switch(radix)
	{
	case 16:
		nReturn = kHextoa(lValue, buffer);
		break;
	case 10:
	default:
		nReturn = kDecimaltoa(lValue, buffer);
		break;
	}


	return nReturn;
}


void kPrintStringXY(int x, int y, const char* pString)
{
	CHARACTER* pScreen = (CHARACTER *) 0xB8000;
	int i;

	pScreen += ( y * 80 ) + x;

	for (i = 0; pString[i] != 0; i++)
	{
		pScreen[i].bCharactor = pString[i];
	}
}

void kMemSetRange(void* pvStart, void* pvEnd, BYTE bData)
{
	kMemset(pvStart, ((char*) pvEnd) - ((char*) pvStart) + 1, bData);
}

BOOL kMemCheckRange(void* pvStart, void* pvEnd, BYTE bData)
{
	BYTE* i;

	for(i = (BYTE*) pvStart; i != (BYTE*) pvEnd + 1; i++)
	{
		if ( *i != bData)
		{
			return FALSE;
		}
	}

	return TRUE;
}


void kMemset(void* pvDest, BYTE bData, int Size)
{
	int i;

	for(i = 0; i< Size; i++)
	{
		( (char*) pvDest )[i] = bData;
	}
}

int	 kMemcpy(void* pvDest, const void* pvSrc, int Size)
{
	int i;

	for(i = 0; i < Size; i++)
	{
		((char*)pvDest)[i] = ((char*) pvSrc)[i];
	}

	return Size;
}

int	 kMemcmp(const void* pvDest, const void* pvSrc, int Size)
{
	int i;
	char cTemp;

	for(i = 0; i < Size; i++)
	{
		cTemp = ((char*)pvDest)[i] - ((char*)pvSrc)[i];
		if (cTemp != 0) {
			return (int) cTemp;
		}
	}

	return 0;
}
