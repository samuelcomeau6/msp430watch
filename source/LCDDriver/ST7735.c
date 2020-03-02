/*
 * ST7735.c
 *
 *  Created on: Jan 19, 2018
 *      Author: samuel
 */
#include <ST7735.h>
#include <msp430.h>
#include <fifo.h>
#include <stdarg.h>
#include <stdio.h>

#define SET(x) |= x
#define CLR(x) &= ~x
#define DC BIT0 //Bit 0
#define RST BIT4 //Bit 4
char data=0x00;
_Bool cmd=1;
int empty=1;
unsigned char RXData;
unsigned char TXData;
unsigned char textR,textG,textB;
unsigned char backR,backG,backB;
unsigned char brightness;
void LCD_init(){
    //Initialize Fifo
    TxFifo_Init();
    //Port definitions
    //4.0 D/C
    P4SEL0 CLR(DC);
    P4SEL1 CLR(DC);
    P4DIR SET(DC);
    P4OUT SET(DC);
    //3.7 Backlight (TB0.6)
    P3DIR SET(BIT7);
    P3SEL0 SET(BIT7);
    P3SEL1 CLR(BIT7);
    TB0CTL SET(CNTL_3);
    TB0CTL SET(TBSSEL__ACLK); //TB0 @ 3.27khz
    TB0CTL SET(MC_2); //Set continuous mode
    TB0CCR0 = 255;
    TB0CCR6 = 0xEF;
    TB0CCTL6 = OUTMOD_7;
    //7.4 Reset
    P7SEL0 CLR(RST);
    P7SEL1 CLR(RST);
    P7DIR SET(RST);
    P7OUT SET(RST);
    //4.4 SPI4W
    P4SEL0 CLR(BIT4);
    P4SEL1 CLR(BIT4);
    P4DIR SET(BIT4);
    P4OUT SET(BIT4);
    //===SPI Initialization====
    P5DIR |= BIT0|BIT1|BIT2|BIT3;
    P5SEL0 SET(BIT0); //Pin 5.0=UCB1SIMO (MOSI)
    P5SEL0 SET(BIT1); //Pin 5.1=UCB1SOMI (MISO)
    P5SEL0 SET(BIT2); //Pin 5.2=UCB1CLK  (CLK)
    P5SEL0 SET(BIT3); //Pin 5.3=UCB1STE  (CS)

    //Set clocks
    FRCTL0 = FRCTLPW;
    FRCTL0_L |= NWAITS_1_L;
    CSCTL0_H = CSKEY_H; // Unlock clock system
    CSCTL1 CLR(DCOFSEL0); // Set digital controlled oscillator to 24MHz!
    CSCTL1 SET(DCOFSEL1);
    CSCTL1 SET(DCOFSEL2);
    CSCTL1 SET(DCORSEL); // Range select is 0
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK; //ACLK from 3.27kHz external, Sclk+Mclk from DCO
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers
    CSCTL4 CLR(LFXTOFF);  //Turn on LFXT
    CSCTL4 CLR(SMCLKOFF); //Make sure SMCLK is on

    //Setup UCB1
    UCB1CTLW0 = UCSWRST; //Reset module
    UCB1CTLW0 SET(UCCKPH);  //set clock phase?
    UCB1CTLW0 CLR(UCCKPL); //Inactive low CLK
    UCB1CTLW0 SET(UCMSB); //MSB first
    UCB1CTLW0 CLR(UC7BIT); //8-bit
    UCB1CTLW0 SET(UCMST); //Set Master
    UCB1CTLW0 SET(UCMODE1); //Set 4-pin SPI STE(CS) active low
    UCB1CTLW0 SET(UCSYNC); //Has to be set to enable SPI
    UCB1CTLW0 SET(UCSSEL__SMCLK); //Clock source SCLK
    UCB1CTLW0 SET(UCSTEM); //4-wire SPI
    UCB1BRW = 0x0000; //Set bit rate SMCLK MHz

    UCB1CTLW0 CLR(UCSWRST); //Start SPI module
    UCB1IE SET(UCTXIE);

    PM5CTL0 &= ~LOCKLPM5;
    //Reset
    P7OUT CLR(RST);
    //Wait 1ms
    __delay_cycles(1000);
    //High Reset pin
    P7OUT SET(RST);
    //Wait 150ms
    __delay_cycles(150000);
    //SLPOUT
    LCD_cmd(SLPOUT);
    //Wait 120ms
    __delay_cycles(120000);
    //Set frame control 1 TODO optimize frame rate
    LCD_cmd(FRMCTR1); //framerate In normal mode FR=850K/((RTNA x 2 + 40) x (LINE + FPA + BPA +2)) =>Min=45FPS
    LCD_data(0x0F);//RTNA=16
    LCD_data(0x40);//FPA=64
    LCD_data(0x40);//BPA=54
    //Set display inversion
    LCD_cmd(INVCTR);
    LCD_data(0x07);
    //Set power control 1 to 4.70v 2.5uA
    LCD_cmd(PWCTR1);
    LCD_data(0x02);// VRHP[4:0] (0-31) Sets GVDD=4.6V
    LCD_data(0x02);// VRHN[4:0] Set GVCL=-4.6
    //Set power control 2 to -7.35v
    LCD_cmd(PWCTR2);
    LCD_data(0xC5);// VGHBT[1:0] (0-15) Sets VGH=3*AVDD+VGH25-0.5 VGL=-7.5 VGH=2.4
    //Set power control 3 to large, booster freq = BCLK
    LCD_cmd(PWCTR3);
    LCD_data(0x0D);// AP[2:0] Sets Operational Amplifier Bias Current
    LCD_data(0x00);// DC[2:0] Booster Frequency
    //Set power control 4 to large, BCLK/4
    LCD_cmd(PWCTR4);
    LCD_data(0x8D);// AP[2:0] Sets Operational Amplifier Bias Current
    LCD_data(0x1A);// DC[2:0] Booster Frequency
    //set pc 5
    LCD_cmd(PWCTR5);
    LCD_data(0x8D);// AP[2:0] Sets Operational Amplifier Bias Current
    LCD_data(0xEE);// DC[2:0] Booster Frequency
    //Set vcom ctrl 1
    LCD_cmd(VMCTR1);
    LCD_data(0x51);// Default: 0x51 => +4.525
    LCD_data(0x4D);// Default: 0x4D => -0.575
    //Set gamma CTRP1
    LCD_cmd(GAMCTRP1);
    LCD_data(0x0a);
    LCD_data(0x1c);
    LCD_data(0x0c);
    LCD_data(0x14);
    LCD_data(0x33);
    LCD_data(0x2b);
    LCD_data(0x24);
    LCD_data(0x28);
    LCD_data(0x27);
    LCD_data(0x25);
    LCD_data(0x2C);
    LCD_data(0x39);
    LCD_data(0x00);
    LCD_data(0x05);
    LCD_data(0x03);
    LCD_data(0x0d);
    //Set GAMCTRN1
    LCD_cmd(GAMCTRN1);
    LCD_data(0x0a);
    LCD_data(0x1c);
    LCD_data(0x0c);
    LCD_data(0x14);
    LCD_data(0x33);
    LCD_data(0x2b);
    LCD_data(0x24);
    LCD_data(0x28);
    LCD_data(0x27);
    LCD_data(0x25);
    LCD_data(0x2D);
    LCD_data(0x3a);
    LCD_data(0x00);
    LCD_data(0x05);
    LCD_data(0x03);
    LCD_data(0x0d);

    //Set pixel format to 18b
    LCD_cmd(COLMOD);
    LCD_data(0x06);// Default: 0x06 => 18-bit/pixel
    //Display on
    LCD_cmd(DISPON);//Display On
    LCD_cmd(IDMOFF);
    //wait 1 ms
    __delay_cycles(1000);
    //Set MADCTL
    LCD_cmd(MADCTL);
    LCD_data(0b10000100);//Set MY=1 MX=0 MV=0 ML=0 RGB=0 MH=0

}
void LCD_cmd(unsigned char byte){
    TxFifo_Put(byte,1);
    UCB1IFG SET(UCTXIFG);
    __bis_SR_register(GIE);

}
void LCD_data(unsigned char byte){
    TxFifo_Put(byte,0);
    UCB1IFG SET(UCTXIFG);
    __bis_SR_register(GIE);
}

