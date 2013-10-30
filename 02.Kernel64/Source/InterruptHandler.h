#ifndef __INTERRUPT_HANDLER__
#define __INTERRUPT_HANDLER__

#include "Types.h"
#include "PIC.h"
#include "Utility.h"
#include "Task.h"
#include "Descriptor.h"


void kHandlerCommonException(int nVector, QWORD qwErrorCode);
void kHandlerCommonInterrupt(int nVector);
void kHandlerKeyboardInterrupt(int nVector);

#endif
