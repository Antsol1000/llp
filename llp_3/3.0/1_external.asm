bits    64
default rel

global  main

extern printf
extern scanf

section .bss
	input	resb 1024
	output	resb 1024
	
section .data
	format_string_in    db '%s', 0
	format_string_out    db '%s', 10
	
section .text
	main:
        sub rsp, 8
        
        ;INPUT READING
        mov al, 0
        mov rsi, input
        lea rdi, [format_string_in]
        call    scanf wrt ..plt
        
        ;GETTING LENGTH
	mov rbx, 0		;length will be stored in rbx
	mov rsi, input
	loop:
	mov al, [rsi]		; if end of string - last sign is a null
	cmp rax, 0
	je copy
	inc rbx
	inc rsi
	jmp loop
	
	copy:
	mov rsi, input
  	mov rdi, output
  	mov rcx, rbx
  	rep movsb		; length was needed in order to perform the copying a fixed number of times
  	
        ;display
        mov al,0
        mov rsi, output
        lea rdi, [format_string_out]
        call    printf wrt ..plt
        
        add     rsp, 8
        sub     rax, rax
        ret
