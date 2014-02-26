#include "msp430fg4618.h"
/* Initialize the LCD_A controller
claims P5.2-P5.4, P8, P9, and P10.0-P10.5
assumes ACLK to be default 32khz (LFXT1)
*/
void lcd_init()
{
	// our LCD screen is a SBLCDA4 => 4-mux operation (SLAU213 p4)
	// 4-mux operation needs all 4 common lines (COM0-COM3). COM0 has
	// a dedicated pin (pin 52, cf SLAS508H p3), but let's claim the
	// other three.
	P5DIR |= (BIT4 | BIT3 | BIT2); // pins are output direction
	P5SEL |= (BIT4 | BIT3 | BIT2); // select 'peripheral' function (VS GPIO)
	
	// Configure LCD controller
	LCDACTL =
		(1<<0) | // turn on the LCD_A module
		(0<<1) | // unused bit
		(1<<2) | // enable LCD segments
		(3<<3) | // LCD mux rate: 4-mux
		(3<<5) ; // frequency select
	
	// Configure port pins
	//
	// mappings are detailed on SLAU213 p15: our screen has 22
	// segments, wired to MCU pins S4 to S25 (shared with GPIO P8, P9,
	// and P10.0 to P10.5)
	LCDAPCTL0 =
		(0 << 0) | // MCU S0-S3 => not connected to the screen
		(1 << 1) | // MCU S4-S7 => screen pins S0-S3 (P$14-P$11)
		(1 << 2) | // MCU S8-S11 => screen pins S4-S7 (P$10-P$7)
		(1 << 3) | // MCU S12-S15 => screen pins S8-S11 (P$6 -P$3)
		(1 << 4) | // MCU S16-S19 => screen pins S12-S15 (P$2, P$1, P$19, P$20)
		(1 << 5) | // MCU S20-S23 => screen pins S16-S19 (P$21-P$24)
		(1 << 6) | // MCU S24-S25 => screen pins S20-21 (P$25, P$26)
		(0 << 7) ; // MCU S28-S31 => not connected to the screen
		
	LCDAPCTL1 = 0 ; // MCU S32-S39 => not connected to the screen
	// Note: as we do not intend to support battery-powered operation,
	// we don't need to mess with charge pumps and such.
	// clear all LCD memory (cf SLAU056J p. 26-4)
	
	int j;
	for( j=0 ; j<20 ; j++)
	{
		LCDMEM[j] = 0x00; // Shut down all the tiles
	}
}

void display_dollar()
{
   LCDMEM[12] = 0x10; // Power up the 26th pin with COM0 on the LCD component
                      // which is connected to the motherboard to the 21th pin
                      // which corresponds to the 25th in the msp430. So, in the
                      // LCD memory, it's at the 12th register in the 2nd word
}
