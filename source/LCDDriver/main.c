/*
 * main.c
 *
 *  Created on: Jan 19, 2018
 *      Author: samuel
 */

#include <msp430.h>

#include "ST7735.h"
#include "buttons.h"
#include "uart.h"
#include "clock.h"
#include "set.h"
#include "states.h"
#include "notes.h"
#include "sleep.h"

extern unsigned char buttons;
extern state_t state;
extern time_t NOW;


void IO_init(void);

void main(void) {
    //WDTCTL = WDTPW | WDTHOLD;
    WDTCTL = WDTPW | WDTSSEL_3 | WDTIS_3;               // Configure watchdog timer
    IO_init();
    LCD_init();
    uart_init();
    rtc_init();
    buttons_init();


    LCD_setbacklight(0x10);
    LCD_fill(0x00,0x00,0x00);  //Black
    LCD_text_color(0xFF,0xFF,0xFF); //White
    LCD_back_color(0x00,0x00,0x00); //Black
    state=CLOCK; //Clock
    //main loop:
    while(1){
        WDTCTL = WDTPW | WDTSSEL_3 | WDTIS_3 | WDTCNTCL;                  //Reset WDT
        switch(state){
        case OFF:
            //watch off
            state = off_mode();
            break;
        case TENS:
            //watch on 10s
            state = clock_mode(); //TODO implement on for 10s
            break;
        case CLOCK:
            state = clock_mode();
            break;
        case CMD:
            //CMD mode
            break;
        case SET:
            //Set Time
            state = set_mode(&NOW);
            break;
        case STOPW:
            //Stopwatch
            break;
        case TIMER:
            //Timer
            break;
        case NOTES:
            //Notes
            state = notes_mode();
            break;
        case DTCLOCK:
            //Dual Time
            state = clock_mode();//TODO DTMODE
            break;
        case CAL:
            //Calendar
            break;
        case CONS:
            //Console
            break;
        case ALARM:
            //Alarm
            break;
        default:
            state=CLOCK;
            break;
        }
        //Stop watchdog for power considerations
        WDTCTL = WDTPW | WDTSSEL_1 | WDTIS_3 | WDTHOLD;
        __bis_SR_register(LPM4_bits | GIE);
    }
}
void IO_init(void){
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
}

