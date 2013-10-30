#include "Types.h"
#include "InlineAssemblyUtility.h"
#include "Keyboard.h"
#include "Queue.h"
#include "Synchronization.h"


static QUEUE	gKeyQueue;
static KEYDATA	gKeyQueueBuffer[KEY_QUEUE_MAXCOUNT];

static KEYBOARDMANAGER gKeyboardManager = { 0 };
static KEYMAPPINGENTRY gKeyMappingTable[KEY_MAPPINGTABLEMAXCOUNT] =
{
    /*  0   */  {   KEY_NONE        ,   KEY_NONE        },
    /*  1   */  {   KEY_ESC         ,   KEY_ESC         },
    /*  2   */  {   '1'             ,   '!'             },
    /*  3   */  {   '2'             ,   '@'             },
    /*  4   */  {   '3'             ,   '#'             },
    /*  5   */  {   '4'             ,   '$'             },
    /*  6   */  {   '5'             ,   '%'             },
    /*  7   */  {   '6'             ,   '^'             },
    /*  8   */  {   '7'             ,   '&'             },
    /*  9   */  {   '8'             ,   '*'             },
    /*  10  */  {   '9'             ,   '('             },
    /*  11  */  {   '0'             ,   ')'             },
    /*  12  */  {   '-'             ,   '_'             },
    /*  13  */  {   '='             ,   '+'             },
    /*  14  */  {   KEY_BACKSPACE   ,   KEY_BACKSPACE   },
    /*  15  */  {   KEY_TAB         ,   KEY_TAB         },
    /*  16  */  {   'q'             ,   'Q'             },
    /*  17  */  {   'w'             ,   'W'             },
    /*  18  */  {   'e'             ,   'E'             },
    /*  19  */  {   'r'             ,   'R'             },
    /*  20  */  {   't'             ,   'T'             },
    /*  21  */  {   'y'             ,   'Y'             },
    /*  22  */  {   'u'             ,   'U'             },
    /*  23  */  {   'i'             ,   'I'             },
    /*  24  */  {   'o'             ,   'O'             },
    /*  25  */  {   'p'             ,   'P'             },
    /*  26  */  {   '['             ,   '{'             },
    /*  27  */  {   ']'             ,   '}'             },
    /*  28  */  {   '\n'            ,   '\n'            },
    /*  29  */  {   KEY_CTRL        ,   KEY_CTRL        },
    /*  30  */  {   'a'             ,   'A'             },
    /*  31  */  {   's'             ,   'S'             },
    /*  32  */  {   'd'             ,   'D'             },
    /*  33  */  {   'f'             ,   'F'             },
    /*  34  */  {   'g'             ,   'G'             },
    /*  35  */  {   'h'             ,   'H'             },
    /*  36  */  {   'j'             ,   'J'             },
    /*  37  */  {   'k'             ,   'K'             },
    /*  38  */  {   'l'             ,   'L'             },
    /*  39  */  {   ';'             ,   ':'             },
    /*  40  */  {   '\''            ,   '\"'            },
    /*  41  */  {   '`'             ,   '~'             },
    /*  42  */  {   KEY_LSHIFT      ,   KEY_LSHIFT      },
    /*  43  */  {   '\\'            ,   '|'             },
    /*  44  */  {   'z'             ,   'Z'             },
    /*  45  */  {   'x'             ,   'X'             },
    /*  46  */  {   'c'             ,   'C'             },
    /*  47  */  {   'v'             ,   'V'             },
    /*  48  */  {   'b'             ,   'B'             },
    /*  49  */  {   'n'             ,   'N'             },
    /*  50  */  {   'm'             ,   'M'             },
    /*  51  */  {   ','             ,   '<'             },
    /*  52  */  {   '.'             ,   '>'             },
    /*  53  */  {   '/'             ,   '?'             },
    /*  54  */  {   KEY_RSHIFT      ,   KEY_RSHIFT      },
    /*  55  */  {   '*'             ,   '*'             },
    /*  56  */  {   KEY_LALT        ,   KEY_LALT        },
    /*  57  */  {   ' '             ,   ' '             },
    /*  58  */  {   KEY_CAPSLOCK    ,   KEY_CAPSLOCK    },
    /*  59  */  {   KEY_F1          ,   KEY_F1          },
    /*  60  */  {   KEY_F2          ,   KEY_F2          },
    /*  61  */  {   KEY_F3          ,   KEY_F3          },
    /*  62  */  {   KEY_F4          ,   KEY_F4          },
    /*  63  */  {   KEY_F5          ,   KEY_F5          },
    /*  64  */  {   KEY_F6          ,   KEY_F6          },
    /*  65  */  {   KEY_F7          ,   KEY_F7          },
    /*  66  */  {   KEY_F8          ,   KEY_F8          },
    /*  67  */  {   KEY_F9          ,   KEY_F9          },
    /*  68  */  {   KEY_F10         ,   KEY_F10         },
    /*  69  */  {   KEY_NUMLOCK     ,   KEY_NUMLOCK     },
    /*  70  */  {   KEY_SCROLLLOCK  ,   KEY_SCROLLLOCK  },

    /*  71  */  {   KEY_HOME        ,   '7'             },
    /*  72  */  {   KEY_UP          ,   '8'             },
    /*  73  */  {   KEY_PAGEUP      ,   '9'             },
    /*  74  */  {   '-'             ,   '-'             },
    /*  75  */  {   KEY_LEFT        ,   '4'             },
    /*  76  */  {   KEY_CENTER      ,   '5'             },
    /*  77  */  {   KEY_RIGHT       ,   '6'             },
    /*  78  */  {   '+'             ,   '+'             },
    /*  79  */  {   KEY_END         ,   '1'             },
    /*  80  */  {   KEY_DOWN        ,   '2'             },
    /*  81  */  {   KEY_PAGEDOWN    ,   '3'             },
    /*  82  */  {   KEY_INS         ,   '0'             },
    /*  83  */  {   KEY_DEL         ,   '.'             },
    /*  84  */  {   KEY_NONE        ,   KEY_NONE        },
    /*  85  */  {   KEY_NONE        ,   KEY_NONE        },
    /*  86  */  {   KEY_NONE        ,   KEY_NONE        },
    /*  87  */  {   KEY_F11         ,   KEY_F11         },
    /*  88  */  {   KEY_F12         ,   KEY_F12         }
};

