/*
 * clock.c
 *
 *  Created on: Dec 11, 2018
 *      Author: samuel
 */
#include <msp430.h>
#include <buttons.h>
#include <ST7735.h>
#include <clock.h>
#define BUILD_HOUR ((__TIME__[0] - '0') * 16 + __TIME__[1] - '0')
#define BUILD_MIN ((__TIME__[3] - '0') * 16 + __TIME__[4] - '0')
#define BUILD_SEC ((__TIME__[6] - '0') * 16 + __TIME__[7] - '0')
#define CAL 61//Calibration -61ppm determined 12/18/2018


unsigned char update;
unsigned char brightness;
const unsigned char y=54;
const unsigned char x=25;
void rtc_init(void){
    PJSEL0 = BIT4 | BIT5;                   // Initialize LFXT pins

    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
    CSCTL0_H = 0;                           // Lock CS registers
    // Configure RTC_C
    RTCCTL0_H = RTCKEY_H;                   // Unlock RTC
    // enable RTC time event interrupt

    RTCCTL13 = RTCBCD |RTCHOLD | RTCMODE;  // RTC enable, BCD mode, RTC hold
    RTCCTL0_L = RTCRDYIE_L;
    //Calibration
    RTCOCAL&=~(RTCOCALS);
    RTCOCAL_L=CAL;


         RTCYEAR = 0x2018;                       // Year
         RTCHOUR = BUILD_HOUR;                         // Hour
         RTCMIN = BUILD_MIN;                          // Minute
         RTCSEC = BUILD_SEC;                          // Seconds

    RTCCTL13 &= ~(RTCHOLD);                 // Start RTC
    update=2;
    brightness=0x10;
}
unsigned char clock_mode(void){
    while(1){
        //Change State on button pushes
        switch(read_buttons()){
        case 0x40:
        case 0x04:
            LCD_on();
            break;
        case 0x02:
            LCD_off();
            break;
        case 0x01:
            brightness+=0x10;
            LCD_setbacklight(brightness);
            LCD_on();
            break;
        case 0xC0:
            return 5;
        }
        display_clock();
        __bis_SR_register(LPM4_bits | GIE);
    }
}
void display_clock(void){
    if(update==1){
        LCD_char(x+60,y,BCD_to_char(RTCSEC,1));
        LCD_char(x+70,y,BCD_to_char(RTCSEC,0));
        update=0;
    }
    if(update==2){
        LCD_char(x,y,BCD_to_char(RTCHOUR,1));
        LCD_char(x+10,y,BCD_to_char(RTCHOUR,0));
        LCD_char(x+20,y,':');
        LCD_char(x+30,y,BCD_to_char(RTCMIN,1));
        LCD_char(x+40,y,BCD_to_char(RTCMIN,0));
        LCD_char(x+50,y,':');
        LCD_char(x+60,y,BCD_to_char(RTCSEC,1));
        LCD_char(x+70,y,BCD_to_char(RTCSEC,0));
        LCD_char(x-10,y-50,BCD_to_char(RTCMON,1));
        LCD_char(x+00,y-50,BCD_to_char(RTCMON,0));
        LCD_char(x+10,y-50,'/');
        LCD_char(x+20,y-50,BCD_to_char(RTCDAY,1));
        LCD_char(x+30,y-50,BCD_to_char(RTCDAY,0));
        LCD_char(x+40,y-50,'/');
        LCD_char(x+50,y-50,BCD_to_char(0x20,1));//Only 100 years to fix this shortcut
        LCD_char(x+60,y-50,BCD_to_char(0x20,0));
        LCD_char(x+70,y-50,BCD_to_char(RTCYEAR_L,1));
        LCD_char(x+80,y-50,BCD_to_char(RTCYEAR_L,0));

        update=0;
    }
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
