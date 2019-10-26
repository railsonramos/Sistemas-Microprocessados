#include <msp430.h> 
#include "libclock/clock.h"

#define passo 65 //passo do pwm
#define max 655 // duty cycle
#define min 0

void debounce();

void main()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    clockInit();

    // DEFINIR LED
    P1DIR |= (BIT0);
    P1OUT &= ~(BIT0);

    // DEFINE O BOTÃO
    P5DIR &= ~BIT5;
    P5REN |= BIT5;
    P5OUT |= BIT5;

    // DEFINE O BOTÃO
    P5DIR &= ~BIT6;
    P5REN |= BIT6;
    P5OUT |= BIT6;

    // DEFINE O TIMER
    TA4CTL = TASSEL__ACLK|MC__UP|TACLR;

    TA4CCTL1 = OUTMOD_7;
    P5DIR |= BIT7;
    P5SEL1 |= BIT7;
    P5SEL0 &= ~BIT7;

    TA4CCR0 = max;
    TA4CCR1 = min;

    while(1){
        if (!(P5IN & BIT5)){
            debounce();
          if (TA4CCR1  < max){
              TA4CCR1 += passo;
          }
      }
        if (!(P5IN & BIT6)){
            debounce();
            if (TA4CCR1 > min){
             TA4CCR1 -= passo;
            }
        }
    }
}
void debounce(){

    volatile int i=50000;
    while(i--){}
    return;
}