void LCD_setbacklight(unsigned char byte){
    brightness = byte;
    TB0CCR6=brightness;
}
unsigned char LCD_adjustbacklight(signed char delta){
    brightness+=delta;
    TB0CCR6=brightness;
    return brightness;
}
void LCD_on(void){
    LCD_cmd(DISPON);
    TB0CCTL6 = OUTMOD_7;
    TB0CCR6=brightness;
    TB0CTL SET(MC_2);
}
void LCD_off(void){
    TB0CTL=TB0CTL&~0x0030;
    TB0CCTL6 = OUTMOD_0;
    LCD_cmd(DISPOFF);

}
void LCD_coord(unsigned char x, unsigned char y){
    LCD_cmd(CASET);
    LCD_data(0x00);
    LCD_data(x);//XS
    LCD_data(0x00);
    LCD_data(MAXX);//XE

    LCD_cmd(RASET);
    LCD_data(0x00);
    LCD_data(y);//YS
    LCD_data(0x00);
    LCD_data(MAXY);//YE

    LCD_cmd(RAMWR);
}
void LCD_window(unsigned char x, unsigned char y,unsigned char x2, unsigned char y2){
    LCD_cmd(CASET);
    LCD_data(0x00);
    LCD_data(x);//XS
    LCD_data(0x00);
    LCD_data(x2);//XE

    LCD_cmd(RASET);
    LCD_data(0x00);
    LCD_data(y);//YS
    LCD_data(0x00);
    LCD_data(y2);//YE

    LCD_cmd(RAMWR);
}
void LCD_line(int x0, int y0, int x1, int y1) {
    //Bitmap/Bresenham's line algorithm <http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C>
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)>>1, e2;

    for(;;){
        LCD_pixel(x0,y0,textR,textG,textB);
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}
void LCD_horizontal_line(unsigned char x,unsigned char y, unsigned char l){
    unsigned char c;
    for(c=x;c<=(x+l);c++) LCD_pixel(c,y,textR,textG,textB);
}
void LCD_fill(unsigned char R, unsigned char G, unsigned char B){
    unsigned int i;
    unsigned int r;
    LCD_coord(0,0);
    for (r = MAXY;r; r--){
        for (i =MAXX;i; i--)
        {
            LCD_data(B); //Blue
            LCD_data(G); //Green
            LCD_data(R); //Red
        }
    }

}
void LCD_pixel(unsigned char x, unsigned char y, unsigned char R, unsigned char G, unsigned char B){
    LCD_coord(x,y);
    LCD_data(B);
    LCD_data(G);
    LCD_data(R);
}
void LCD_text_color(unsigned char R, unsigned char G, unsigned char B){
    textR=R;
    textG=G;
    textB=B;
}
void LCD_back_color(unsigned char R, unsigned char G, unsigned char B){
    backR=R;
    backG=G;
    backB=B;
}
void LCD_text(unsigned char x, unsigned char y, char* string){
    unsigned int i;
    for (i = 0; string[i] != 0; i++){
        char x2=x+8*i;
        if(x2>120){
            y=y-10;
            x=x-120;
        }
        LCD_char(x+8*i,y,string[i]);
    }
}

