#include <msp430.h> 


/**
 * main.c
 */

void debounce();

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
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

     while(1){
F
         if (!(P5IN & BIT6)){
             debounce();
             if(!(P5IN & BIT5)){
                 while(!(P5IN & BIT6) || (!(P5IN & BIT5))){}
                 P1OUT &= ~(BIT0|BIT1);
                 debounce();
             }
             else{
                 while(!(P5IN & BIT6)){}
                 if (P5IN & BIT5){
                     P1OUT ^= BIT0;
                     P1OUT ^= BIT1;
                     debounce();
                 }
             }
          }
          else if (!(P5IN & BIT5)){
              debounce();
              if(!(P5IN & BIT6)){
                  while(!(P5IN & BIT6) || (!(P5IN & BIT5))){}
                  P1OUT &= ~(BIT0|BIT1);
                  debounce();
              }
              else{
                  while(!(P5IN & BIT5)){}
                  if (P5IN & BIT6){
                      P1OUT ^= BIT1;
                      debounce();

                  }
              }
          }
     }

	return 0;
}
void debounce(){

    volatile int i=50000;
    while(i--){}
    return;
}
