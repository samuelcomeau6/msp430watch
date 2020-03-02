/*
 * notes.c
 *
 *  Created on: Feb 7, 2020
 *      Author: samuel
 */
#include "ST7735.h"
#include "buttons.h"
#include "states.h"

state_t notes_mode(void){
    LCD_printf(0, MAXY-CHAR_HEIGHT, "FROM LEFT: ",0);
    LCD_printf(0, MAXY-CHAR_HEIGHT*2,"SBWTCK, SBWTDIO, GND, TXD, RXD",0);
    switch(read_buttons()){
    case 0x04: //RB3
        //Scroll up
        break;
    case 0x84: //RB3 Held
        break;
    case 0x40: //LB3
        break;
    case 0xC0: //LB3 Held
        //new note
        break;
    case 0x02: //RB2
        //Next note
        break;
    case 0x82: //RB2 Held
        break;
    case 0x20: //LB2
        //Previous note
        break;
    case 0xA0: //LB2 Held
        break;
    case 0x01: //RB1
        //Scroll down
        break;
    case 0x81: //RB1 Held
        break;
    case 0x10: //LB1
        return DTCLOCK;
    case 0x90: //LB1 Held
        return TENS;
    default:
        break;
    }
    return NOTES;
}


