#include <msp430.h> 
#include "libclock/clock.h"

void main()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    clockInit();

    // DEFINIR LED
    P1DIR |= (BIT1|BIT0);
    P1OUT &= ~(BIT1|BIT0);


    TA0CTL = TASSEL__ACLK|MC__UP|TACLR;
    TA1CTL = TASSEL__ACLK|MC__UP|TACLR;

    TA0CCR0 = 8191;
    TA1CCR0 = 3276;

    TA0CCTL0 = CCIE;
    TA1CCTL0 = CCIE;



    __enable_interrupt();
    while(1){}

}
#pragma vector = 45;
__interrupt void isr(){
        P1OUT ^= BIT0;
}
#pragma vector = 41;
__interrupt void isr2(){
        P1OUT ^= BIT1;
}



