[ORG 0x00]
[BITS 16]

SECTION .text

;
; Code Area
;

START:

	mov	ax,	0x1000
	mov	ds,	ax
	mov	es,	ax
	
	; A20 Gate On using BIOS Service 0x15
	mov ax, 0x2401
	int 0x15
	jc A20_GATE_FAIL
	jmp A20_GATE_SUCCESS

A20_GATE_FAIL:
	in al, 0x92
	or al, 0x02
	and al, 0xFE
	out 0x92, al

A20_GATE_SUCCESS:

	cli ; disable interrupt
	lgdt [GDTR] ; load GDT data structure at GDTR

	; set CR0
	mov eax, 0x4000003B
	mov cr0, eax

	; jmp to Protected Mode using kernel code segment descriptor 0x08
	; and set EIP

	jmp dword 0x18: ( PROTECTED_MODE - $$ + 0x10000 ) ;; 0x18 = kernel32 code seg dis.


[BITS 32]
PROTECTED_MODE:

	; reset segment selector

	mov	ax,	0x20	; 0x20 = kernel32 data segment selector
	mov ds,	ax
	mov es,	ax
	mov	fs,	ax
	mov	gs,	ax

	; create stack
	mov	ss,	ax
	mov	esp, 0xFFFE
	mov ebp, 0xFFFE

	push (MESSAGE_ENTER_PROTECTED_MODE - $$ + 0x10000)
	push 5
	push 0
	call PRINT_MESSAGE_32
	add esp, 12

	jmp dword 0x18: 0x10200

;
; Function Area
;
PRINT_MESSAGE_32:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push eax
	push ecx
	push edx

	; calculating y
	mov eax, dword [ebp+12] ; ebp+12 = y
	mov esi, 160
	mul esi
	mov edi, eax

	; calculating x
	mov eax, dword [ebp+8] ; ebp + 8 = x
	mov esi, 2
	mul esi
	add edi, eax

	; obtaining string to print
	mov esi, dword[ebp+16]

LOOP_PRINT:
	mov cl, byte[esi]
	cmp cl, 0
	je PRINT_MESSAGE_32_END

	mov byte [edi + 0xB8000], cl
	add esi, 1
	add edi, 2

	jmp LOOP_PRINT

PRINT_MESSAGE_32_END:
	pop edx
	pop ecx
	pop eax
	pop edi
	pop esi
	pop ebp
	ret

;
;	Data Area
;

	align 8, db 0	; 8 bytes alignment
	dw 0x0000

GDTR:
	dw (GDT_END - GDT_START - 1)	; 16bit GDT size
	dd (GDT_START - $$ + 0x10000)	; 32bit GDT address
	
GDT_START:
	DESCRIPTOR_NULL:
		dw	0x0000	; limit
		dw	0x0000	; base
		db	0x00	; base
		db	00000000b	; P / DPL / S / Type
		db	00000000b	; G / D or B / L / AVL / limit
		db	0x00	; base
	
	DESCRIPTOR_KERNEL64_CODE:
		dw	0xFFFF	; limit
		dw	0x0000	; base
		db	0x00	; base
		db	10011010b	; P / DPL / S / TYPE
		db	10101111b	; G / D or B / L / AVL / limit
		db	0x00	; base

	DESCRIPTOR_KERNEL64_DATA:
		dw	0xFFFF	; limit
		dw	0x0000	; base
		db	0x00	; base
		db	10010010b	; P / DPL / S / TYPE
		db	10101111b	; G / D or B / L / AVL / limit
		db	0x00	; base

	DESCRIPTOR_KERNEL32_CODE:
		dw	0xFFFF	; limit
		dw	0x0000	; base
		db	0x00	; base
		db	10011010b	; P / DPL / S / TYPE
		db	11001111b	; G / D or B / L / AVL / limit
		db	0x00	; base
	
	DESCRIPTOR_KERNEL32_DATA:
		dw	0xFFFF	; limit
		dw	0x0000	; base
		db	0x00	; base
		db	10010010b	; P / DPL / S / TYPE
		db	11001111b	; G / D or B / L / AVL / limit
		db	0x00	; base
GDT_END:

MESSAGE_ENTER_PROTECTED_MODE:	db 'Entering Protected Mode......................[PASS]', 0

	times 512 - ( $ - $$ )	db 0x00


























	
	
