bits    64
default rel
global  main
extern  scanf
extern printf

section .data
    format_int     db '%d', 0
    format_dis     db '%d ', 0
    format_r12     db 'r12: %d', 10, 0
    format_equ     db 'rbx: %d, r15: %d', 10, 0

section .bss
    array_int       resd 100

section .text
    main:
        sub rsp, 8
        
	;READING INTS
	;szczerze mówiąc nie mam pojęcia czm ale nie można tutaj zrobić calla jak w _display_array
	
	mov rbx, array_int
	mov r12,0
	
	while1:
	
	lea rsi, [rbx]			;reading part
	lea rdi, [format_int] 
	mov al, 0 
	call scanf wrt ..plt
	
	add rbx, 4
	inc r12	; r12 is the increm. bc ecx gets trashed by the calls
	cmp r12, 100	;there can not be any more than 100 inigers
	jge endwhile1	;if there is already 100 - break from the while loop
	cmp rax, 0	; output from scanf operation stored in rax so if not succesfull - break
	jne while1
	endwhile1:
	
	
	sub r12,2	;needs to be decremented bc it is one too big
	
	
	
	;*****THA MEAT = BUBBLESORT*****
	mov r13, 0 	; i 
	mov rax, 0
	loop1:
	mov r14, r12	; j
	loop2:
	
	 mov r15, array_int
	 add r15, r14
	 add r15, r14
	 add r15, r14
	 add r15, r14
	 
	 mov rbx, r15
	 sub rbx, 4
	 
	 mov edx, [r15]
	 mov esi, [rbx]
	 cmp esi, edx
	 jg greater
	 jmp end
	 
	 greater:
	 call _swap
	 
	 end:
	
	dec r14
	cmp r14, r13
	jg loop2
	inc r13
	mov rax, r12
	dec rax
	cmp r13, rax
	jle loop1

	inc r12
				;previously it was decreased one time too much for the sake of handyness
	;DISPLAYING THE ARRAY
	call _display_array
	
	add rsp, 8
	sub rax, rax
	ret


_display_array:

	mov rbx, array_int
	
	while3:
	mov rsi, [rbx]			
	lea rdi, [format_dis] 
	mov al, 0 
	call printf wrt ..plt
	add rbx, 4
	dec r12		;this is the number of times we have to perform the loop
	cmp r12, 0		; if incrmtor is 0 - the 
	jg while3
	ret
	
_swap:
	;swapping - needex rbx(greater)(first one) - r15(lower) (second), rax as helper
	mov eax, [r15]
	push rax
	mov eax, [rbx]
	mov dword [r15], eax
	pop rax
	mov dword [rbx], eax
	ret
	
