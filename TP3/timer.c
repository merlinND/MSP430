#include "msp430fg4618.h"

/*
 * Configure the Timer_A timer of the MSP430
 */
void init_timer ()
{
	// Reset the timer (clear any previous configuration)
	TACTL = TACTL | (1 << 2);
	// Set clock source to ACLK
	// Clock divider is 1 by default
	TACTL = TACTL | (1 << 8);

	// Enable Up mode
	TACTL = TACTL | (1 << 4);
	// Set the maximum value for Up mode
	TACCR0 = 326;

	// Enable interruptions
	// Note: timer interrupt vector is TAIV
	TACCTL0 = TACCTL0 | (1 << 4);
}