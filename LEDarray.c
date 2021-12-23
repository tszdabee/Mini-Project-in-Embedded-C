#include <xc.h>
#include "LEDarray.h"

/************************************
/ LEDarray_init
/ Function used to initialise pins to drive the LEDarray
************************************/
void LEDarray_init(void)
{
    // Setup LED array pins G0, G1, A2, F6, A4, A5, F0, B0, B1 for output
    //set up TRIS registers for pins connected to LED array
    TRISGbits.TRISG0 = 0;
    TRISGbits.TRISG1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISFbits.TRISF6 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 0;
    TRISFbits.TRISF0 = 0;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    
	//set initial output LAT values (they may have random values when powered on)
    LATGbits.LATG0 = 0;
    LATGbits.LATG1 = 0;
    LATAbits.LATA2 = 0;
    LATFbits.LATF6 = 0;
    LATAbits.LATA4 = 0;
    LATAbits.LATA5 = 0;
    LATFbits.LATF0 = 0;
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
}

/************************************
/ Setup RH3 and RD7 LED for output
************************************/
void RH3_RD7_init(void)
{
    LATHbits.LATH3=0;   // Set initial output state for RH3 LED
    TRISHbits.TRISH3=0; // Set TRIS value for pin (output)
    LATDbits.LATD7=1;   // Set initial output state for RD7 LED
    TRISDbits.TRISD7=0; // Set TRIS value for pin (output)
}
/************************************
/ LEDarray_disp_bin
/ Function used to display a number on the LED array in binary
************************************/
void LEDarray_disp_bin(volatile int number)
{
	// code to turn on/off the pins connected to the LED array
    // Option 0: use math to check remainder with % and compute each bit. computationally intensive and inefficient.
    // Option 1: compare numbers to individual bits with & operator ie. number & 0b000000100 for checking bit2. Somewhat long code
    // Option 2: similar to above, but use bitwise shift rightwards until desired bit at rightmost and compare to 0b1 (aka 1). Cleaner and consistent code
    if(number & 1) {LATGbits.LATG0 = 1;} else {LATGbits.LATG0 = 0;} // bit0 ON if corresponding bit is 1
    if((number>>1) & 1) {LATGbits.LATG1 = 1;} else {LATGbits.LATG1 = 0;} // bit1 ON if corresponding bit is 1
    if((number>>2) & 1) {LATAbits.LATA2 = 1;} else {LATAbits.LATA2 = 0;} // bit2 ON if corresponding bit is 1
    if((number>>3) & 1) {LATFbits.LATF6 = 1;} else {LATFbits.LATF6 = 0;} // bit3 ON if corresponding bit is 1
    if((number>>4) & 1) {LATAbits.LATA4 = 1;} else {LATAbits.LATA4 = 0;} // bit4 ON if corresponding bit is 1
    if((number>>5) & 1) {LATAbits.LATA5 = 1;} else {LATAbits.LATA5 = 0;} // bit5 ON if corresponding bit is 1
    if((number>>6) & 1) {LATFbits.LATF0 = 1;} else {LATFbits.LATF0 = 0;} // bit6 ON if corresponding bit is 1
    if((number>>7) & 1) {LATBbits.LATB0 = 1;} else {LATBbits.LATB0 = 0;} // bit7 ON if corresponding bit is 1
    if((number>>8) & 1) {LATBbits.LATB1 = 1;} else {LATBbits.LATB1 = 0;} // bit8 ON if corresponding bit is 1
}
