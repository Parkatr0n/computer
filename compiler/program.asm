
; program.asm

mov 1, b
mov 0, a

loop:
	add a, b
	dbg
	jmp loop

