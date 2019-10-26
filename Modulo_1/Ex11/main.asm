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

			mov		#MEMO,R5	; Endereço do vetor (0x1C00)
			mov		#0x1C10,R6	; Local onde sera salvo o resultado final
			clr 	R11			; Recebe cada um dos valores do vetor (0x8000+0x0900+0x00A+0x000B)
			mov.b	#4,R8	; numero de elementos
			mov.b	#12,R7	; maior numero de rotacoes
			call	#ASC_W16 ;chamar subrotina
OK:
			jc 		OK ;travar execução com sucesso
NOK:
			jnc 	NOK ; travar execução com falha
ASC_W16:
			mov.b	@R5+,R9		; pega o elemento do vetor e guada em R9
			cmp		#41h,R9	; compara se e letra ou numero
			jlo		numero	; se R9<0x41, R9 e um numero
letra:
			sub.b	#37h, R9	; pega o valor correspondente da tabela ASCII 
			jmp		numrotacao
numero:
			sub.b	#30h,R9

numrotacao:					; condicoes de rotacao
			mov.b	R7,R10	; R10 = numero de rotacoes atual
			cmp		#0,R10 ; ja chegou em zero?
			jeq		resultante ; se ja chegou em zero vai pra resultante
rotacao:
			rla		R9	; shift left 
			dec		R10	; decrementa o numero de rotacoes a serem feitas
			jnz		rotacao	; ja chegou em zero
sub:
			sub.b	#4,R7		; subtrai um nibble a cada rotacao

resultante:
			add		R9,R11	
			mov		R11,0(R6)

final:
			dec		R8
			jnz		ASC_W16
			ret

			.data
; Declarar 4 caracteres ASCII (0x38, 0x39, 0x41, 0x42)
MEMO: 		.byte 	'8','9','A','B'

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
            
