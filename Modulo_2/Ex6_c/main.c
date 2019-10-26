#include <msp430.h> 

void debounce();

int main(void)
{
        WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
        PM5CTL0 &= ~LOCKLPM5;

        //Definir Led
        P1DIR |= BIT0;              //Led Vermelho
        P1OUT &= ~BIT0;

        //Definir Led
        P1DIR |= BIT1;              //Led Verde
        P1OUT &= ~BIT1;

        // DEFINE O BOTÃO
        P5DIR &= ~BIT5;
        P5REN |= BIT5;
        P5OUT |= BIT5;

        // DEFINE O BOTÃO
         P5DIR &= ~BIT6;
         P5REN |= BIT6;
         P5OUT |= BIT6;

         int estado = 0;

         while(1){
             switch (estado){
             case 0:
                 if((P5IN & BIT6) && !(P5IN & BIT5)){
                     debounce();
                     while(!(P5IN & BIT5)){}
                     estado=1;
                     P1OUT |= BIT1;
                 }
                 break;

             case 1:
                 if((P5IN & BIT6) && !(P5IN & BIT5)){
                     debounce();
                     while(!(P5IN & BIT5)){}
                     estado=2;
                     P1OUT |= BIT0;
                     P1OUT &= ~BIT1;
                 }
                 else if(!(P5IN & BIT6) && (P5IN & BIT5)){
                     debounce();
                     while(!(P5IN & BIT6)){}
                     estado=0;
                     P1OUT &= ~BIT1;
                 }
                 break;

             case 2:
                 if((P5IN & BIT6) && !(P5IN & BIT5)){
                     debounce();
                     while(!(P5IN & BIT5)){}
                     estado=3;
                     P1OUT |= BIT1;
                 }
                 else if(!(P5IN & BIT6) && (P5IN & BIT5)){
                     debounce();
                     while(!(P5IN & BIT6)){}
                     estado=1;
                     P1OUT &= ~BIT0;
                     P1OUT |= BIT1;
                 }
                 break;

             case 3:
                 if(!(P5IN & BIT6) && (P5IN & BIT5)){
                     debounce();
                     while(!(P5IN & BIT6)){}
                     estado=2;
                     P1OUT &= ~BIT1;
                 }
                 break;
             }
         }

	
	return 0;
}

void debounce(){

    volatile int i=50000;
    while(i--){}
    return;
}
