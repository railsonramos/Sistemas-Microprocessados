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
DELAY1 		.equ	 11199
DELAY2 		.equ	 34


			mov		#-1,R10
			call	#confpin
			jmp		$

sw:
			bit.b	#BIT5,&P5IN
			jc		sw
			jmp		delay
contador:
			inc		R10
			cmp		#0,R10
			jeq		conte0
			cmp		#1,R10
			jeq		conte1
			cmp		#2,R10
			jeq		conte2
			cmp		#3,R10
			jeq		conte3
conte0:
			bic.b	#BIT1,&P1OUT
			bic.b	#BIT0,&P1OUT
			jmp		sw
conte1:
			bis.b	#BIT1,&P1OUT
			jmp		sw
conte2:
			bic.b	#BIT1,&P1OUT
			bis.b	#BIT0,&P1OUT
			jmp		sw
conte3:
			bis.b	#BIT1,&P1OUT
			mov		#-1,R10
			jmp		sw

confpin:
			; DEFINE LEDS
			bic.b	#LOCKLPM5,&PM5CTL0	; destrava os pinos do MSP430
			bis.b	#BIT0,&P1DIR
			bis.b	#BIT1,&P1DIR
			bic.b	#BIT0,&P1OUT
			bic.b	#BIT1,&P1OUT
            ; DEFINE BOTOES
            bic.b	#BIT5,&P5DIR
            bis.b	#BIT5,&P5REN
            bis.b	#BIT5,&P5OUT
            JMP		sw


delay:	 	MOV		 #DELAY1,R6
RT1: 		MOV		 #DELAY2,R5
RT2: 		DEC		 R5
			JNZ		RT2
			DEC		R6
			JNZ		RT1
			jmp		contador
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
            
