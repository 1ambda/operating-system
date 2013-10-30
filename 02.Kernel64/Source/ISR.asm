[BITS 64]

section .text

extern kHandlerCommonException, kHandlerCommonInterrupt, kHandlerKeyboardInterrupt
extern kTimerHandler
; specified ISR will call the appropriate handler.

; a list of exception handlers 
global kIsrDivideError, kIsrDebug, kIsrNMI, kIsrBreakpoint, kIsrOverflow
global kIsrBoundRangeExceeded, kIsrInvalidOpcode, kIsrDeviceNotAvailable, kIsrDoubleFault
global kIsrCorprocessorSegmentOverrun, kIsrInvalidTSS, kIsrSegmentNotPresent
global kIsrStackSegmentFault, kIsrGeneralProtection, kIsrPageFault, kIsr15
global kIsrFPU, kIsrAlignmentCheck, kIsrMachineCheck, kIsrSIMD, kIsrEtcException

; a list of interrupt handlers
global kIsrTimer, kIsrKeyboard, kIsrSlavePIC, kIsrSerial1, kIsrSerial2, kIsrParallel2
global kIsrFloppy, kIsrParallel1, kIsrRTC, kIsrReserved, kIsrNotUsed1, kIsrNotUsed2
global kIsrMouse, kIsrCoprocessor, kIsrHDD1, kIsrHDD2, kIsrEtcInterrupt

%MACRO KSAVECONTEXT	0
	push rbp
	mov rbp, rsp
	push rax
	push rbx
	push rcx
	push rdx

	push rdi
	push rsi
	
	push r8	
	push r9	
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	
	mov ax, ds
	push rax
	mov ax, es
	push rax
	push fs
	push gs
	
	mov ax, 0x10	; kernel data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
%ENDMACRO

%MACRO	KLOADCONTEXT 0
	pop gs
	pop fs
	pop rax
	mov es, ax
	pop rax
	mov ds, ax
	
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	
	pop rsi
	pop rdi
	
	pop rdx
	pop rcx
	pop rbx
	pop rax
	
	pop rbp

%ENDMACRO

;;;;;;;;;;;;;;;;;;;;;;;;
;; Exception Handlers ;;
;;;;;;;;;;;;;;;;;;;;;;;;

; #0 Divide Error Isr
kIsrDivideError:
	
	KSAVECONTEXT
	
	mov rdi, 0
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq
	
; #1 Debug Isr
kIsrDebug:
	
	KSAVECONTEXT
	
	mov rdi, 1
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq
	
; #2 NMI Isr
kIsrNMI:
	
	KSAVECONTEXT
	
	mov rdi, 2
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq
	
; #3 BreakPoint Isr
kIsrBreakpoint:
	
	KSAVECONTEXT
	
	mov rdi, 3
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq

; #4 Overflow Isr
kIsrOverflow:
	
	KSAVECONTEXT
	
	mov rdi, 4
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq

; #5 Bound Range Exceeded Isr
kIsrBoundRangeExceeded:
	
	KSAVECONTEXT
	
	mov rdi, 5
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq

; #6 Invalid Opcode Isr
kIsrInvalidOpcode:
	
	KSAVECONTEXT
	
	mov rdi, 6
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq

; #7 Device Not Available Isr
kIsrDeviceNotAvailable:
	
	KSAVECONTEXT
	
	mov rdi, 7
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq

; #8 Double Fault Isr
kIsrDoubleFault:
	
	KSAVECONTEXT
	
	mov rdi, 8
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq

; #9 Coprocessor segment overrun Isr
kIsrCorprocessorSegmentOverrun:
	
	KSAVECONTEXT
	
	mov rdi, 9
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq
	
; #10 Invalid TSS Isr
kIsrInvalidTSS:
	
	KSAVECONTEXT
	
	mov rdi, 10
	; error code
	mov rsi, qword[rbp +8]
	call kHandlerCommonException
	
	KLOADCONTEXT
	add rsp, 8
	iretq	
	
; #11 Segment Not Present Isr
kIsrSegmentNotPresent:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 11
	; error code
	mov rsi, qword[rbp +8]
	
	call kHandlerCommonException
	
	KLOADCONTEXT
	add rsp, 8
	iretq	
	
; #12 Stack Segment Fault Isr
kIsrStackSegmentFault:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 12
	; error code
	mov rsi, qword[rbp +8]
	
	call kHandlerCommonException
	
	KLOADCONTEXT
	add rsp, 8
	iretq	
	
; #13 General Protection Isr
kIsrGeneralProtection:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 13
	; error code
	mov rsi, qword[rbp +8]
	
	call kHandlerCommonException
	
	KLOADCONTEXT
	add rsp, 8
	iretq		
	
; #14 Page Fault Isr
kIsrPageFault:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 14
	; error code
	mov rsi, qword[rbp +8]
	
	call kHandlerCommonException
	
	KLOADCONTEXT
	add rsp, 8
	iretq		
	
; #15 Reserved Isr
kIsr15:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 15
	
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq		


; #16 FPU Error Isr
kIsrFPU:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 16
	
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq		

; #17 Alignment Check Isr
kIsrAlignmentCheck:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 17
	mov rsi, qword[rbp + 8]
	
	call kHandlerCommonException
	
	KLOADCONTEXT
	add rsp, 8
	iretq		

; #18 Machine Check Isr
kIsrMachineCheck:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 18
	
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq		

; #19 kIsrSIMD Isr
kIsrSIMD:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 19
	
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq		
	
; #20~#31 Reserved Isr
kIsrEtcException:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 20
	
	call kHandlerCommonException
	
	KLOADCONTEXT
	iretq		

;;;;;;;;;;;;;;;;;;;;;;;;
;; Interrupt Handlers ;;
;;;;;;;;;;;;;;;;;;;;;;;;

; #32 Timer Isr
kIsrTimer:
	
	KSAVECONTEXT
	
	; Interrupt number
	mov rdi, 32
	
	call kTimerHandler
	
	KLOADCONTEXT
	iretq		

; #33 Keyboard Isr
kIsrKeyboard:
	
	KSAVECONTEXT
	
	; Interrupt number
	mov rdi, 33
	
	call kHandlerKeyboardInterrupt
	
	KLOADCONTEXT
	iretq		

; #34 Slave PIC Isr
kIsrSlavePIC:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 34
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		

; #35 Serial2 Isr
kIsrSerial2:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 35
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		
	
; #36 Serial1 Isr
kIsrSerial1:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 36
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		

; #37 Parallel2 Isr
kIsrParallel2:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 37
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		

; #38 Floppy Isr
kIsrFloppy:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 38
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		

; #39 Parallel1 Isr
kIsrParallel1:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 39
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		
	
; #40 RTC Isr
kIsrRTC:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 40
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		

; #41 Reserved Interrupt Isr
kIsrReserved:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 41
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		

; #42 Not Used1 Isr
kIsrNotUsed1:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 42
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		

; #43 Not Used2 Isr
kIsrNotUsed2:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 43
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		

; #44 Mouse Isr
kIsrMouse:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 44
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		
	
; #45 Coprocessor Isr
kIsrCoprocessor:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 45
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		

; #46 HDD1 Isr
kIsrHDD1:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 46
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		

; #47 HDD2 Isr
kIsrHDD2:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 47
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		
	
; #48 ~ #255 Etc INterrupt Isr
kIsrEtcInterrupt:
	
	KSAVECONTEXT
	
	; exception number
	mov rdi, 48
	
	call kHandlerCommonInterrupt
	
	KLOADCONTEXT
	iretq		















