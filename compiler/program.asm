
; program.asm

mov 800, a
mov 1, b
mov 255, c

loop:
	add a, b
	mov c, $a
	jmp loop


