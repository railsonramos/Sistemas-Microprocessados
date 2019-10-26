#include <msp430.h> 
#include "libclock/clock.h"


void configpins();
void debounce();

void main()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    clockInit();
    configpins();

    TA0CTL = TASSEL__ACLK|MC__UP|TACLR;

    TA0CCR0 = 655;
    TA0CCR1 = 655;
    TA0CCR2 = 0;

    TA0CCTL1 = CCIE;
    TA0CCTL2 = CCIE;


    __enable_interrupt();
    while(1){
        if(!(P5IN & BIT6) && TA0CCR2>0){
            debounce();
            while(!(P5IN & BIT6));
            TA0CCR2 -= 65;
        }
        if(!(P5IN & BIT5) && TA0CCR2<655){
            debounce();
            while(!(P5IN & BIT5));
            TA0CCR2 += 65;
        }

    }

}

#pragma vector = 44;
__interrupt void isr(){
    switch(TA0IV){
    case 0x2: // CCR1
        P1OUT |= BIT0;
        break;
    case 0x4: // CCR2
        P1OUT &= ~BIT0;
        break;
    }
}

void configpins(){

    // DEFINIR LED
    P1DIR |= (BIT0);
    P1OUT &= ~(BIT0);

    //DEFINIR BOTÕES
    P5DIR &= ~(BIT5|BIT6);
    P5REN |= (BIT5|BIT6);
    P5DIR |= (BIT5|BIT6);
}
void debounce(){

    volatile int i=5000;
    while(i--){}
    return;
}

