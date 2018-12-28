/*
 * spi.c
 *
 *  Created on: Jan 19, 2018
 *      Author: samuel
 */
#include <msp430.h>

#define SET(x) |= x
#define CLR(x) &= ~x
unsigned char RXData;
unsigned char TXData;
void SPI_init(){
    //Configure output pins
    P5DIR |= BIT0|BIT1|BIT2|BIT3;
    P5SEL0 SET(BIT0); //Pin 5.0=UCB1SIMO (MOSI)
    P5SEL0 SET(BIT1); //Pin 5.1=UCB1SOMI (MISO)
    P5SEL0 SET(BIT2); //Pin 5.2=UCB1CLK  (CLK)
    P5SEL0 SET(BIT3); //Pin 5.3=UCB1STE  (CS)

    //Set clocks
    CSCTL0_H = CSKEY_H; // Unlock clock system
    CSCTL1 SET(DCOFSEL0); // Set digital controlled oscillator to 8MHz!
    CSCTL1 SET(DCOFSEL1);
    CSCTL1 SET(DCOFSEL2);
    CSCTL1 CLR(DCORSEL); // Range select is 0
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK; //ACLK from 3.27kHz external, Sclk+Mclk from DCO
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__8;   // Set all dividers MCLK=1MHz
    CSCTL4 CLR(LFXTOFF);  //Turn on LFXT
    CSCTL4 CLR(SMCLKOFF); //Make sure SMCLK is on

    //Setup UCB1
    UCB1CTLW0 = UCSWRST; //Reset module
    UCB1CTLW0 SET(UCCKPH);  //set clock phase?
    UCB1CTLW0 CLR(UCCKPL); //Inactive low CLK
    UCB1CTLW0 SET(UCMSB); //MSB first
    UCB1CTLW0 CLR(UC7BIT); //8-bit
    UCB1CTLW0 SET(UCMST); //Set Master
    UCB1CTLW0 SET(UCMODE1); //Set 4-pin SPI STE(CS) active low
    UCB1CTLW0 SET(UCSYNC); //Has to be set to enable SPI
    UCB1CTLW0 SET(UCSSEL__SMCLK); //Clock source SCLK
    UCB1CTLW0 SET(UCSTEM); //4-wire SPI
    UCB1BRW = 0x00; //Set bit rate 4MHz

    UCB1CTLW0 CLR(UCSWRST); //Start SPI module
    UCB1IE SET(UCTXIE);
}
void SPI_tx(unsigned char byte){
    TXData=byte;
    UCB1IE |= UCTXIE;
    __bis_SR_register(LPM0_bits | GIE);
}

#pragma vector=EUSCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void){
    switch(__even_in_range(UCB1IV, USCI_SPI_UCTXIFG))
    {
        case USCI_NONE: break;
        case USCI_SPI_UCRXIFG:
            RXData = UCB1RXBUF;
            UCB1IFG &= ~UCRXIFG;
            break;
        case USCI_SPI_UCTXIFG:
            UCB1TXBUF = TXData; // Transmit characters
            UCB1IE &= ~UCTXIE; //Clear flag
            __bic_SR_register_on_exit(LPM0_bits);
            break;
        default: break;
    }
}


