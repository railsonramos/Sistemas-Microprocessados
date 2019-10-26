//Projeto Final do Laboratório de Sistemas Microprocessados

//Alunos: Thiago Caldas dos Santos - 170126528
//        Railson Ramos de Matos - 170154769

//Informações sobre os pinos do MSP utilizados

//LCD

   //SCL-> P7.1
   //SDA -> P7.0
   //VCC -> 5V
   //GND -> GND

//SERVO

   //Pino do PWM -> P3.5
   //VCC -> 5V
   //GND -> GND

//SENSOR DE COR TCS230

   //VCC -> 3.3 V    //S2 -> P3.2
   //GND -> GND      //S3 -> P3.3
   //S0 -> P3.0      //OUT -> P1.2
   //S1 -> P3.1      //OE -> GND

#include <msp430.h>
#include "libclock/clock.h"
#include <stdint.h>
#include <LCD_I2C.h>

// DEFINES TCS230
#define f_clk 1000000   // clock SMCLK

#define S0 BIT0
#define S1 BIT1
#define S2 BIT2
#define S3 BIT3
#define out BIT2

uint16_t pulseIn();
uint16_t cor();
void moveServo(uint16_t cor_recebida);
void mostraLCD(uint16_t vermelho ,uint16_t verde, uint16_t azul);
void inicializaLCD();
void debounce();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    clockInit();

    I2C_config_B2(1,0x12,10);
    Config_LCD();

    __enable_interrupt();

   volatile  uint16_t nVermelho=0,nVerde=0,nAzul=0;
   volatile uint16_t cor_do_objeto;


    // PINO DO PWM
    P3SEL1 &= ~BIT5;
    P3SEL0 |= BIT5;
    P3DIR |= BIT5;

    // DEFINIR BOTÕES
    P5DIR &=~(BIT5|BIT6);
    P5OUT |= (BIT5|BIT6);
    P5REN |= (BIT5|BIT6);

    //TIMER DO SERVO

    TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR;
    TB0CCTL4 = OUTMOD_3;
    TB0CCR0 = 3000;
    TB0CCR4 = 1000;

    //CONFIGURAÇÕES PARA A CAPTURA DO SENSOR DE COR

    TA1CTL = TASSEL__SMCLK|MC__CONTINOUS|TACLR;
    TA1CCTL1 = CM__RISING | CCIS__CCIA | SCS | CAP;  // P1.2 -> A1.1 // pino de capitura

    P1DIR &= ~out;  // dado de entrada
    P1SEL0 |= out;  // função captura
    P1SEL1 &= ~out;

    P3DIR |= S0|S1|S2|S3; // saida do msp430
    P3OUT &= ~S1;        // 2% da freq original // descrita no datasheet
    P3OUT |= S0;

    inicializaLCD();

    while(1){
        mostraLCD(nVermelho,nVerde,nAzul);
        if (!(P5IN & BIT5)){        //espera pressionar o botão para realizar a captura da cor
            debounce();
            while(!(P5IN & BIT5));
            cor_do_objeto = cor();
            moveServo(cor_do_objeto);
            if (cor_do_objeto == 3) nVermelho++;
            else if (cor_do_objeto == 2) nVerde++;
            else if (cor_do_objeto == 1) nAzul++;
        }
    }
}



uint16_t cor(){

   volatile uint16_t freqvm, freqvd, freqa; // frequência das cores vermelho, verde e azul

    P3OUT &= ~(S2|S3); // vermelho
    debounce();
    freqvm = pulseIn();

    P3OUT |= (S2|S3); // verde
    debounce();
    freqvd = pulseIn();

    P3OUT &= ~S2;  // azul
    P3OUT |= S3;
    debounce();
    freqa = pulseIn();

    if (freqvm > freqvd && freqvm > freqa) return 3;          //Vermelho
    else if (freqvd > freqvm && freqvd > freqa) return 2;     //Verde
    else if (freqa > freqvm && freqa > freqvd) return 1;      //Azul
    else  return 0;
 }

uint16_t pulseIn(){

    volatile uint16_t freq;
    volatile uint16_t t1,t2,dt = 0,i;

    TA1CTL |= TACLR;  // limpa o timer

    for(i=0;i<16;i++){         //faz 16 medidas

        TA1CCTL1 &= ~CCIFG;  // garante que a flag esteja resetada

        while(!(TA1CCTL1 & CCIFG));  // espera a captura
        TA1CCTL1 &= ~CCIFG;

        t1 = TA1CCR1; // valor capturado no 1º flanco

        TA1CCTL1 &= ~CCIFG;  // garante que a flag esteja resetada

        while(!(TA1CCTL1 & CCIFG));  // espera a captura
        TA1CCTL1 &= ~CCIFG;

        t2 = TA1CCR1; // valor capturado no 2º flanco

        dt += (t2-t1);  //período da onda capturada
    }

    dt >>= 4;           //divide por 16 para fazer a média

    freq = (f_clk/dt); // frequência da onda capturada
    TA1CTL |= TACLR;  // limpa o timer

    return freq;
}

void moveServo(uint16_t cor_recebida){

    //P3.5 -> Pino do PWM

    //2ms -> 0
    //1,5ms -> -45 (direciona para a direita)
    //2,5ms-> 45 (direciona para a esquerda)

    if (cor_recebida==2 || cor_recebida==1){  //verde e azul
        TB0CCR4 = 1500;
    }

    else if (cor_recebida==3){  //vermelho
        TB0CCR4 = 500;
    }
    else TB0CCR4 = 1000;
}

void inicializaLCD(){

    LcdPosicao(0, 0);
    LcdDisplay("Certos:V=00 A=00");
    LcdPosicao(0, 1);
    LcdDisplay("Defeituosos:00");

}
void mostraLCD(uint16_t vermelho ,uint16_t verde, uint16_t azul){

    //P7.1 -> SCL
    //P7.0 -> SDA

    uint16_t vermD,vermU,verdD,verdU,azulD,azulU;

    vermD = vermelho/10 + 0x30;
    vermU = vermelho%10 + 0x30;

    verdD = verde/10 + 0x30;
    verdU = verde%10 + 0x30;

    azulD = azul/10 + 0x30;
    azulU = azul%10 + 0x30;

    LcdPosicao(9, 0);
    LcdWriteByte(0x27, verdD, 1);
    LcdPosicao(10, 0);
    LcdWriteByte(0x27, verdU, 1);

    LcdPosicao(14, 0);
    LcdWriteByte(0x27, azulD, 1);
    LcdPosicao(15, 0);
    LcdWriteByte(0x27, azulU, 1);

    LcdPosicao(12, 1);
    LcdWriteByte(0x27, vermD, 1);
    LcdPosicao(13,1 );
    LcdWriteByte(0x27, vermU, 1);
}

void debounce(){

    volatile int i=500000;
    while(i--);
    return;
}
