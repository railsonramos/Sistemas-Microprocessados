#include <msp430.h>
#include <stdint.h>
#include <clock.h>
#include <LCD_I2C.h>


uint16_t a0 = 0, a1 = 0;
uint16_t valor1,dec1,hexa1;
uint8_t inteiro1,dec11,dec12,dec13 ,hex11,hex12,hex13;
uint16_t valor2,dec2,hexa2;
uint8_t inteiro2,dec21,dec22,dec23 ,hex21,hex22,hex23;
void mostraTensao(uint16_t V1, uint16_t V2);


void main() {

    WDTCTL = WDTPW | WDTHOLD;               // Trava o watchdog
    PM5CTL0 &= ~LOCKLPM5;                   // Destrava os pinos digitais

    clockInit();                            // Inicializa o sistema de clocks
                                            // ACLK @32768Hz, SMCLK @1MHz,
                                            // MCLK @16MHz

    I2C_config_B2(1,0x12,10);
    Config_LCD();

    //Timer para amostragem no display

    TA1CTL = MC__UP | TASSEL__ACLK | TACLR ;
    TA1CCR0 = 32768;

    /* Configuração */

    TA0CTL = TASSEL__ACLK | MC__UP | TACLR;         //Gera f=16Hz
    TA0CCR0 = 2048;
    TA0CCR1 = 1024;
    TA0CCTL1 = OUTMOD_6;



    ADC12CTL0 &= ~ADC12ENC;     // Zerar o enable para configurar a interface

    ADC12CTL0 = ADC12SHT0_1   |   // Gera o SAMP/CONV -> 8
                ADC12ON;        // Liga o ADC

    ADC12CTL1 = ADC12SSEL_1     | // Seleciona o clock ACLK
                ADC12SHP        | // Ativa o Sample-Timer
                ADC12SHS_1      | //TA0.1
                ADC12CONSEQ_3;  // Seleciona o modo de operacao -> Repeated - sequence

    ADC12CTL2 = ADC12RES__12BIT; // Modo 12 bits

    ADC12CTL3 = ADC12CSTARTADD_0; //Inicia na memória 0

    ADC12MCTL0 = ADC12INCH_12; // Seleciona o canal
    ADC12MCTL1 = ADC12INCH_13; // Seleciona o canal
    ADC12MCTL2 = ADC12INCH_12; // Seleciona o canal
    ADC12MCTL3 = ADC12INCH_13; // Seleciona o canal
    ADC12MCTL4 = ADC12INCH_12; // Seleciona o canal
    ADC12MCTL5 = ADC12INCH_13; // Seleciona o canal
    ADC12MCTL6 = ADC12INCH_12; // Seleciona o canal
    ADC12MCTL7 = ADC12INCH_13; // Seleciona o canal
    ADC12MCTL8 = ADC12INCH_12; // Seleciona o canal
    ADC12MCTL9 = ADC12INCH_13; // Seleciona o canal
    ADC12MCTL10 = ADC12INCH_12; // Seleciona o canal
    ADC12MCTL11 = ADC12INCH_13; // Seleciona o canal
    ADC12MCTL12 = ADC12INCH_12; // Seleciona o canal
    ADC12MCTL13 = ADC12INCH_13; // Seleciona o canal
    ADC12MCTL14 = ADC12INCH_12; // Seleciona o canal
    ADC12MCTL15 = ADC12INCH_13|ADC12EOS; // Seleciona o canal e ativa EOS


    P3SEL0 |= (BIT0|BIT1);
    P3SEL1 |= (BIT0|BIT1);

    ADC12CTL0 |= ADC12ENC;

    /* Loop infinito */
    while(1){
        while(!(ADC12IFGR0 & ADC12IFG15)); //Espera a última conversão
        a0 = (ADC12MEM0 + ADC12MEM2 + ADC12MEM4 + ADC12MEM6 + ADC12MEM8 + ADC12MEM10 + ADC12MEM12 + ADC12MEM14)/8;
        a1 = (ADC12MEM1 + ADC12MEM3 + ADC12MEM5 + ADC12MEM7 + ADC12MEM9 + ADC12MEM11 + ADC12MEM13 + ADC12MEM15)/8;
        while(!(TA1CCTL0 & CCIFG));  //Espera 1 seg
        mostraTensao(a0,a1);
    }
}


void mostraTensao(uint16_t V1, uint16_t V2){

    valor1 = 0.805*V1;    //Convertendo o valor convertido para tens�o, 0.805 = 3.3/4096 * 1000
    inteiro1 = valor1/1000 + 0x30;
    dec1 = valor1%1000;
    dec11 = dec1/100 + 0x30;
    dec12 = dec1%100;
    dec12 = dec12/10 + 0x30;
    dec13 = dec1%10 + 0x30;

    hexa1 = valor1%4096;

    hex11 = hexa1/256;
    if (hex11>9) hex11 +=0x37;
    else hex11 +=0x30;

    hex12 = hexa1%256;
    hex12 = hex12/16;
    if (hex12>9) hex12 +=0x37;
    else hex12 +=0x30;

    hex13 = hexa1%16;
    if (hex13>9) hex13 +=0x37;
    else hex13 +=0x30;

    valor2 = 0.805*V2;
    inteiro2 = valor2/1000 + 0x30;
    dec2 = valor2%1000;
    dec21 = dec2/100 + 0x30;
    dec22 = dec2%100;
    dec22 = dec22/10 + 0x30;
    dec23 = dec2%10 + 0x30;

    hexa2 = valor2%4096;

    hex21 = hexa2/256;
    if (hex21>9) hex21 +=0x37;
    else hex21 +=0x30;

    hex22 = hexa2%256;
    hex22 = hex22/16;
    if (hex22>9) hex22 +=0x37;
    else hex22 +=0x30;

    hex23 = hexa2%16;
    if (hex23>9) hex23 +=0x37;
    else hex23 +=0x30;

    LcdDisplay("A0: 0,000V  0HHH");
    LcdPosicao(0, 1);
    LcdDisplay("A1: 0,000V  0HHH");

    LcdPosicao(4, 0);
    LcdWriteByte(0x27, inteiro1, 1);
    LcdPosicao(6, 0);
    LcdWriteByte(0x27, dec11, 1);
    LcdWriteByte(0x27, dec12, 1);
    LcdWriteByte(0x27, dec13, 1);
    LcdPosicao(13, 0);
    LcdWriteByte(0x27, hex11, 1);
    LcdWriteByte(0x27, hex12, 1);
    LcdWriteByte(0x27, hex13, 1);

    LcdPosicao(4, 1);
    LcdWriteByte(0x27, inteiro2, 1);
    LcdPosicao(6, 1);
    LcdWriteByte(0x27, dec21, 1);
    LcdWriteByte(0x27, dec22, 1);
    LcdWriteByte(0x27, dec23, 1);
    LcdPosicao(13, 1);
    LcdWriteByte(0x27, hex21, 1);
    LcdWriteByte(0x27, hex22, 1);
    LcdWriteByte(0x27, hex23, 1);

}
