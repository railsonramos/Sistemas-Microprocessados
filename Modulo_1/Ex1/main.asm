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
			mov		#vetor,R5	; endereço do vetor em R5 (1C00)
			mov.b	@R5+,R10	; Passa para R10 o primeiro valor do vetor, isto é o tamanho do vetor
			mov.b	#0x00FF,R6	; maior valor em 8 bits, logo nenhuma letra a maior que essa
			call	#loop		; Pula para a subrotina "menor"
			jmp		$			; Fim da execucao

loop:
			mov.b	@R5+,R9		; R9 recebe primeira leitra do vetor, o R e já salta para a próxima
			cmp		R6,R9		; Compara a letra com a menor atual (dst compara com src)
			JLO		menor		; Se for menor vai pra label menor (R9<R6?)
			JEQ		igual		; Se for igual vai para label igual (R9=R6?)
			JMP		fim			; else fim

menor:
			CLR		R7			; Limpa R7
			INC		R7			; Incrementa 1, pois o menor atual tem que acumular com os proximos
			mov.b	R9,R6		; Atualiza o menor
			JMP 	fim			; salta para label fim
igual:
			inc		R7			; incrementa 1 caso a letra se repita

fim:
			dec		R10			; Decrementa o tamanho do vetro
			jnz		loop		; Enquanto não varrerr
			ret

			.data
vetor:		.byte	13,"RAILSONTHIAGO"



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
            