void kWaitingInputBuffer(BOOL bAction)
{
	int i;

	for (i = 0; i < 0xFFFF; i++)
	{
		if (kIsInputBufferFull() == bAction) {
			break;
		}
	}
}

void kWaitingOutputBuffer(BOOL bAction)
{
	int i;

	for (i = 0; i < 0xFFFF; i++) {
		if (kIsOutputBufferFull() == bAction) {
			break;
		}
	}
}

void kWaitingInputBufferEmpty(void)
{
	kWaitingInputBuffer(FALSE);
}

void kWaitingOutputBufferFull(void)
{
	kWaitingOutputBuffer(TRUE);
}

BOOL kIsOutputBufferFull(void)
{
	// Controller Command Register.OUTB(0)
	if (kReadPortByte(0x64) & 0x01)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL kIsInputBufferFull(void)
{
	// Controller Command Register.INPB(1)

	if (kReadPortByte(0x64) & 0x02) {
		return TRUE;
	}

	return FALSE;
}



BOOL kWaitingAckFromKeyboard(void)
{
	int		i;
	BYTE	bData;

	for(i = 0; i<100; i++) {
		kWaitingOutputBufferFull();

		// 0xFA == ACK
		bData = kReadPortByte(0x60);
		if ( bData == 0xFA ) {
			return TRUE;
		}

		kConvertScanCodeAndPutQueue(bData);
	}

	return FALSE;
}

BOOL kKeyboardInitialize(void)
{
	kQueueInitialize(&gKeyQueue, gKeyQueueBuffer, KEY_QUEUE_MAXCOUNT, sizeof(KEYDATA));

	return kActivateKeyboard();
}

void kEnable20GateByKeyboardController(void)
{
	BYTE bData;
	int i;

	kWritePortByte(0x64, 0xD0);

	kWaitingOutputBufferFull();

	bData = kReadPortByte(0x60);
	bData |= 0x02;

	kWaitingInputBufferEmpty();

	kWritePortByte(0x64, 0xD1);
	kWritePortByte(0x60, bData);

}

void kRebootByKeyboardController(void)
{
	kWaitingInputBufferEmpty();

	kWritePortByte(0x64, 0xD1);
	kWritePortByte(0x60, 0x00);
}

BOOL kActivateKeyboard(void)
{
	int i, j;
	BOOL bPrevInterrupt;
	BOOL bResult;


	// Disable Interupt
	bPrevInterrupt = kSetInterruptFlag(FALSE);

	// Enable Keyboard Device at 8042A
	kWritePortByte(0x64, 0xAE);

	kWaitingInputBufferEmpty();

	// Enable Keyboard Device itself.
	kWritePortByte(0x60, 0xF4);

	bResult = kWaitingAckFromKeyboard();

	kSetInterruptFlag(bPrevInterrupt);

	return bResult;
}

BOOL kChangeKeyboardLED(BOOL bCapsLockOn, BOOL bNumLockOn, BOOL bScrollLockOn)
{
	int i, j;
	BOOL bResult;
	BOOL bPrevInterruptFlag;
	BYTE bData;

	// Disable Interrupt
	bPrevInterruptFlag = kSetInterruptFlag(FALSE);

	kWaitingInputBufferEmpty();

	// Making change in LED status
	kWritePortByte(0x60, 0xED);

	// Keyboard device got 0xED
	kWaitingInputBufferEmpty();

	bResult = kWaitingAckFromKeyboard();

	if ( bResult == FALSE ) {
		kSetInterruptFlag(bPrevInterruptFlag);
		return FALSE ;
	}

	kWritePortByte(0x60, (bCapsLockOn << 2) | (bNumLockOn << 1) | bScrollLockOn);

	kWaitingInputBufferEmpty();

	bResult = kWaitingAckFromKeyboard();
	kSetInterruptFlag(bPrevInterruptFlag);

	return bResult;
}

BOOL kConvertScanCodeAndPutQueue(BYTE bScanCode)
{
	KEYDATA kd;
	BOOL bResult = FALSE;
	BOOL bPrevInterruptFlag;

	kd.bScan = bScanCode;

	if ( kConvertScanCodeToASCIICode(bScanCode, &(kd.bASCII), &(kd.bFlags)) == TRUE )
	{
		bPrevInterruptFlag = kLockInterrupt();
		// Critical Section Start
		{
			bResult = kQueuePut(&gKeyQueue, &kd);
		}
		// Critical Section End
		kUnlockInterrupt(bPrevInterruptFlag);
	}

	return bResult;
}

BOOL kGetKeyFromKeyQueue(KEYDATA* pData)
{
	BOOL bResult;
	BOOL bPrevInterruptFlag;

	if ( kQueueIsEmpry(&gKeyQueue) == TRUE )
		return FALSE;

	bPrevInterruptFlag = kLockInterrupt();
	// Critical Section Start
	{
		bResult = kQueueGet(&gKeyQueue, pData);
	}
	// Critical Section End
	kUnlockInterrupt(bPrevInterruptFlag);

	return bResult;
}

BYTE kGetKeyboardScanCode(void)
{
	while ( kIsOutputBufferFull() == FALSE);

	return kReadPortByte(0x60);
}

BOOL kConvertScanCodeToASCIICode(BYTE bScanCode, BYTE* pbASCIICode, BOOL* pbFlags)
{
	BOOL bUseCombinedKey;

	if ( gKeyboardManager.iSkipCountForPause > 0 ){
		gKeyboardManager.iSkipCountForPause--;
		return FALSE;
	}

	if (bScanCode == 0xE1) {
		*pbASCIICode = KEY_PAUSE;
		*pbFlags = KEY_FLAGS_DOWN;
		gKeyboardManager.iSkipCountForPause = KEY_SKIPCOUNTFORPAUSE;
		return TRUE;
	}

	if (bScanCode == 0xE0)
	{
		gKeyboardManager.bExtendedCodeIn = TRUE;
		return FALSE;
	}

	bUseCombinedKey = kIsUseCombinedCode(bScanCode);

	if (bUseCombinedKey == TRUE) {
		*pbASCIICode = gKeyMappingTable[bScanCode & 0x7F].bCombinedCode;
	}
	else {
		*pbASCIICode = gKeyMappingTable[bScanCode & 0x7F].bNormalCode;
	}

	if ( gKeyboardManager.bExtendedCodeIn == TRUE) {
		*pbFlags = KEY_FLAGS_EXTENDEDKEY;
		gKeyboardManager.bExtendedCodeIn = FALSE;
	}
	else {
		*pbFlags = 0;
	}

	if ( !(bScanCode & 0x80) ) {
		*pbFlags |= KEY_FLAGS_DOWN;
	}

	UpdateCombinationKeyStatusAndLED(bScanCode);

	return TRUE;
}
BOOL kIsAlphabetScanCode( BYTE bScanCode )
{
    if( ( 'a' <= gKeyMappingTable[ bScanCode ].bNormalCode ) &&
        ( gKeyMappingTable[ bScanCode ].bNormalCode <= 'z' ) )
    {
        return TRUE;
    }
    return FALSE;
}

BOOL kIsNumberOrSymbolScanCode( BYTE bScanCode )
{
    if( ( 2 <= bScanCode ) && ( bScanCode <= 53 ) &&
        ( kIsAlphabetScanCode( bScanCode ) == FALSE ) )
    {
        return TRUE;
    }

    return FALSE;
}

BOOL kIsNumberPadScanCode( BYTE bScanCode )
{
    if( ( 71 <= bScanCode ) && ( bScanCode <= 83 ) )
    {
        return TRUE;
    }

    return FALSE;
}

BOOL kIsUseCombinedCode( BOOL bScanCode )
{
    BYTE bDownScanCode;
    BOOL bUseCombinedKey;

    bDownScanCode = bScanCode & 0x7F;

    if( kIsAlphabetScanCode( bDownScanCode ) == TRUE )
    {
        if( gKeyboardManager.bShiftDown ^ gKeyboardManager.bCapsLockOn ) {
            bUseCombinedKey = TRUE;
        }
        else {
            bUseCombinedKey = FALSE;
        }
    }
    else if( kIsNumberOrSymbolScanCode( bDownScanCode ) == TRUE )
    {
        if( gKeyboardManager.bShiftDown == TRUE ) {
            bUseCombinedKey = TRUE;
        }
        else {
            bUseCombinedKey = FALSE;
        }
    }
    else if( ( kIsNumberPadScanCode( bDownScanCode ) == TRUE ) &&
             ( gKeyboardManager.bExtendedCodeIn == FALSE ) )
    {
        if( gKeyboardManager.bNumLockOn == TRUE ) {
            bUseCombinedKey = TRUE;
        }
        else {
            bUseCombinedKey = FALSE;
        }
    }

    return bUseCombinedKey;
}

void UpdateCombinationKeyStatusAndLED( BYTE bScanCode )
{
    BOOL bDown;
    BYTE bDownScanCode;
    BOOL bLEDStatusChanged = FALSE;

    if( bScanCode & 0x80 )
    {
        bDown = FALSE;
        bDownScanCode = bScanCode & 0x7F;
    }
    else
    {
        bDown = TRUE;
        bDownScanCode = bScanCode;
    }

    if( ( bDownScanCode == 42 ) || ( bDownScanCode == 54 ) )
    {
        gKeyboardManager.bShiftDown = bDown;
    }
    else if( ( bDownScanCode == 58 ) && ( bDown == TRUE ) )
    {
        gKeyboardManager.bCapsLockOn ^= TRUE;
        bLEDStatusChanged = TRUE;
    }
    else if( ( bDownScanCode == 69 ) && ( bDown == TRUE ) )
    {
        gKeyboardManager.bNumLockOn ^= TRUE;
        bLEDStatusChanged = TRUE;
    }
    else if( ( bDownScanCode == 70 ) && ( bDown == TRUE ) )
    {
        gKeyboardManager.bScrollLockOn ^= TRUE;
        bLEDStatusChanged = TRUE;
    }

    if( bLEDStatusChanged == TRUE )
    {
        kChangeKeyboardLED( gKeyboardManager.bCapsLockOn,
            gKeyboardManager.bNumLockOn, gKeyboardManager.bScrollLockOn );
    }
}








































