#include <msp430.h> 
#include "libclock/clock.h"
#include <stdint.h>

void debounce();

void main()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    clockInit();

    // DEFINIR LED VERMELHO
    P1DIR |= (BIT0);
    P1OUT &= ~(BIT0);

    // DEFINE LED VERDE
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

    TA1CTL = TASSEL__ACLK|ID_1|MC__STOP|TACLR; // ID_2 divide a frequencia do clock por 2

    uint16_t dt;

    while(1){

        while(!(P5IN & BIT5));
        TA1CTL |= MC__CONTINUOUS;

        while(!(P5IN & BIT6) && !(TA1CTL & TAIFG));
        // Para o timer e retorna o valor de contagem
        TA1CTL &= ~(MC__CONTINUOUS);
    }
          dt = TA1R;
          if (dt <= 16384)
          {
              P1OUT |= BIT0;
              P1OUT &= ~(BIT1);
          }
          else if ((dt > 16384 )&&( dt <= 49152 ))
          {
              P1OUT |= BIT0;
              P1OUT |= (BIT1);
          }
          else if (dt > 49152 )
          {
              P1OUT &= ~BIT0;
              P1OUT |= BIT1;
          }
}
void debounce(){

    volatile int i=50000;
    while(i--){}
    return;
}
