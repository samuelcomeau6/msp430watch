/*
 * fifo.h
 *
 *  Created on: Feb 10, 2018
 *      Author: samuel
 */

#ifndef FIFO_H_
#define FIFO_H_
#include <stdint.h>

void TxFifo_Init(void);
int TxFifo_Put(char data,_Bool cmd);
int TxFifo_Get(char *datapt,_Bool *cmdpt);
uint8_t TxFifo_Size(void);
/**
void fifo_init();
void fifo_write(unsigned char byte);
unsigned char fifo_read();
**/


#endif /* FIFO_H_ */
