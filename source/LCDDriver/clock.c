/*
 * clock.c
 *
 *  Created on: Dec 11, 2018
 *  Last Update: Feb 06, 2020
 *      Author: Samuel Comeau
 */
#include <stdio.h>
#include <stdlib.h>
#include <msp430.h>

#include "buttons.h"
#include "ST7735.h"
#include "clock.h"


#define CAL 61//Calibration -61ppm determined 12/18/2018



unsigned char update;
const unsigned char y=54;
const unsigned char x=25;




void rtc_init(void){
    // Initialize LFXT pins
    PJSEL0 = BIT4 | BIT5;

    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
    CSCTL0_H = 0;                           // Lock CS registers

    // Configure RTC_C
    RTCCTL0_H = RTCKEY_H;                   // Unlock RTC
    RTCCTL13 = RTCHOLD | RTCMODE;           // RTC enable, RTC hold
    //Set Calibration
    RTCOCAL&=~(RTCOCALS);
    RTCOCAL_L=CAL;
    RTCCTL0_L = RTCRDYIE_L;                 //Enable RTC interrupt

    //Start RTC
    RTCCTL13 &= ~(RTCHOLD);

    //Set for full update
    update=2;

    //Initialize time struct for NOW
    NOW.hours = &RTCHOUR;
    NOW.minutes = &RTCMIN;
    NOW.seconds = &RTCSEC;
    NOW.month = &RTCMON;
    NOW.day = &RTCDAY;
    NOW.year = &RTCYEAR;
    NOW.doweek = &RTCDOW;
}
state_t clock_mode(void){
    //Change State on button pushes
    switch(read_buttons()){
    case 0x40:
    case 0x04:
        return OFF;
    case 0x02:
        return NOTES;
    case 0x01:
        LCD_adjustbacklight(0x10);
        LCD_on();
        break;
    case 0xC0:
        return SET;
    }
    switch(update){
    case 1:
        update_seconds(&NOW);
        break;
    case 2:
        display_clock(&NOW);
        break;
    case 0:
    default:
        break;
    }
    return CLOCK;
}

void update_seconds(time_t * time){
    LCD_printf(x+48, y, "%02d", 1, *time->seconds);
}
void display_clock(time_t * time){
        LCD_printf(x, y, "%02d:%02d:%02d", 3, *time->hours,*time->minutes,*time->seconds);
        LCD_printf(x+20, y-20, "%02d",1,*time->doweek);
        LCD_printf(x-10, y-50, "%02d/%02d/%04d", 3, RTCMON,RTCDAY,RTCYEAR);
}
unsigned char BCD_to_char(unsigned char byte, _Bool isHigh){
    if(isHigh){
        return (((byte&0xF0)>>4)+0x30);
    }
    else{
        return ((byte&0x0F)+0x30);
    }
}

#pragma vector=RTC_C_VECTOR
__interrupt void RTC_ISR(void)
{
    switch(__even_in_range(RTCIV, RTCIV__RT1PSIFG))
    {
    case RTCIV__NONE:      break;       // No interrupts
    case RTCIV__RTCOFIFG:  break;       // RTCOFIFG
    case RTCIV__RTCRDYIFG:              // RTCRDYIFG
        if(RTCSEC!=0x00&&update==0) update=1;
        else update=2;
        __bic_SR_register_on_exit(LPM4_bits);
        break;
    case RTCIV__RTCTEVIFG:              // RTCEVIFG
        break;
    case RTCIV__RTCAIFG:   break;       // RTCAIFG
    case RTCIV__RT0PSIFG:  break;       // RT0PSIFG
    case RTCIV__RT1PSIFG:  break;       // RT1PSIFG
    default: break;
    }
}
