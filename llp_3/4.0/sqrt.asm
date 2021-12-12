bits    64
default rel
global  main

extern  scanf
extern printf

section .data
    format      db 'Check!', 0xA, 0
    format_output    db 'sqrt(%lf) = %lf',0xA, 0
    format_read    db '%lf', 0
    align 16
    start dq 0.0
    var dq 0.125

section .bss
    end   resq 1

section .text
    main:
        sub     rsp, 8
        
        ;READING END
        lea     rsi, [end]    
        lea     rdi, [format_read]        
        mov     al, 0                      
        call    scanf wrt ..plt
        
        ;xmm2 will be the register storing the current value, xmm3 the current square root
        movlpd xmm4, [start]
        while:
        sqrtsd	xmm3, xmm4
        
        movsd	xmm1, xmm3			; arguments to print...
        movsd	xmm0, xmm4 			; moves value lower float from xmm3 to xmm0
        lea     rdi, [format_output]        
        mov     al, 2                     
        call    printf wrt ..plt
        addsd	xmm4, [var]			; next step
        
        movsd	xmm0, xmm4 
        cmpsd xmm0, [end], 1
	movq rax, xmm0				;result of cmpsd is stored in the destination - moving it to rax
	cmp al, 0xff
        je	while
	
        end_program:
        add     rsp, 8
        sub     rax, rax
        ret
     

