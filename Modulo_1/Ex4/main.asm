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
			mov		#vetor,R5
			mov		@R5+,R10	; tamanho do vetor
			mov		@R5,R7		; maior elemento
			mov		@R5,R6		; menor elemento
			call	#loop
			jmp		$

loop:
			mov		@R5+,R4		; valor para comparar
			cmp		R7,R4		; compara e atualiza a flag
			jge		maior		; R4>=R9
			cmp		R6,R4		; compara e atualiza a flag
			jl		menor		; R4<R8
			jmp		fim

maior:
			mov		R4,R7
			jmp		fim

menor:
			mov		R4,R6

fim:
			dec		R10
			jnz		loop
			ret

			.data
vetor:		.word	8, 121, 234, 567, -1990, 117, 867, 45, -1980
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
            
