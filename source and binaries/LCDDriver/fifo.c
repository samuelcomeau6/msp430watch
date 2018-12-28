/*
 * fifo.c
 *
 *  Created on: Feb 10, 2018
 *      Author: samuel
 */
#include <stdint.h>

#define size 512 //Must be 2^n

uint16_t volatile TxPutI;
uint16_t volatile TxGetI;
static char TxFifo[size];
static _Bool CmdFifo[size];
void TxFifo_Init(void){
    TxPutI = TxGetI = 0;
}
int TxFifo_Put(char data,_Bool cmd){
    if( (TxPutI - TxGetI)&~(size-1) ) return 0;
    TxFifo[TxPutI&(size-1)] = data;
    CmdFifo[TxPutI&(size-1)] = cmd;
    TxPutI++;
    return 1;
}
int TxFifo_Get(char *datapt,_Bool *cmdpt){
    if (TxPutI==TxGetI) return 0;
    *datapt = TxFifo[TxGetI & (size-1)];
    *cmdpt = CmdFifo[TxGetI & (size-1)];
    TxGetI++;
    return 1;
}
uint8_t TxFifo_Size(void){
    return ( (uint16_t) (TxPutI-TxGetI));
}




/**#define SIZE 120

unsigned char buffer[SIZE];
unsigned char head;
unsigned char tail;
void fifo_init(){
    head=0;
    tail=0;
}
void fifo_write(unsigned char byte){
    if((head+1)!=tail){
        buffer[head]=byte;
        head++;
        if(head>SIZE) head=0;
    }
}
unsigned char fifo_read(){
    if(head!=tail){
        return buffer[tail];
        tail++;
        if(tail>SIZE) tail=0;
    }
}
**/

