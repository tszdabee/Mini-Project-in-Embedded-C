// ********** README ********** //
// Before starting, current date/time needs to be defined in main.c, and real/test mode needs to be defined in interrupts.c
// ********** README ********** //

// ********** EDITME ********** //
// Initialize starting date/time. Input starting values.
// ********** EDITME ********** //
volatile int year = 2021, month = 12, day = 31, hour = 0, min = 59, sec = 40;

// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include <stdio.h>
#include "interrupts.h"
#include "LEDarray.h"
#include "timers.h"
#include "comparator.h"
#include "LCD.h"
#include "date.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) {
    RH3_RD7_init(); // Initialize RH3 light
    LEDarray_init(); // Initialize LED Array
    Timer0_init(); // Initialize Timer0
    Interrupts_init(); // Initialize Interrupts
    Comp1_init(); // Initialize Comparator
    LCD_Init(); // Initialize LCD

    char buf[16]; // Buffer to refresh LCD values
    int weekday, bwdcheck=0; // Weekday count (0=Sun, 1=Mon, 2=Tues etc.), Backward check for DST Clock backwards in October
    
    while(1){
        seccount(&sec); // Increment seconds
        mincount(&min); // Increment minutes
        datecount(&hour, &day, &month, &year, &bwdcheck); // Increment date
        weekday = weekdaycheck(day, month, year); // Find day of week
        dst(&hour, day, month, weekday, &bwdcheck); // Adjust for daylight saving time
        LEDarray_disp_bin(hour); // LED Array to display hour count in binary
        LCD_Disp(buf,year,month,day,hour,min,sec,weekday); // LCD Display for Date and Time for reference
        __delay_ms(20); // 20ms delay to update LED display
    }
}
