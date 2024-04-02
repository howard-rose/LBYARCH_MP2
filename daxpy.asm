section .text
bits 64
default rel

global daxpy_asm
;params: 
;n: ecx
;A: xmm1
;X: r8
;Y: r9
;Z: rbp+32
daxpy_asm:
	push rsi
	push rbp
	mov rbp, rsp
	add rbp, 16
	add rbp, 8
	
	xor rsi, rsi ;array position counter
	mov r10, [rbp+32] ;address of Z
L1:
	movsd xmm0, xmm1
	mulsd xmm0, [r8+rsi*8]
	addsd xmm0, [r9+rsi*8]
	movsd [r10+rsi*8], xmm0

	inc rsi
	cmp rsi, rcx
	jb L1

	pop rbp
	pop rsi
	ret