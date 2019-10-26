#include <msp430.h> 
#include "libclock/clock.h"


void configpins();


void main()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    clockInit();

    TA4CCR0 = 655;
    TA4CCR1 = 655/2;

    TA4CTL = TASSEL__ACLK | MC__UP | TACLR;
    TA4CCTL1 = OUTMOD_7;
    P5DIR |= BIT7;
    P5SEL1 |= BIT7;
    P5SEL0 &= ~BIT7;

    configpins();
    while(1){}

}
void configpins(){

    // DEFINIR LED
    P1DIR |= (BIT0);
    P1OUT &= ~(BIT0);
}

