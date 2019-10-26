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

matr		.set	35243

main:
			mov		#0x1C00,R5
			mov		#4,R12		; numero de elementos
			mov		#12,R9		; Tamanho da rotacao atual
			call	#tamRota
			jmp		$
tamRota:
			mov		#matr,R6	; vetor guardado em R6
			mov		R9,R10		; R10 tamanho da rotacao
			cmp		#0,R10		; ainda tem que rotacionar
			jeq		mascara		; esse caso e apenas para o ultimo valor do vetor, pois ao inves de ir p rotacao ele ja ira passar a mascara

rotacao:
			RRA		R6			; totacao aritimetica do vetor
			dec		R10			; decrementa tamanho do vetor
			jnz		rotacao		; R10=0 acabou

atuRota:
			sub		#4, R9		; diminui o tamanho da rotacao em um nibble
mascara:
			and		#0x000F,R6	; mantem apenas o ultimo caracter
			cmp		#0x000A,R6	; o caracter e letra ou numero
			jlo		numeros

letras:
			add		#37h,R6
			jmp		fim

numeros:
			add		#30h,R6

fim:
			mov		R6,0(R5)
			incd	R5
			dec		R12
			jnz		tamRota
			ret


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
            
