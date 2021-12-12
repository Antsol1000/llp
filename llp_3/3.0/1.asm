section .data

section .bss
        input resb 1024
	output resb 1024

section .text
	global _start
_start:
        call _input
        call _strcpy
        call _output

	mov rax, 60
	mov rdi, 0
	syscall

_strcpy:
	mov rcx, 1024
	mov rsi, input
	mov rdi, output
	rep movsb
	ret

_input:
	mov rax, 0
	mov rdi, 0
	mov rsi, input
	mov rdx, 1024
	syscall
	ret

_output:
        mov rax, 1
        mov rdi, 1
        mov rsi, output
        mov rdx, 1024
        syscall
        ret
