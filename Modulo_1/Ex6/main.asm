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
			mov		#vetor1,R5
			mov		#vetor2,R6
			mov		@R5+,R10		;tamanho do vetor
			incd	R6				; para desprezar o primeiro elemento que é o tamanho do vetor
			clr		R12				;MSW
			clr		R11				;LSW
			call	#loop
			jmp		$

loop:
			add		@R5+,R11		; Adiciona o  elemento do vetor1 em R11 (acumulado)
			addc	#0,R12			; Adiciona 0+dst+carry, se houver carry ira somar no R12
			add		@R6+, R11		; Adicona a R11 o elemento do vetor2
			addc	#0,R12			; Adiciona 0+dst+carry, se houver carry ira somar no R12
			dec		R10				; decrementa o tamanho do vetor
			jnz		loop			; continua o loop enquanto R10 != 0
			ret


			.data
vetor1: 	.word 7, 9021, 9223, 9425, 9627, 9829, 9031, 9233 ; exemplo que há carry
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
            
