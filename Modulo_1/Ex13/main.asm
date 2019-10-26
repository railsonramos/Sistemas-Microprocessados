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


;	P1.0 - LED VERMELHO - BOTAO P5.6
;	P1.1 - LED VERDE - BOTAO P5.5
DELAY1 		.equ	 1234
DELAY2 		.equ	 34


main:
			; DEFINE LEDS
			bic.b	#LOCKLPM5,&PM5CTL0	; destrava os pinos do MSP430
			bis.b	#BIT0,&P1DIR
			bis.b	#BIT1,&P1DIR
			bic.b	#BIT0,&P1OUT
			bic.b	#BIT1,&P1OUT
            ; DEFINE BOTOES
            bic.b	#BIT6,&P5DIR
            bic.b	#BIT5,&P5DIR
            bis.b	#BIT6,&P5REN
            bis.b	#BIT5,&P5REN
            bis.b	#BIT6,&P5OUT
            bis.b	#BIT5,&P5OUT
            call	#loop1
            jmp		$


loop1:
			bit.b	#BIT6,&P5IN	;
			jz		acende1	; Botao precionado?
			bic.b	#BIT0,&P1OUT
			jmp		loop2
acende1:
			bis.b	#BIT0,&P1OUT
loop2:
			bit.b	#BIT5,&P5IN	; como é pull-up ativo = 1, portanto ativo seta o carry
			jz		acende2
			bic.b	#BIT1,&P1OUT
			jmp		loop1
acende2:
			bis.b	#BIT1,&P1OUT
			jmp		loop1


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
            
