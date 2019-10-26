#include <msp430.h>
#include "libclock/clock.h"
#include <stdint.h>

#define BT BIT3
#define EN BIT2
#define RW BIT1
#define RS BIT0


void I2C_config_B2(uint8_t isMaster, uint8_t ownAddr, uint32_t baudRate_kHz){

//    SCL -> P7.1
//    SDA -> P7.0

//    Configura periferico I2C + Reseta a interface

    isMaster = 1;

    UCB2CTLW0 = UCSSEL__SMCLK | UCSYNC | UCMODE_3 | UCSWRST;  // Clock, Sincrono, Modo I2C, UCSWRST=1

//    Master or Slave
    if(isMaster == 1)
        UCB2CTLW0 |= UCMST;
    else
        UCB2CTLW0 &= ~UCMST;

//    band rate
    UCB2BRW = baudRate_kHz;

//    Endereco proprio
    UCB2I2COA0 = UCOAEN | ownAddr;

//    Configura pinos
    P7DIR &= ~(BIT1 | BIT0);
    P7SEL1 &= ~(BIT1 | BIT0);
    P7SEL0 |= BIT1 | BIT0;
    P7REN |= BIT1 | BIT0;
    P7OUT |= BIT1 | BIT0;

    UCB2CTLW0 &= ~UCSWRST;
}

void i2cSendByte(uint8_t addr, uint16_t data) {
    I2C_Write(addr,&data,1);
}

uint8_t I2C_Write(uint8_t slaveAddr, uint8_t *data, uint16_t nBytes){

//    Endereco do Slave
    UCB2I2CSA = slaveAddr;
//    Gera Start e permite transmissao
    UCB2CTLW0 |= UCTXSTT | UCTR;

    // Escrever no TXBUF para destravar o ACK
    while(!(UCB2IFG & UCTXIFG));
    UCB2TXBUF = *data++; nBytes--;

    // Espera o ACK (espera STT = 0)
    while( !(UCB2IFG & UCTXIFG) && !(UCB2IFG & UCNACKIFG) );

    if(UCB2IFG & UCNACKIFG) {
        // Se o escravo nao respondeu
        UCB2CTLW0 |= UCTXSTP;
        //    Aguarda o fim do compartilhamento e limpa a flag
        while(UCB2CTLW0 & UCTXSTP);
        return -1; // ERRO: Escravo nao esta presente
    }

    while(nBytes--) {
        while(!(UCB2IFG & UCTXIFG));
        UCB2TXBUF = *data++;
    }

    // Aguarda o ultimo byte ser enviado
    while(!(UCB2IFG & UCTXIFG));
    // Limpa a flag manualmente pois nao escrevemos em TXBUF
    UCB2IFG &= ~UCTXIFG;

//    Gerar stop
    UCB2CTLW0 |= UCTXSTP;
//    Aguarda o fim do compartilhamento e limpa a flag
    while(UCB2CTLW0 & UCTXSTP);

    return 0;
}



void LcdWriteNibble(uint8_t slaveAddr, uint8_t nibble, uint8_t R_S){

    i2cSendByte(slaveAddr, (nibble << 4) | BT | 0  | 0 | R_S);
    i2cSendByte(slaveAddr, (nibble << 4) | BT | EN | 0 | R_S);
    i2cSendByte(slaveAddr, (nibble << 4) | BT | 0  | 0 | R_S);

    return;
}

void LcdWriteByte(uint8_t slaveAddr, uint8_t byte, uint8_t R_S){

    LcdWriteNibble(slaveAddr, (byte >> 4)  , R_S);              //Envia MSN
    LcdWriteNibble(slaveAddr, (byte & 0x0F) , R_S);             //Envia LSN

    return;
}

void Config_LCD(){

    LcdWriteNibble(0x27, 0x3, 0); //Verifica se est� em modo 8 bits
    LcdWriteNibble(0x27, 0x3, 0);
    LcdWriteNibble(0x27, 0x3, 0);
    LcdWriteNibble(0x27, 0x2, 0);//Configura modo 4 bits
    LcdWriteByte(0x27, 0x28,0);
    LcdWriteByte(0x27, 0x08,0);
    LcdWriteByte(0x27, 0x01,0);
    LcdWriteByte(0x27, 0x0F,0);

    LcdWriteByte(0x27, 1, 0); // limpa o LCD

}

void LcdDisplay(char *palavra){
    int i;
    for(i=0 ; palavra[i] != '\0' ; i++){

        LcdWriteByte(0x27, palavra[i], 1); // Escreve a letra da palavra
        if(i == 15) LcdWriteByte(0x27,0xC0,0); // Pula a linha

    }
    LcdWriteByte(0x27, 0x0F, 0); // Escreve no LCD

}

void LcdPosicao(uint8_t cursor, uint8_t linha){
    uint8_t pos;

    pos = BIT7 | cursor;
    if(linha==1){
        pos |= 0x40;
    }
    LcdWriteByte(0x27, pos, 0);
}



