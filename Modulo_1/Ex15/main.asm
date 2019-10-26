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

DELAY1		.equ	400
DELAY2		.equ	333

config:		bic.b	#LOCKLPM5,&PM5CTL0
			bis.b	#(BIT0|BIT1),&P1DIR		;Configurando os leds
			bic.b	#(BIT0|BIT1),&P1REN
			bic.b	#(BIT0|BIT1),&P1OUT

			bic.b	#(BIT5|BIT6),&P5DIR			;Configurando os botões
			bis.b	#(BIT5|BIT6),&P5REN
			bis.b	#(BIT5|BIT6),&P5OUT

			call	#teste1


teste1:		bit.b	#BIT6,&P5IN
			jnc		inverteLed0

teste2:		bit.b	#BIT5,&P5IN
			jnc		inverteLed1
			jmp		teste1

inverteLed0:	call	#rot_atz
				bit.b	#BIT6,&P5IN
				jnc		pressionado1
				xor.b	#BIT0,&P1OUT
				jmp		teste2

inverteLed1:	call	#rot_atz
				bit.b	#BIT5,&P5IN
				jnc		pressionado2
				xor.b	#BIT1,&P1OUT
				jmp		teste1

pressionado1:	bit.b	#BIT5,&P5IN
				jnc		guardaEstado1
				jmp		teste2

pressionado2:	bit.b	#BIT6,&P5IN
				jnc		guardaEstado1
				jmp		teste1

guardaEstado1:	bit.b	#(BIT0|BIT1),&P1OUT
				jc		estado3						;Led 0 ligado e led 1 ligado
				bit.b	#BIT0,&P1OUT				;Led 0 desligado
				jnc		guardaEstado2
				jmp 	estado2						;Led 0 ligado e led 1 desligado

guardaEstado2:	bit.b	#BIT1,&P1OUT
				jc		estado1						;Led 0 desligado e led 1 ligado

estado0:	mov		#0,R10
			jmp 	inicioPisca

estado1:	mov		#1,R10
			jmp 	pisca

estado2:	mov		#2,R10
			jmp 	pisca

estado3:	mov		#3,R10

inicioPisca:	bis.b	#BIT0,&P1OUT
				bic.b	#BIT1,&P1OUT

pisca:			bit.b	#(BIT5|BIT6),&P5IN
				jc		decideEstado
				xor.b	#BIT0,&P1OUT
				xor.b	#BIT1,&P1OUT
				call	#rot_atz
				jmp 	pisca


decideEstado:	cmp		#0,R10
				jeq		setaEstado0
				cmp		#1,R10
				jeq		setaEstado1
				cmp		#2,R10
				jeq		setaEstado2
				cmp		#3,R10
				jeq		setaEstado3

setaEstado0:	bic.b	#(BIT0|BIT1),&P1OUT
				call	#rot_atz
				jmp 	teste1

setaEstado1:	bic.b	#BIT0,&P1OUT
				bis.b	#BIT1,&P1OUT
				call	#rot_atz
				jmp 	teste1

setaEstado2:	bis.b	#BIT0,&P1OUT
				bic.b	#BIT1,&P1OUT
				call	#rot_atz
				jmp 	teste1

setaEstado3:	bis.b	#(BIT0|BIT1),&P1OUT
				call	#rot_atz
				jmp 	teste1


rot_atz:	mov		#DELAY1,R6

RT1:		mov		#DELAY2,R5

RT2:		dec		R5
			jnz		RT2
			dec		R6
			jnz		RT1
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
            
