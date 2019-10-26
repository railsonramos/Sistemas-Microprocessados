#include <msp430.h> 

void debounce();
unsigned char rand();

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

    unsigned char valor;
    int contador=0;

    valor = rand();
    while (1){
        if (!(P5IN & BIT6)){
            debounce();
            while(!(P5IN & BIT6)){}
            contador++;
        }


        if (!(P5IN & BIT5)){
            debounce();
            while(!(P5IN & BIT5)){}
            if (contador == valor){
                P1OUT |= BIT1;
                debounce();
            }
            else{
                P1OUT |= BIT0;
                debounce();
            }
            valor = rand();
            contador=0;
            debounce();
            debounce();
            P1OUT &= ~(BIT0|BIT1);
        }
    }
}


void debounce(){
    volatile int i=10000;
    while(i--){}
    return;
}

unsigned char rand(){
    static unsigned char num = 5;
    num = (num * 17) % 7;
    return num;
}
