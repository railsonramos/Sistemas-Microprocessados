/*
  PROBLEMA 2
  ALUNOS:
  RAILSON RAMOS DE MATOS - 170154769
  THIAGO CALDAS DO SANTOS - 170126528
  ----------------------------------
  MSP4305529
 */


#include <msp430.h> 
#include<stdint.h>
#include "libclock/clock.h"


/**
 * main.c
 */
void configpins(){
    P2DIR |= BIT0;               //pin trigger: P2.0
    P2OUT &= ~(BIT0);

    P1DIR &= ~(BIT2);            //pin echo: P1.2 >> TA0.1
    P1SEL |= BIT2;

    P1DIR |= (BIT0);            //led vermelho
    P1OUT &= ~(BIT0);

    P4DIR |= (BIT7);            //led verde
    P4OUT &= ~(BIT7);

    TA0CTL = TASSEL__SMCLK|MC__UP|TACLR;
    TA0CCR0 = 20000;                        //20ms,  tempo de cada aquisição de dados
    TA0CCR2 = 20;                           //20us, duração do trigger
    TA0CCTL1 = CAP|CM_3;         //CM__3 = both

}
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	configpins();
	clockInit();
	uint16_t t1,t2,dt;

	while(1)
	{
	    P2OUT |= BIT0;                  //Setando o trigger
	    TA0CTL |= TACLR;                //Zerando o contador
	    TA0CCTL2 &= ~CCIFG;             //Limpando a flag
	    while(!(TA0CCTL2 & CCIFG));     //Trava de execução, esperando-se atingir 20us
	    TA0CCTL2 &= ~CCIFG;             //Limpando a flag
	    P2OUT &= ~BIT0;                 //Resetando o trigger

	    while(!(TA0CCTL1 & CCIFG));     //Trava de execução para capturar o tempo em que há as bordas de subida e descida do echo
	    TA0CCTL1 &= ~CCIFG;             //Limpando a flag
	    t1 = TA0CCR1;                   //Armazenando o tempo da borda de subida do echo, em us por conta do clock ser de 1MHz

	    while(!(TA0CCTL1 & CCIFG));     //Trava de execução para capturar o tempo em que há as bordas de subida e descida do echo
	    TA0CCTL1 &= ~CCIFG;             //Limpando a flag
	    t2 = TA0CCR1;                   //Armazenando o tempo da borda de descida do echo, em us por conta do clock ser de 1MHz

	    dt = t2-t1;

	    if (dt <= 1176)          //O som leva 1176 us para percorrer 40cm (ida e volta), assim, como o clock é de 1MHz, o n deve ser igual a 1176
	    {
	        P1OUT |= BIT0;              //Liga Led Vermelho
	        P4OUT &= ~(BIT7);           //Desliga Led Verde
	    }
	    else if ((dt > 1176 )&&( dt <= 2353 )) //O som leva 2353 us para percorrer 80cm (ida e volta)
	    {
	        P1OUT &= ~BIT0;             //Desliga Led Vermelho
	        P4OUT |= (BIT7);            //Liga Led Verde
	    }
	    else if (dt > 2353 )
	            {
	                P1OUT |= BIT0;      //Liga Led Vermelho
	                P4OUT |= BIT7;      //Liga Led Verde
	            }

	}
	
	return 0;
}
