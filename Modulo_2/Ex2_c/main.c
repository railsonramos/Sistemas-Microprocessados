#include <msp430.h> 




int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;
	//Definir Led
	P1DIR |= BIT0;
	P1OUT &= ~BIT0;
	
	// DEFINE O BOTÃO
	P5DIR &= ~BIT5;
	P5REN |= BIT5;
	P5OUT |= BIT5;

    while(1){
        if (!(P5IN & BIT5)){
            P1OUT ^= BIT0;
        }
    }


	return 0;
}


