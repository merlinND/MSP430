#include "msp430fg4618.h"
#include "lcd.h"

int main (void)
{
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
	lcd_init();
        display_dollar();
        lcd_display_number(42);
	for (;;); // endless loop
}