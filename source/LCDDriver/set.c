/*
 * set.c
 *
 *  Created on: Dec 14, 2018
 *      Author: samuel
 */
#include <msp430.h>

#include "set.h"
#include "clock.h"
#include "ST7735.h"
#include "buttons.h"
#include "states.h"

enum time_part {HRS,MINS,SECS,DOWEEKS,MONTHS,DAYS,YEARS};
typedef enum time_part time_part_t;

time_part_t index=HRS;
unsigned char Xpos = 25;
unsigned char Ypos = 54;

void change_time(time_t * time, time_part_t index,signed char delta){
    switch(index){
    case HRS:
        Xpos = 25;
        Ypos = 54;
        (*time->hours)+=delta;
        break;
    case MINS:
        Xpos = 25+CHAR_WIDTH*3;
        Ypos = 54;
        (*time->minutes)+=delta;
        break;
    case SECS:
        Xpos = 25+CHAR_WIDTH*6;
        Ypos = 54;
        (*time->seconds)+=delta;
        break;
    case DOWEEKS:
        Xpos = 25+20;
        Ypos = 54-20;
        (*time->doweek)+=delta;
        break;
    case MONTHS:
        Xpos = 25-10;
        Ypos = 54-50;
        (*time->month)+=delta;
        break;
    case DAYS:
        Xpos = 25-10+CHAR_WIDTH*3;
        Ypos = 54-50;
        (*time->day)+=delta;
        break;
    case YEARS:
        Xpos = 25-10+CHAR_WIDTH*6;
        Ypos = 54-50;
        (*time->year)+=delta;
        break;
    }
}

state_t set_mode(time_t * time){
    RTCCTL13 |= RTCHOLD;

    //Change State on button pushes
    switch(read_buttons()){
    case 0x04:
        change_time(time,index,1);
        break;
    case 0x01:
        change_time(time,index,-1);
        break;
    case 0x02:
        //Next
        index++;
        return SET;
    case 0x20:
        //Back
        index--;
        return SET;
    case 0x40:
        //Clear screen and return to normal clock
        RTCCTL13 &= ~(RTCHOLD);
        LCD_fill(0x00,0x00,0x00);
        return CLOCK;
    case 0x10:
        //Set dual clock time
        RTCCTL13 &= ~(RTCHOLD);
        return DTCLOCK;
   }
    if(index>YEARS){
        index=HRS;
    }
    display_clock(time);
    change_time(time,index,0);
    LCD_horizontal_line(Xpos,Ypos,2*CHAR_WIDTH);
    return SET;
}



