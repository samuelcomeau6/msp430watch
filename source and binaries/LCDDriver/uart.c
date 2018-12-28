/*
 * uart.c
 *
 *  Created on: Feb 5, 2018
 *      Author: samuel
 */
#include <msp430.h>
char TXData;
void uart_init(){
    P2SEL1 |= (BIT5|BIT6);
    PM5CTL0 &= ~LOCKLPM5;
    UCA1CTLW0 = UCSWRST;                    // Put eUSCI in reset
    UCA1CTLW0 |= UCSSEL__SMCLK;             // CLK = SMCLK
    UCA1BRW = 52;                           // 8000000/16/9600
    UCA1MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
    UCA1CTLW0 &= ~UCSWRST;                  // Initialize eUSCI
    UCA1IE |= UCRXIE;                       // Enable USCI_A3 RX interrupt

}
void uart_send(unsigned char d){
    TXData=d;
    UCA1IE |= UCTXIE;
    __bis_SR_register(LPM4_bits | GIE);
}

#pragma vector=EUSCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
    {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG: break;
        case USCI_UART_UCTXIFG:
            UCA1TXBUF = TXData; // Transmit characters
            UCA1IE &= ~UCTXIE; //Clear flag
            __bic_SR_register_on_exit(LPM4_bits);break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;
        default: break;
    }
}

