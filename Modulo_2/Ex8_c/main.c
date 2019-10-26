#include <msp430.h> 
#include <stdint.h>

unsigned int delay(uint16_t);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog time
    PM5CTL0 &= ~LOCKLPM5;
	//Definir led verde
	P1DIR |= BIT1;              // define saida
	P1OUT &= ~BIT1;             // led inicia desligado

	while(1){
	    P1OUT ^= BIT1;
	    delay(32768);
	}
	
	return 0;
}

// Funcao delay
unsigned int delay(uint16_t times_us){
    TA2CTL = TASSEL__ACLK|MC__UP|TACLR;                // vefine o modo de contagem
	TA2CCR0 = times_us;                                 // variavel de entrada é maximo que deve contar (CCRx)
	while(!(TA2CCTL0 & CCIFG));                         // trava de execução, enquanto a falg não for ativada
	TA2CTL = MC__STOP;                                  // modo de contagem fica em hold (contador não incrementa)
	TA2CCTL0 &=~ CCIFG;                                  //
	return times_us;
}
