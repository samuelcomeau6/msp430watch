/*
 * sleep.c
 *
 *  Created on: Feb 7, 2020
 *      Author: samuel
 */
#include "states.h"
#include "ST7735.h"
#include "buttons.h"
#include "sleep.h"

state_t off_mode(void){
    LCD_off();
    switch(read_buttons()){
    case 0x04: //RB3
        LCD_on();
        return TENS;
    case 0x84: //RB3 Held
        LCD_on();
        return CLOCK;
    case 0x40: //LB3
        LCD_on();
        return TENS;
    case 0xC0: //LB3 Held
        LCD_on();
        return SET;
    case 0x02: //RB2
        LCD_on();
        return NOTES;
    case 0x82: //RB2 Held
        break;
    case 0x20: //LB2
        LCD_on();
        return DTCLOCK;
    case 0xA0: //LB2 Held
        break;
    case 0x01: //RB1
        LCD_adjustbacklight(0x10);
        LCD_on();
        return TENS;
    case 0x81: //RB1 Held
        LCD_setbacklight(0xFF);
        LCD_on();
        break;
    case 0x10: //LB1
        LCD_on();
        return STOPW;
    case 0x90: //LB1 Held
        LCD_on();
        return CMD;
    default:
        break;
    }
    return OFF;
}


