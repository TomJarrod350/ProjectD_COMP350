;Tom OLeary
;Jarrod LeComte
;userlib.asm

	.global _syscall

;void syscall (int AX, int BX, int CX, int DX)
_syscall:
	push bp
	mov bp,sp
	mov ax,[bp+4]
	mov bx,[bp+6]
	mov cx,[bp+8]
	mov dx,[bp+10]
	int #0x21
	pop bp
	ret
