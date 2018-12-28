/*
 * main.c
 *
 *  Created on: Jan 19, 2018
 *      Author: samuel
 */

#include <msp430.h>
#include <ST7735.h>
#include <buttons.h>
#include <uart.h>
#include <clock.h>
#include <set.h>
extern unsigned char buttons;


unsigned char state;
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    //Set all unused pins to output
    P1DIR = 0xFF;
    P1OUT = 0x00;
    P2DIR = 0b10000100;
    P2OUT = 0x00;
    P3DIR = 0b01111000;
    P3OUT = 0x00;
    P4DIR = 0b11100000;
    P4OUT = 0x00;
    P5DIR = 0b11110010;
    P5OUT = 0x00;
    P6DIR = 0xFF; //Port doesn't exist?
    P6OUT = 0x00;
    P7DIR = 0b11101100;
    P7OUT = 0x00;
    P8DIR = 0xFF;
    P8OUT = 0x00;
    PJDIR = 0b11001111;
    PADIR = 0xFF;
    PAOUT = 0x00;
    PBDIR = 0xFF;
    PBOUT = 0x00;
    PCDIR = 0xFF;
    PCOUT = 0x00;
    PDDIR = 0xFF;
    PDOUT = 0x00;
    LCD_init();
    uart_init();
    rtc_init();
    buttons_init();


    LCD_setbacklight(0x10);
    LCD_fill(0x00,0x00,0x00);
    LCD_text_color(0xFF,0xFF,0xFF);
    LCD_back_color(0x00,0x00,0x00);
    state=3;
    //main loop:
    while(1){
        switch(state){
        case 1:
            //watch off
            break;
        case 2:
            //watch on 10s
            break;
        case 3:
            state = clock_mode();
            break;
        case 4:
            //CMD mode
            break;
        case 5:
            //Set Time
            state = set_mode(&RTCHOUR,&RTCMIN,&RTCSEC,&RTCDOW,&RTCDAY,&RTCMON,&RTCYEAR_H,&RTCYEAR_L);
            break;
        case 6:
            //Stopwatch
            break;
        case 7:
            //Timer
            break;
        case 8:
            //Notes
            break;
        case 9:
            //Dual Time
            break;
        case 10:
            //Calendar
            break;
        case 11:
            //Console
            break;
        case 12:
            //Alarm
            break;
        default:
            state=3;
            break;
        }
        __bis_SR_register(LPM4_bits | GIE);
    }
}


