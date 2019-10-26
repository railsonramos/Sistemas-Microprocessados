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
			mov		#0,R5
			mov		#1,R7
			clr		R6
			clr		R8
			call	#loop
			jmp		$

loop:
			add		R7,R5
			addc	R8,R6
			mov		R7,R10		; R10 rebe o valor anterior, antes de ocorrer o carry
			mov		R8,R11		; R11 rebe o valor anterior, antes de ocorrer o carry
			jc		fim			; se tiver carry (mais de 32bits) nessa itercao, ja para por aqui
			add		R5,R7
			addc	R6,R8
			mov		R5,R10		; R10 rebe o valor anterior, antes de ocorrer o carry
			mov		R6,R11		; R11 rebe o valor anterior, antes de ocorrer o carry
			jc		fim			; se tiver carry (mais de 32bits) nessa itercao, ja para por aqui
			jmp		loop

fim:
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
            
