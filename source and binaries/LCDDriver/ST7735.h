/*
 * ST7735.h
 *
 *  Created on: Jan 19, 2018
 *      Author: samuel
 */

#ifndef ST7735_H_
#define ST7735_H_
//Commands
#define NOP         (0x00)  //No Operation
#define SWRESET     (0x01)  //Software reset
#define RDDID       (0x04)  //Read Display ID
#define RDDST       (0x09)  //Read display status
#define RDDPM       (0x0A)  //Read Display Power
#define RDDMADCTL   (0x0B)  //Read Display
#define RDDCOLMOD   (0x0C)  //Read Display Pixel

#define SLPIN       (0x10)  //Sleep mode and booster off
#define SLPOUT      (0x11)  //Exit Sleep mode and booster on
#define PTLON       (0x12)  //Partial mode on
#define NORON       (0x13)  //Normal Mode
#define INVOFF      (0x20)  //Display inversion off
#define INVON       (0x21)  //Display inversion on
#define GAMSET      (0x26)  //Gamma curve select
#define DISPOFF     (0x28)  //Display off
#define DISPON      (0x29)  //Display on
#define CASET       (0x2A)  //Column address set
#define RASET       (0x2B)  //Row address set
#define RAMWR       (0x2C)  //Memory write
#define RAMRD       (0x2E)  //Memory read
#define PTLAR       (0x30)  //Partial start/end address set
#define TEOFF       (0x34)  //Tearing effect line off
#define TEON        (0x35)  //Tearing effect mode set and on
#define MADCTL      (0x36)  //Memory data access control
#define IDMOFF      (0x38)  //Idle mode off
#define IDMON       (0x39)  //Idle mode on
#define COLMOD      (0x3A)  //Interface pixel format
#define RDID1       (0xDA)  //Read ID1
#define RDID2       (0xDB)
#define RDID3       (0xDC)

#define FRMCTR1  (0xB1)
#define FRMCTR2  (0xB2)
#define FRMCTR3  (0xB3)
#define INVCTR   (0xB4)
#define PWCTR1   (0xC0)
#define PWCTR2   (0xC1)
#define PWCTR3   (0xC2)
#define PWCTR4   (0xC3)
#define PWCTR5   (0xC4)
#define VMCTR1   (0xC5)
#define GAMCTRP1 (0xE0)
#define GAMCTRN1 (0xE1)

void LCD_init();
void LCD_cmd(unsigned char byte);
void LCD_data(unsigned char byte);
void LCD_setbacklight(unsigned char byte);
void LCD_coord(unsigned char x, unsigned char y);
void LCD_window(unsigned char x, unsigned char y,unsigned char x2, unsigned char y2);
void LCD_fill(unsigned char R, unsigned char G, unsigned char B);
void LCD_pixel(unsigned char x, unsigned char y, unsigned char R, unsigned char G, unsigned char B);
void LCD_text_color(unsigned char R, unsigned char G, unsigned char B);
void LCD_back_color(unsigned char R, unsigned char G, unsigned char B);
void LCD_char(unsigned char x, unsigned char y, char c);
void SPI_tx(unsigned char byte);
void LCD_on(void);
void LCD_off(void);
void LCD_line(int x0, int y0, int x1, int y1);
void LCD_horizontal_line(unsigned char x,unsigned char y, unsigned char l);
void LCD_text(unsigned char x, unsigned char y, char* string);
//
static const unsigned char bbl[10]={0b00000000,//1
                                    0b00000000,//2
                                    0b00000000,//3
                                    0b00000000,//4
                                    0b00000000,//5
                                    0b00000000,//6
                                    0b00000000,//7
                                    0b00000000,//8
                                    0b00000000,//9
                                    0b00000000};//10
