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

    TA0CCR0 = 16383;
    TA0CCR1 = 16383;
    TA0CCR2 = 8191;

    TA0CCTL1 = CCIE;
    TA0CCTL2 = CCIE;


    __enable_interrupt();
    while(1){}

}
#pragma vector = 44
__interrupt void isr(){
    switch(TA0IV){
    case 0x2: // CCR1
        P1OUT ^= BIT0;
        break;
    case 0x4: // CCR2
        P1OUT ^= BIT0;
        P1OUT ^= BIT1;
        break;
    }
}
