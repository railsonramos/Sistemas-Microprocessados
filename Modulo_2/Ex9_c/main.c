#include <msp430.h> 
#include <stdint.h>

unsigned int delay();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog time
    PM5CTL0 &= ~LOCKLPM5;
    //Definir led verde
    P1DIR |= BIT1;              // define saida
    P1OUT &= ~BIT1;             // led inicia desligado

    while(1){
        P1OUT ^= BIT1;
        delay();
    }

    return 0;
}

// Funcao delay
unsigned int delay(){
    TA2CTL = TASSEL__ACLK|MC__UP|TACLR;                // vefine o modo de contagem
    TA2CCR0 = 32768;                                 // variavel de entrada � maximo que deve contar (CCRx)
    while(!(TA2CCTL0 & CCIFG));                         // trava de execu��o, enquanto a falg n�o for ativada
    TA2CTL = MC__STOP;                                  // modo de contagem fica em hold (contador n�o incrementa)
    TA2CCTL0 &=~ CCIFG;                                  //
    return 0;
}
