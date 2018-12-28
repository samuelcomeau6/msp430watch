/*
 * spi.h
 *
 *  Created on: Jan 19, 2018
 *      Author: samuel
 */

#ifndef SPI_H_
#define SPI_H_

#define SPI_CS_LCD          //Chip select port mask


void SPI_init();
void SPI_tx(unsigned char byte);
void screen_send(unsigned char Tx[49152]);



#endif /* SPI_H_ */
