/*
 * set.c
 *
 *  Created on: Dec 14, 2018
 *      Author: samuel
 */
#include <msp430.h>
#include <clock.h>
#include <ST7735.h>
#include <buttons.h>
extern const unsigned char y;
extern const unsigned char x;
volatile unsigned char *selected;
unsigned char index=0x00;
unsigned char set_mode(volatile unsigned char *hour,volatile unsigned char *minute,volatile unsigned char *second,volatile unsigned char *dayofweek,volatile unsigned char *day,volatile unsigned char *month,volatile unsigned char *year_h,volatile unsigned char *year_l){
    unsigned char lx,ly;
    RTCCTL13 |= RTCHOLD;
    switch(index){
    case 0x00:
        selected=hour;
        lx=x;
        ly=y+1;

        break;
    case 0x01:
        selected=minute;
        lx=x+30;
        ly=y+1;
        break;
    case 0x02:
        selected=second;
        lx=x+60;
        ly=y+1;
        break;
    case 0x03:
        lx=x+10;
        ly=y-30;
        selected=dayofweek;
        break;
    case 0x04:
        lx=x-10;
        ly=y-50;
        selected=month;
        break;
    case 0x05:
        lx=x+20;
        ly=y-50;
        selected=day;
        break;
    case 0x06:
        selected=year_l;
        break;
    default:
        index=0x00;
        break;
    }
    //Change State on button pushes
    switch(read_buttons()){
    case 0x04:
        *selected=*selected+1;
        break;
    case 0x01:
        *selected=*selected-1;
        break;
    case 0x02:
        //Next
        index++;
        return 5;
    case 0x20:
        //Back
        index--;
        return 5;
    case 0x40:
        RTCCTL13 &= ~(RTCHOLD);
        return 3;
    case 0x10:
        //Set dual clock time
        RTCCTL13 &= ~(RTCHOLD);
        return 9;
    }
    LCD_char(x,y,BCD_to_char(*hour,1));
    LCD_char(x+10,y,BCD_to_char(*hour,0));
    LCD_char(x+20,y,':');
    LCD_char(x+30,y,BCD_to_char(*minute,1));
    LCD_char(x+40,y,BCD_to_char(*minute,0));
    LCD_char(x+50,y,':');
    LCD_char(x+60,y,BCD_to_char(*second,1));
    LCD_char(x+70,y,BCD_to_char(*second,0));
    LCD_char(x+10,y-30,BCD_to_char(*dayofweek,1));
    LCD_char(x+20,y-30,BCD_to_char(*dayofweek,0));
    LCD_char(x-20,y-50,' ');
    LCD_char(x-10,y-50,BCD_to_char(*month,1));
    LCD_char(x+00,y-50,BCD_to_char(*month,0));
    LCD_char(x+10,y-50,'/');
    LCD_char(x+20,y-50,BCD_to_char(*day,1));
    LCD_char(x+30,y-50,BCD_to_char(*day,0));
    LCD_char(x+40,y-50,'/');
    LCD_char(x+50,y-50,BCD_to_char(0x20,1));//Only 100 years to fix this shortcut
    LCD_char(x+60,y-50,BCD_to_char(0x20,0));
    LCD_char(x+70,y-50,BCD_to_char(*year_l,1));
    LCD_char(x+80,y-50,BCD_to_char(*year_l,0));
    LCD_horizontal_line(lx,ly,20);
    return 5;
}



