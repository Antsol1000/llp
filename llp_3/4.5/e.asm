bits    64
default rel
global  main

extern  scanf
extern printf

section .data
    format_output    db 'e^x = %lf',0xA, 0
    format_read    db '%d %lf', 0
    align 16
    var dq 1.0

section .bss
    k   resd 1
    x	resq 1

section .text
    main:
        sub     rsp, 8
        
        ;READING 
        lea     rdx, [x] 
        lea     rsi, [k]    
        lea     rdi, [format_read]        
        mov     al, 1                      
        call    scanf wrt ..plt
        
        ;xmm3 is going to be the numerator ,  xmm4 the denumerator, xmm5 the ourput
        movlpd xmm5, [var]
        movlpd xmm4, [var]
        movlpd xmm3, [var]
        ;tutaj wiem, ze dosyc mocno bez sensu, ale nie znalazlam innej opcji ://
        movlpd	xmm6, [var]		; xmm6 will be a qword counting the iterations and multiplying
      	mov	r12, 0			; rbx will be used to monitor looping action
      	
      	while:
      	
        movlpd xmm0, [x]
        mulsd	xmm3, xmm0		; mul of numerator
        mulsd	xmm4, xmm6		; mul of denominator
        movsd	xmm0, xmm3		; we will loose xmm3 bc of dividing it by xmm4
        divsd	xmm3, xmm4		; quotient - next to be added
        addsd	xmm5, xmm3
        movsd	xmm3, xmm0		; restoring xmm3
        
        addsd	xmm6, [var]		;increment iter
        add	r12, 1
        
        mov	rax, [k]
        cmp	r12, rax		;checking stopping condition
        jl	while
       
        end_program:
        movsd	xmm0, xmm5 
        lea     rdi, [format_output]        
        mov     al, 1                    
        call    printf wrt ..plt
        
        add     rsp, 8
        sub     rax, rax
        ret
