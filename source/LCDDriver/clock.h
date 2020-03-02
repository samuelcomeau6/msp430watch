/*
 * clock.h
 *
 *  Created on: Dec 11, 2018
 *      Author: samuel
 */
#include "states.h"
#ifndef CLOCK_H_
#define CLOCK_H_
struct time_tag{
    volatile unsigned char * hours;
    volatile unsigned char * minutes;
    volatile unsigned char * seconds;
    volatile unsigned char * month;
    volatile unsigned char * day;
    volatile unsigned int * year;
    volatile unsigned char * doweek;
};
typedef struct time_tag time_t;
time_t NOW;
void rtc_init(void);
state_t clock_mode(void);
void update_seconds(time_t * time);
void display_clock(time_t * time);
unsigned char BCD_to_char(unsigned char byte, _Bool isHigh);

#endif /* CLOCK_H_ */
