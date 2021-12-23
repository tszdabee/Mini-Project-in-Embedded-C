#include <xc.h>
#include "timers.h"

/************************************
 * Function to set up timer 0
************************************/
void Timer0_init(void)
{
    T0CON1bits.T0CS=0b010; // Clock source of Fosc/4
    T0CON1bits.T0ASYNC=1; // See datasheet errata - needed to ensure correct operation when Fosc/4 used as clock source
    T0CON1bits.T0CKPS=0b1000; // PS of 1:256, 16 bit timer overflows every second. (T_overflow = (4*PS/F_osc) * (2^16-1) bits. T_overflow of 1s obtained.)
    T0CON0bits.T016BIT=1;	// Toggle 16 bit mode for Timer0
	// Substituting values to obtain # bits given T_overflow = 1s
    // T_overflow = 4*PS/F_osc*#bits
    // 1 = 4*256/(64*10^6)*#bits
    // Manipulate algebra, obtain #bits = 62500. Hence Timer0 needs to count from 65535-62500 = 3035 to 65535.
    // 3035 converted to binary is 0b0000101111011011, splitting into most and least significant for timer count, or use following:
    TMR0H=3035>>8;   // 8 most significant bits for starting Timer0 count at 3035
    TMR0L=3035&0b11111111;   // 8 least significant bits for starting Timer0 count at 3035
    T0CON0bits.T0EN=1;	// Start the timer
}