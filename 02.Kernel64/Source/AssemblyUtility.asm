[BITS 64]

global	kTest, kTest1
global	kReadPortByteAsm, kWritePortByteAsm
global	kLoadGDTRAsm, kLoadIDTRAsm, kLoadTRAsm
global	kEnableInterrupt, kDisableInterrupt, kReadRFlags
global	kReadTscAsm
global	kSwitchContext
global	kHaltAsm
global	kAtomicCmpSet

extern Hello

SECTION .text

%MACRO	KSAVECONTEXT 0
	push rbp
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

	mov ax, ds ; ds
	push rax
	mov ax, es ; es
	push rax
	push fs
	push gs
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

kAtomicCmpSet:
;; BOOL kAtomicCmpSet(volatile BYTE* pDest, BYTE bCmp, BYTE Src) : rdi, rsi, rdx
	mov rax, rsi

	lock cmpxchg byte[rdi], dl
	je .SUCCESS

.FAIL:
	mov rax, 0x00
	ret

.SUCCESS:
	mov rax, 0x01
	ret

kHaltAsm:
	hlt
	hlt
	ret

kSwitchContext:
	push rbp
	mov rbp, rsp

	; if current context is null, we don't have to save current context.
	pushfq
	cmp rdi, 0 ; save zf
	je _LoadContext
	popfq

	push rax ; save rax

	mov ax, ss
	mov qword[ rdi + (23 * 8)], rax

	mov rax, rbp
	add rax, 16 ; original rbp before we pushed
	mov qword[ rdi + (22 * 8)], rax

	pushfq
	pop rax
	mov qword[ rdi + (21 * 8)], rax

	mov ax, cs
	mov qword[ rdi + (20 * 8)], rax

	mov rax, qword[rbp + 8] ; saved rip
	mov qword [rdi + (19 * 8)], rax

	pop rax
	pop rbp

	add rdi, (8 * 19)
	mov rsp, rdi
	sub rdi, (8 * 19)

	KSAVECONTEXT

_LoadContext:
	mov rsp, rsi	; rsi : next context
	KLOADCONTEXT
	iretq 			; interrupt return. this operation automatically pops 'rip, cs, rfalgs, rbp, ss'




kTest:
	call Hello
	ret

kTest1:
	call kEnableInterrupt
	ret



kReadTscAsm:
	; no Param
	push rdx
	rdtsc

	shl rdx, 32
	or rax, rdx

	pop rdx
	ret

kEnableInterrupt:
	sti
	ret

kDisableInterrupt:
	cli
	ret

kReadRFlags:
	pushfq
	pop rax

	ret

kReadPortByteAsm:
	push rdx

	mov rdx, rdi
	mov rax, 0
	in al, dx

	pop rdx
	ret

kWritePortByteAsm:
	push rdx
	push rax

	mov rdx, rdi
	mov rax, rsi

	out dx, al

	pop rax
	pop rdx
	ret

kLoadTRAsm:
	ltr		di
	ret

kLoadGDTRAsm:
	lgdt	[rdi]
	ret

kLoadIDTRAsm:
	lidt	[rdi]
	ret


