/*
 * clock.h
 *
 *  Created on: Dec 11, 2018
 *      Author: samuel
 */

#ifndef CLOCK_H_
#define CLOCK_H_

void rtc_init(void);
unsigned char clock_mode(void);
void display_clock(void);
unsigned char BCD_to_char(unsigned char byte, _Bool isHigh);

#endif /* CLOCK_H_ */
