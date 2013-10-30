[ORG 0x00]
[BITS 16]

SECTION .text

jmp 0x07C0:START

DATA_SECTOR_TOTAL_COUNT:
	dw	0x02
DATA_SECTOR_32_COUNT:
	dw	0x02

START:
; set CS, DS as 0x7C00
mov ax, 0x07C0
mov ds, ax

; set real mode stack segment
mov ax, 0x0000
mov ss, ax
; but, in my opinion, this stack can be overrided with bootloader code
mov sp, 0xFFFE
mov bp, 0xFFFE


INIT_SCREEN:
mov ax, 0xB800
mov es, ax
mov si, 0

LOOP_SCREENCLEAR:
	mov byte [es:si], 0			;
	mov byte [es:si+1], 0x07

	add si, 2
	cmp si, 80 * 25 * 2
	jl LOOP_SCREENCLEAR

END_SCREENCLEAR:

; show Helloworld message
push STRING_HELLOWORLD
push 0
push 0
call FUNCTION_PRINT
add sp, 6

; show image loading message
push STRING_IMAGE_LOADING
push 1
push 0
call FUNCTION_PRINT
add sp, 6


RESET_DISK:
	;BIOS 0x13 with ax = 0x00
	mov ax, 0x00
	mov dl, 0x00
	int 0x13

	; jc Handdling Error
	jc LOOP_HANDLE_DISK_ERROR

	; show message
	push STRING_RESET_COMPLETED
	push 2
	push 0
	call FUNCTION_PRINT
	add sp, 6

INIT_READ_DATA:
	mov si, 0x1000
	mov es, si
	mov bx, 0x0000
	mov di, word[DATA_SECTOR_TOTAL_COUNT]

LOOP_READ_DATA:

	cmp di, 0
	je END_READ_DATA    ; if di == 0, we already read all sectors
	sub di, 0x1

	; BIOS int 0x13
	mov ah, 0x02
	mov al, 0x1
	mov ch, byte [DATA_TRACK_CURRENT]
	mov cl, byte [DATA_SECTOR_CURRENT]
	mov dh, byte [DATA_HEAD_CURRENT]
	mov dl, 0x00
	int 0x13

	; jc Handling Error
	jc LOOP_HANDLE_DISK_ERROR

	add si, 0x0020
	mov es, si			; + 512 bytes

	; check current sector number
	mov al, byte [DATA_SECTOR_CURRENT]
	add al, 0x01
	mov byte [DATA_SECTOR_CURRENT], al
	cmp al, 19
	jl LOOP_READ_DATA

	; check current head number
	xor byte [DATA_HEAD_CURRENT], 0x01
	mov byte [DATA_SECTOR_CURRENT], 0x01
	cmp byte [DATA_HEAD_CURRENT], 0x00
	jne LOOP_READ_DATA

	; add current track number
	add byte[DATA_TRACK_CURRENT], 0x01
	jmp LOOP_READ_DATA

END_READ_DATA:
	push STRING_IMAGE_LOADED
	push 3
	push 0
	call FUNCTION_PRINT
	add sp, 6

	; execute loaded OS image
	jmp 0x1000:0000


LOOP_HANDLE_DISK_ERROR:
	push STRING_DISK_ERROR
	push 5
	push 20
	call FUNCTION_PRINT

	jmp $

FUNCTION_PRINT:
	; Paramter x, y, string
	push bp
	mov bp, sp

	push es
	push si
	push di
	push ax
	push cx
	push dx

	mov ax, 0xB800
	mov es, ax

	; get line-number by calculating y
	mov ax, word [bp+6] ; string > y > x > ret > bp
	mov si, 80 * 2
	mul si
	mov di, ax

	; get column-number by calculating x
	mov ax, word [bp+4]
	mov si, 2
	mul si
	add di, ax ; es:di == target address

	; get source address
	mov si, word [bp+8]


LOOP_MESSAGE:
	mov cl, byte [si]
	cmp cl, 0
	je END_MESSAGE

	mov byte[es:di], cl
	add si, 1
	add di, 2

	jmp LOOP_MESSAGE

END_MESSAGE:
	pop dx
	pop cx
	pop ax
	pop di
	pop si
	pop es

	pop bp
	ret

STRING_HELLOWORLD:
	db 'CLKH64 OS Boot Loader Starting...', 0
STRING_DISK_ERROR:
	db 'Disk Error occured!', 0
STRING_RESET_COMPLETED:
	db 'Reset completed.', 0

STRING_IMAGE_LOADING:
	db 'CLKH64 OS Image is Loading by Bootloader.', 0
STRING_IMAGE_LOADED:
	db 'Image load completed.', 0

DATA_SECTOR_CURRENT:
	db	0x02
DATA_HEAD_CURRENT:
	db	0x00
DATA_TRACK_CURRENT:
	db	0x00


times 510 - ( $ - $$ )	db 0x00

db 0x55
db 0xAA
