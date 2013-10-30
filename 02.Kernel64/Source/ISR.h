#ifndef __ISR_H__
#define __ISR_H__

// Exception Routines


void kIsrDivideError(void);
void kIsrDebug(void);
void kIsrNMI(void);
void kIsrBreakpoint(void);
void kIsrOverflow(void);
void kIsrBoundRangeExceeded(void);
void kIsrInvalidOpcode(void);
void kIsrDeviceNotAvailable(void);
void kIsrDoubleFault(void);
void kIsrCorprocessorSegmentOverrun(void);
void kIsrInvalidTSS(void);
void kIsrSegmentNotPresent(void);
void kIsrStackSegmentFault(void);
void kIsrGeneralProtection (void);
void kIsrPageFault(void);
void kIsr15(void);
void kIsrFPU(void);
void kIsrAlignmentCheck(void);
void kIsrMachineCheck(void);
void kIsrSIMD(void);
void kIsrEtcException(void);


// Interrupt Routines

void kIsrTimer(void);
void kIsrKeyboard(void);
void kIsrSlavePIC(void);
void kIsrSerial1(void);
void kIsrSerial2(void);
void kIsrParallel2(void);
void kIsrFloppy(void);
void kIsrParallel1(void);
void kIsrRTC(void);
void kIsrReserved(void);
void kIsrNotUsed1(void);
void kIsrNotUsed2(void);
void kIsrMouse(void);
void kIsrCoprocessor(void);
void kIsrHDD1(void);
void kIsrHDD2(void);
void kIsrEtcInterrupt(void);

#endif
