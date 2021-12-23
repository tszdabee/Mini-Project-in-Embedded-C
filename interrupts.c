#include <xc.h>
#include "interrupts.h"

// ********** EDITME ********** //
// Choose run mode. TEST_MODE for testing (1s = 1h), REAL_MODE for realtime (1s = 1s)
// ********** EDITME ********** //
#define TEST_MODE

extern volatile int hour, sec; // Global variables for Timer0 increment
char light = 1; // Implement light value for mornings, otherwise previous state of light not saved resulting in inversion of lights

/************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE.
************************************/
void Interrupts_init(void)
{
	// Turn on Global Interrupts, Peripheral Interrupts, and Interrupt Source (Turn on Global last)
    PIR2bits.C1IF=0; // Clear interrupt flag (Non-essential)
    PIE2bits.C1IE = 1; // Enables comparator1 interrupt source (C1IE in PIE2 for comparator)
    IPR2bits.C1IP = 0; // Set comparator1 interrupt priority LOW
    PIE0bits.TMR0IE = 1; // Enables Timer0 interrupt source (TMR0IE for PIE0 for Timer0)
    IPR0bits.TMR0IP = 1; // Set Timer0 interrupt priority HIGH
    INTCONbits.IPEN = 1; // Enable priority levels on interrupt
    INTCONbits.GIEL = 1; // Enable peripheral interrupt
    INTCONbits.GIEH = 1; // Enable global interrupt
}

/************************************
 * Low priority interrupt service routine:
 * Responsible for turning on LED in low light conditions (i.e. night-time) and off in bright conditions (i.e. daytime).
 * Make sure all enabled interrupts are checked and flags cleared
************************************/
void __interrupt(low_priority) LowISR() //Low priority interrupt
{
    if(PIR2bits.C1IF){ // Checking interrupt source C1IF bit in PIR2
        light = !light; // Toggle light value, if between 1am-5am switch light value, don't update lights.
        if(hour < 1 || hour >= 5){ // Outside of 1am - 5am
            LATDbits.LATD7 = light; // Update light to toggled light value
            LATHbits.LATH3 = light;
        }
        PIR2bits.C1IF=0; // Clearing interrupt flag (C1IF bit in PIR2)
    }
}


/************************************
 * High priority interrupt service routine
 * Responsible for incrementing time unit by every real-world second.
 * Make sure all enabled interrupts are checked and flags cleared
************************************/
void __interrupt(high_priority) HighISR() // High priority interrupt every second
{
	// ISR code: Check the flag, Execute Code, Clear Flag
    if(PIR0bits.TMR0IF){ // Checking interrupt flag for Timer0 (TMR0IF in PIR0)
        // Rewriting initial bits to 3036 for Timer0 count as value resets every overflow. Ensures switching at 1s after overflow.
        TMR0H=3035>>8;   // 8 most significant bits for starting Timer0 count at 3035
        TMR0L=3035&0b11111111;   // 8 least significant bits for starting Timer0 count at 3035
#if defined(TEST_MODE) // if define directive for test mode
            ++hour; //add to hour for test mode
#elif defined(REAL_MODE) // if define directive for real mode
            ++sec; //add to sec for real mode
#else // if no mode set, show error message:
            #error "Please set TEST_MODE or REAL_MODE."
#endif
        LATDbits.LATD7 = light; // Update light to light value every real-life second
        LATHbits.LATH3 = light;
        if(hour >= 1 && hour < 5){ // Turning off lights between 1am and 5am
            LATDbits.LATD7=0; // Turn off lights RD7, RH3
            LATHbits.LATH3=0;
        }
        PIR0bits.TMR0IF=0; // Clearing interrupt flag for Timer0 (TMR0IF in PIR0)
    }
}