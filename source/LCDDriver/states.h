/*
 * states.h
 *
 *  Created on: Feb 6, 2020
 *      Author: samuel
 */

#ifndef STATES_H_
#define STATES_H_

enum states{OFF, TENS, CLOCK, CMD, SET, STOPW, TIMER, NOTES, DTCLOCK, CAL, CONS, ALARM};
typedef enum states state_t;
state_t state;



#endif /* STATES_H_ */