static const unsigned char b1[10]={0b00000000,//1
                                   0b00011000,//2
                                   0b00101000,//3
                                   0b00001000,//4
                                   0b00001000,//5
                                   0b00001000,//6
                                   0b00001000,//7
                                   0b00111110,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char b2[10]={0b00000000,//1
                                   0b00011000,//2
                                   0b00100100,//3
                                   0b00000100,//4
                                   0b00000100,//5
                                   0b00001000,//6
                                   0b00010000,//7
                                   0b00111110,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char b3[10]={0b00000000,//1
                                   0b00111000,//2
                                   0b00000100,//3
                                   0b00000100,//4
                                   0b00011000,//5
                                   0b00000100,//6
                                   0b00000100,//7
                                   0b00111000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char b4[10]={0b00000000,//1
                                   0b00001100,//2
                                   0b00010100,//3
                                   0b00100100,//4
                                   0b01000100,//5
                                   0b01111110,//6
                                   0b00000100,//7
                                   0b00000100,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char b5[10]={0b00000000,//1
                                   0b00111100,//2
                                   0b00100000,//3
                                   0b00100000,//4
                                   0b00111000,//5
                                   0b00000100,//6
                                   0b00000100,//7
                                   0b00111000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char b6[10]={0b00000000,//1
                                   0b00011100,//2
                                   0b00100000,//3
                                   0b00100000,//4
                                   0b00111100,//5
                                   0b00100010,//6
                                   0b00100010,//7
                                   0b00011100,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char b7[10]={0b00000000,//1
                                   0b00111100,//2
                                   0b00000100,//3
                                   0b00000100,//4
                                   0b00001000,//5
                                   0b00001000,//6
                                   0b00010000,//7
                                   0b00010000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char b8[10]={0b00000000,//1
                                   0b00011000,//2
                                   0b00100100,//3
                                   0b00100100,//4
                                   0b00011000,//5
                                   0b00100100,//6
                                   0b00100100,//7
                                   0b00011000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char b9[10]={0b00000000,//1
                                   0b00011000,//2
                                   0b00100100,//3
                                   0b00100100,//4
                                   0b00011100,//5
                                   0b00000100,//6
                                   0b00000100,//7
                                   0b00011000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char b0[10]={0b00000000,//1
                                   0b00011000,//2
                                   0b00100100,//3
                                   0b00101100,//4
                                   0b00111100,//5
                                   0b00110100,//6
                                   0b00100100,//7
                                   0b00011000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bA[10]={0b00000000,//1
                                   0b00011000,//2
                                   0b00011000,//3
                                   0b00100100,//4
                                   0b00100100,//5
                                   0b00111100,//6
                                   0b01000010,//7
                                   0b01000010,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bB[10]={0b00000000,//1
                                   0b00111100,//2
                                   0b00100010,//3
                                   0b00100010,//4
                                   0b00111100,//5
                                   0b00100010,//6
                                   0b00100010,//7
                                   0b00111100,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bC[10]={0b00000000,//1
                                   0b00011100,//2
                                   0b00100000,//3
                                   0b00100000,//4
                                   0b00100000,//5
                                   0b00100000,//6
                                   0b00100000,//7
                                   0b00011100,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bD[10]={0b00000000,//1
                                   0b00111000,//2
                                   0b00100100,//3
                                   0b00100010,//4
                                   0b00100010,//5
                                   0b00100010,//6
                                   0b00100100,//7
                                   0b00111000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bE[10]={0b00000000,//1
                                   0b00111100,//2
                                   0b00100000,//3
                                   0b00100000,//4
                                   0b00111100,//5
                                   0b00100000,//6
                                   0b00100000,//7
                                   0b00111100,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bF[10]={0b00000000,//1
                                   0b00111100,//2
                                   0b00100000,//3
                                   0b00100000,//4
                                   0b00111100,//5
                                   0b00100000,//6
                                   0b00100000,//7
                                   0b00100000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bG[10]={0b00000000,//1
                                   0b00001110,//2
                                   0b00010000,//3
                                   0b00100000,//4
                                   0b00100110,//5
                                   0b00100010,//6
                                   0b00010010,//7
                                   0b00001110,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bH[10]={0b00000000,//1
                                   0b00100100,//2
                                   0b00100100,//3
                                   0b00100100,//4
                                   0b00111100,//5
                                   0b00100100,//6
                                   0b00100100,//7
                                   0b00100100,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bI[10]={0b00000000,//1
                                   0b00111110,//2
                                   0b00001000,//3
                                   0b00001000,//4
                                   0b00001000,//5
                                   0b00001000,//6
                                   0b00001000,//7
                                   0b00111110,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bJ[10]={0b00000000,//1
                                   0b00111100,//2
                                   0b00000100,//3
                                   0b00000100,//4
                                   0b00000100,//5
                                   0b00000100,//6
                                   0b00100100,//7
                                   0b00011000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bK[10]={0b00000000,//1
                                   0b00100010,//2
                                   0b00100100,//3
                                   0b00101000,//4
                                   0b00110000,//5
                                   0b00101000,//6
                                   0b00100100,//7
                                   0b00100010,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bL[10]={0b00000000,//1
                                   0b00100000,//2
                                   0b00100000,//3
                                   0b00100000,//4
                                   0b00100000,//5
                                   0b00100000,//6
                                   0b00100000,//7
                                   0b00111100,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bM[10]={0b00000000,//1
                                   0b01000010,//2
                                   0b01100110,//3
                                   0b01100110,//4
                                   0b01011010,//5
                                   0b01011010,//6
                                   0b01000010,//7
                                   0b01000010,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bN[10]={0b00000000,//1
                                   0b00100100,//2
                                   0b00110100,//3
                                   0b00110100,//4
                                   0b00101100,//5
                                   0b00101100,//6
                                   0b00100100,//7
                                   0b00100100,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bO[10]={0b00000000,//1
                                   0b00011000,//2
                                   0b00100100,//3
                                   0b01000010,//4
                                   0b01000010,//5
                                   0b01000010,//6
                                   0b00100100,//7
                                   0b00011000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bP[10]={0b00000000,//1
                                   0b00111100,//2
                                   0b00100010,//3
                                   0b00100010,//4
                                   0b00111100,//5
                                   0b00100000,//6
                                   0b00100000,//7
                                   0b00100000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bQ[10]={0b00000000,//1
                                   0b00011000,//2
                                   0b00100100,//3
                                   0b01000010,//4
                                   0b01000010,//5
                                   0b01000010,//6
                                   0b00101100,//7
                                   0b00011000,//8
                                   0b00001000,//9
                                   0b00001110};//10
static const unsigned char bR[10]={0b00000000,//1
                                   0b00111000,//2
                                   0b00100100,//3
                                   0b00100100,//4
                                   0b00111000,//5
                                   0b00101000,//6
                                   0b00100100,//7
                                   0b00100100,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bS[10]={0b00000000,//1
                                   0b00011100,//2
                                   0b00100010,//3
                                   0b00100000,//4
                                   0b00011100,//5
                                   0b00000010,//6
                                   0b00100010,//7
                                   0b00011100,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bT[10]={0b00000000,//1
                                   0b00111110,//2
                                   0b00001000,//3
                                   0b00001000,//4
                                   0b00001000,//5
                                   0b00001000,//6
                                   0b00001000,//7
                                   0b00001000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bU[10]={0b00000000,//1
                                   0b00100010,//2
                                   0b00100010,//3
                                   0b00100010,//4
                                   0b00100010,//5
                                   0b00100010,//6
                                   0b00100010,//7
                                   0b00011100,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bV[10]={0b00000000,//1
                                   0b01000010,//2
                                   0b01000010,//3
                                   0b00100100,//4
                                   0b00100100,//5
                                   0b00100100,//6
                                   0b00011000,//7
                                   0b00011000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bW[10]={0b00000000,//1
                                   0b01000010,//2
                                   0b01000010,//3
                                   0b01000010,//4
                                   0b01011010,//5
                                   0b01011010,//6
                                   0b01100110,//7
                                   0b01100110,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bX[10]={0b00000000,//1
                                   0b01000010,//2
                                   0b01000010,//3
                                   0b00100100,//4
                                   0b00011000,//5
                                   0b00100100,//6
                                   0b01000010,//7
                                   0b01000010,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bY[10]={0b00000000,//1
                                   0b00100010,//2
                                   0b00010100,//3
                                   0b00010100,//4
                                   0b00001000,//5
                                   0b00001000,//6
                                   0b00001000,//7
                                   0b00001000,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bZ[10]={0b00000000,//1
                                   0b00111100,//2
                                   0b00000100,//3
                                   0b00001000,//4
                                   0b00001000,//5
                                   0b00010000,//6
                                   0b00100000,//7
                                   0b00111100,//8
                                   0b00000000,//9
                                   0b00000000};//10
static const unsigned char bcol[10]={0b0000000,//1
                                     0b0001100,//2
                                     0b0001100,//3
                                     0b0000000,//4
                                     0b0000000,//5
                                     0b0001100,//6
                                     0b0001100,//7
                                     0b0000000,//8
                                     0b0000000,//9
                                     0b0000000};//10
static const unsigned char bslash[10]={0b00000100,//1
                                       0b00000100,//2
                                       0b00001000,//3
                                       0b00001000,//4
                                       0b00010000,//5
                                       0b00010000,//6
                                       0b00010000,//7
                                       0b00100000,//8
                                       0b00100000,//9
                                       0b00000000};//10
#endif /* ST7735_H_ */