void LCD_printf(int x, int y, const char * format, int n_arg, ...){
    va_list valist;
    va_start(valist, n_arg);
    char temp_str[80];
    vsnprintf(temp_str, 80,format, valist);
    LCD_text(x,y,temp_str);
    va_end(valist);
}

void LCD_char(unsigned char x, unsigned char y, char c){
    const unsigned char (*letter);
    switch (c){
    case ' ':
        letter=bbl;
        break;
    case '0':
        letter=b0;
        break;
    case '1':
        letter=b1;
        break;
    case '2':
        letter=b2;
        break;
    case '3':
        letter=b3;
        break;
    case '4':
        letter=b4;
        break;
    case '5':
        letter=b5;
        break;
    case '6':
        letter=b6;
        break;
    case '7':
        letter=b7;
        break;
    case '8':
        letter=b8;
        break;
    case '9':
        letter=b9;
        break;
    case ':':
        letter=bcol;
        break;
    case ';':

        break;
    case '<':

        break;
    case '=':

        break;
    case '>':

        break;
    case 'A':
        letter=bA;
        break;
    case 'B':
        letter=bB;
        break;
    case 'C':
        letter=bC;
        break;
    case 'D':
        letter=bD;
        break;
    case 'E':
        letter=bE;
        break;
    case 'F':
        letter=bF;
        break;
    case 'G':
        letter=bG;
        break;
    case 'H':
        letter=bH;
        break;
    case 'I':
        letter=bI;
        break;
    case 'J':
        letter=bJ;
        break;
    case 'K':
        letter=bK;
        break;
    case 'L':
        letter=bL;
        break;
    case 'M':
        letter=bM;
        break;
    case 'N':
        letter=bN;
        break;
    case 'O':
        letter=bO;
        break;
    case 'P':
        letter=bP;
        break;
    case 'Q':
        letter=bQ;
        break;
    case 'R':
        letter=bR;
        break;
    case 'S':
        letter=bS;
        break;
    case 'T':
        letter=bT;
        break;
    case 'U':
        letter=bU;
        break;
    case 'V':
        letter=bV;
        break;
    case 'W':
        letter=bW;
        break;
    case 'X':
        letter=bX;
        break;
    case 'Y':
        letter=bY;
        break;
    case 'Z':
        letter=bZ;
        break;
    case '/':
        letter=bslash;
        break;
    default:
        letter=bbl;
        break;
    }
    unsigned char i;
    unsigned char k;
    for(k=10;k;k--){
        LCD_coord(x,y-k+10);
        for(i=8;i;i--){
            if(letter[k-1]&0x01<<(i-1)){
                LCD_data(textB);
                LCD_data(textG);
                LCD_data(textR);
            }
            else{
                LCD_data(backB);
                LCD_data(backG);
                LCD_data(backR);
            }
        }
    }
}

#pragma vector=EUSCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void){
    switch(__even_in_range(UCB1IV, USCI_SPI_UCTXIFG))
    {
    case USCI_NONE: break;
    case USCI_SPI_UCRXIFG: break;
    case USCI_SPI_UCTXIFG:
        empty = TxFifo_Get(&data,&cmd);
        if(empty==0){
            UCB1IFG CLR(UCTXIFG);
            break;
        }
        if(cmd) P4OUT CLR(DC);
        else P4OUT SET(DC);
        UCB1TXBUF=data;
        break;
    default: break;
    }
}
