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
			mov		@R5+, R10	; Passa para R10 o primeiro valor do vetor, isto é o tamanho do vetor
			mov		#0,R6		; primeiro valor para comparar com os do vetor
			clr		R7			; R7 recebe a frência com que certa palavra se repete
			call	#loop		; vai para a subrotina loop
			jmp		$			; fim da execucao

loop:
			mov		@R5+,R8		; R8 armazena palavra do vetor em little endian
			cmp		R6,R8		; compara R8 com R6
			jhs		maior		; R8>R6?
			jeq		igual		; R8=R6?
			jmp		fim			; Salta pro lebel fim

maior:
			mov		R8,R6		; atualiza o maior
			clr		R7			; Limpa R7 (frequencia do anterior)
			inc		R7			; Incrementa 1 ao R7 pois contabiliza o maior

igual:
			inc		R7			; Incrementa 1 caso haja plavras (2 bytes) repetidas

fim:
			dec		R10			; Decremnta o tamanho do vetor
			jnz 	loop		; Se não varreu todo o vetor ainda, volta pro loop
			ret					; retorna pra main (instrução seguinte a subrotina loop)

			.data
vetor:		.byte	7,0, "RAILSONTHIAGO",0 ; agora estamos pegando de 2 em 2 bytes, logo o tamanho
;do vetor deve ser declarado em 2 bytes (0x0008), como o armazenamento é feito em little-endian
; a concatenação dos bytes 08 e 00 será 0008.
                                            

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
            
