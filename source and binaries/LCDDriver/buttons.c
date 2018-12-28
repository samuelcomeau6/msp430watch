/*
 * buttons.c
 *
 *  Created on: Dec 8, 2018
 *      Author: samuel
 */

#include <msp430.h>
#include <buttons.h>
#define SET(x) |= x
#define CLR(x) &= ~x

#define RB1 BIT1
#define RB2 BIT2
#define RB3 BIT3

#define LB1 BIT2
#define LB2 BIT0
#define LB3 BIT1

unsigned char buttons=0x00;//Button
unsigned char buttonflags=0x00;
unsigned char pressed=0x00;
unsigned char heldbuttons;//Local

void buttons_init(void){
    //RB1 = P4.1
    P4SEL0 CLR(RB1);
    P4SEL1 CLR(RB1);
    P4DIR CLR(RB1);
    P4REN SET(RB1);
    P4OUT CLR(RB1);
    P4IES CLR(RB1);

    //RB2 =P4.2
    P4SEL0 CLR(RB2);
    P4SEL1 CLR(RB2);
    P4DIR CLR(RB2);
    P4REN SET(RB2);
    P4OUT CLR(RB2);
    P4IES CLR(RB2);

    //RB3 = P4.3
    P4SEL0 CLR(RB3);
    P4SEL1 CLR(RB3);
    P4DIR CLR(RB3);
    P4REN SET(RB3);
    P4OUT CLR(RB3);
    P4IES CLR(RB3);

    //LB1 = P3.2
    P3SEL0 CLR(LB1);
    P3SEL1 CLR(LB1);
    P3DIR CLR(LB1);
    P3REN SET(LB1);
    P3OUT CLR(LB1);
    P3IES CLR(LB1);

    //LB2 = P3.0
    P3SEL0 CLR(LB2);
    P3SEL1 CLR(LB2);
    P3DIR CLR(LB2);
    P3REN SET(LB2);
    P3OUT CLR(LB2);
    P3IES CLR(LB2);

    //LB3 = P3.1
    P3SEL0 CLR(LB3);
    P3SEL1 CLR(LB3);
    P3DIR CLR(LB3);
    P3REN SET(LB3);
    P3OUT CLR(LB3);
    P3IES CLR(LB3);
    //Configure timer A0 @32,700Hz
    TA0CTL SET(TASSEL__ACLK);
    TA0CTL SET(ID__2);//Divide clock by 2
    TA0CTL CLR(MC_3);//Stop timer
    TA0CCR0=49050;//3sec
    TA0CCTL0 SET(OUTMOD_1);//Set mode
    TA0CCR1=16;//~1 msec Time=ID/32700*count
    TA0CCR1 SET(OUTMOD_1);//Set mode

    //Enable Interrupts
    P3IE=BIT0|BIT1|BIT2;//Interrupts enabled
    P4IE=BIT1|BIT2|BIT3;//Interrupts enabled
    TA0CTL SET(TAIE);//CCTLx interrupts
    TA0CCTL0 SET(CCIE);//CCTL0 interrupt
    TA0CCTL1 SET(CCIE);
    //Set to 0
    buttons=0x00;
    heldbuttons=0x00;
    pressed=0x00;
}
unsigned char read_buttons(void){
    unsigned char temp=buttonflags;
    buttonflags=0x00;
    return temp;
}

#pragma vector=PORT3_VECTOR
__interrupt void P3_ISR(void)
{
    switch(__even_in_range(P3IV, P3IV__P3IFG7))
    {
    case P3IV__NONE:      break;       // No interrupts
    case P3IV__P3IFG0: //LB2
        pressed^=0x20;
        P3IES^=BIT0;
        break;
    case P3IV__P3IFG1: //LB3
        pressed^=0x40;
        P3IES^=BIT1;
        break;
    case P3IV__P3IFG2: //LB1
        pressed^=0x10;
        P3IES^=BIT2;
        break;
    default: break;
    }
    TA0CTL SET(TACLR);//Reset timer
    TA0CTL SET(MC_1);//Up mode Timer
}

#pragma vector=PORT4_VECTOR
__interrupt void P4_ISR(void)
{
    switch(__even_in_range(P4IV, P4IV__P4IFG7))
    {
    case P4IV__NONE:      break;       // No interrupts
    case P4IV__P4IFG1: //RB1
        pressed^=0x01;
        P4IES^=BIT1;
        break;
    case P4IV__P4IFG2: //RB2
        pressed^=0x02;
        P4IES^=BIT2;
        break;
    case P4IV__P4IFG3: //RB3
        pressed^=0x04;
        P4IES^=BIT3;
        break;
    default: break;
    }
    TA0CTL SET(TACLR);//Reset timer
    TA0CTL SET(MC_1);//Up mode Timer
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0CCR0_ISR(void){
    if(buttons==heldbuttons&&buttons!=0x00){
        buttons SET(0x80);
        buttonflags|=buttons;
        __bic_SR_register_on_exit(LPM4_bits);
    }
    TA0CTL CLR(MC_3);//Stop
    TA0CTL SET(TACLR);
}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TA0_ISR(void){
    switch(__even_in_range(TA0IV,TAIV__TAIFG)){
    case TAIV__NONE:break;
    case TAIV__TACCR1:
        //10ms since state change
        buttons=pressed;//Set flags for pressed buttons
        buttonflags|=buttons;
        heldbuttons=buttons;//Update to test if button held for 3s
        __bic_SR_register_on_exit(LPM4_bits);
        break;
    case TAIV__TACCR2:
            break;
    case TAIV__TACCR3:
            break;
    case TAIV__TACCR4:
            break;
    case TAIV__TACCR5:
            break;
    case TAIV__TACCR6:
            break;
    case TAIV__TAIFG:
            break;
    default:break;
    }
}
