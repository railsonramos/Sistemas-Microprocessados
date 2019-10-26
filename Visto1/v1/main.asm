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
;Alunos: Thiago Caldas dos Santos - 170126528
;		Railson Ramos de Matos	- 170154769

main:		mov			#vetor,R5
			mov.b		@R5+,R10
			mov.b		R10,R11				;salva o conte�do de R10 em R11 para n�o perder o valor
			clr			R12
			call		#ordena
			jmp			$
			nop

ordena:		mov.b		@R5+,R6
			mov.b		@R5,R7
			dec.b		R10
			cmp.b		#0,R10				;se ocorrer, chegou ao fim do vetor
			jeq			decideFim
			cmp.b		R6,R7
			jlo			troca
			jmp			ordena

troca:		dec			R5
			mov.b		R7,0(R5)
			inc			R5
			mov.b		R6,0(R5)
			mov			#1,R12				;se ocorrer troca R12 recebe 1
			jmp 		ordena

voltaInicio:	dec 	R5
				cmp		#0x1C01,R5
				jeq		ordena
				mov.b	R11,R10
				jmp		voltaInicio

decideFim:		cmp		#0,R12				;caso n�o tenha ocorrido troca R12 ser� igual a 0
				jeq		fim
				clr		R12
				jmp		voltaInicio

fim:			ret

			.data
vetor:		.byte	13,"MLKJIHGFEDCBA"
                                            

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
            
