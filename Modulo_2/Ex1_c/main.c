#include <msp430.h> 
/**
 * main.c
 */



int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;

	P1DIR |= BIT0;      //Seta P1.1 como saida -- LED

	P5DIR &= ~BIT5;     //Limpa P5.5 como entrada -- Botão
	P5REN |= BIT5;      //Seta o enable, ativa resitor -- Botão
	P5OUT |= BIT5;     //pull-up -- Botão
	
	while (1){
	    if(P5IN & BIT5){
	        P1OUT &= ~BIT0;
	    }
	    else{
	        P1OUT |= BIT0;
	    }

	}
	return 0;
}
