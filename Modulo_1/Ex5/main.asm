;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------

main:
			mov		#vetor1,R5	; inicio do vetor1
			mov		@R5+, R10 ; tamanho do vetor
			mov		#vetor2,R6	; inicio do vetro2 (automaticamente o vetor já icia em 1C10, pois depois de todos os elementos do vetor anteriores inicia o segundo vetor)
			incd	R6
			mov		#0x1C20,R7	; icicio de vetor Soma
			call	#loop
			jmp		$

loop:
			mov		@R5+,R9
			mov		@R6+,R8
			add		R9,R8
			mov		R8,0(R7)
			incd	R7
fim:
			dec		R10
			jnz		loop
			ret



			.data
vetor1:		.word 7, 2021, 2223, 2425, 2627, 2829, 3031, 3233
vetor2: 	.word 7, 6867, 6665, 6463, 6261, 6059, 5857, 5655
                                            

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
