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

DELAY1 		.equ	 19999
DELAY2 		.equ	 34

main:
			bic.b	#LOCKLPM5,&PM5CTL0	; destrava os pinos do MSP430
			bis.b	#BIT0,&P1DIR	; Define a direção do pino P1.1 (saida)
			bis.b	#BIT1,&P1DIR	; Define a direção do pino P1.1 (saida)
			bic.b	#BIT0,&P1OUT	;Inicia desligado
			bis.b	#BIT1,&P1OUT	;Inicia ligado

			call	#alterna
			jmp		$
alterna:
			xor		#(BIT0|BIT1),&P1OUT	; Alterna o valor dos pinos

ROT_ATZ: 	MOV		 #DELAY1,R6
RT1: 		MOV		 #DELAY2,R5
RT2: 		DEC		 R5
			JNZ		RT2
			DEC		R6
			JNZ		RT1
			jmp		alterna
			Ret
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
            
