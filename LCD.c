#include <xc.h>
#include "LCD.h"

/************************************
 * Function to toggle LCD enable bit on then off
 * when this function is called the LCD screen reads the data lines
************************************/
void LCD_E_TOG(void)
{
    //turn the LCD enable bit on //(This will cause the LCD screen to check what we are writing to it)
    LCD_E = 1;
	__delay_us(5); //wait a short delay
	//turn the LCD enable bit off again
    LCD_E = 0;
}

/************************************
 * Function to set the 4-bit data line levels for the LCD
************************************/
void LCD_sendnibble(unsigned char number)
{
	// Setting data lines
    if (number & 0b0001) {LCD_DB4 = 1;} else {LCD_DB4 = 0;} // DB4 high if RB3 at pin 11 high
    if (number & 0b0010) {LCD_DB5 = 1;} else {LCD_DB5 = 0;} // DB5 high if RB2 at pin 12 high
    if (number & 0b0100) {LCD_DB6 = 1;} else {LCD_DB6 = 0;} // DB6 high if RE3 at pin 13 high
    if (number & 0b1000) {LCD_DB7 = 1;} else {LCD_DB7 = 0;} // DB7 high if RE1 at pin 14 high
    LCD_E_TOG();			//toggle the enable bit to instruct the LCD to read the data lines
    __delay_us(5);          //Delay 5uS
}

/************************************
 * Function to send full 8-bit commands/data over the 4-bit interface
 * high nibble (4 most significant bits) are sent first, then low nibble sent
************************************/
void LCD_sendbyte(unsigned char Byte, char type)
{
    // Set RS pin whether it is a Command (0) or Data/Char (1) using type argument
     LCD_RS = type;
    // send high bits of Byte using LCDout function
     LCD_sendnibble(Byte>>4); // Rightward bitwise shift of 4 to obtain 4 most significant bits
    // send low bits of Byte using LCDout function
     LCD_sendnibble(Byte&0b1111); // Use AND function to obtain 4 least significant bits
    __delay_us(50);               //delay 50uS (minimum for command to execute)
}

/************************************
 * Function to initialise the LCD after power on
************************************/
void LCD_Init(void)
{
    //Define all LCD Pins as Outputs
    TRISCbits.TRISC6=0;
    TRISCbits.TRISC2=0;
    TRISBbits.TRISB3=0;
    TRISBbits.TRISB2=0;
    TRISEbits.TRISE3=0;
    TRISEbits.TRISE1=0;
    
    // Set all pins low (might be random values on start up, fixes lots of issues)
    LCD_RS = 0;
    LCD_E = 0;
    LCD_DB4 = 0;
    LCD_DB5 = 0;
    LCD_DB6 = 0;
    LCD_DB7 = 0;
    
    //Initialisation sequence code
	// follow the sequence in the GitHub Readme picture for 4-bit interface.
	// first Function set should be sent with LCD_sendnibble (the LCD is in 8 bit mode at start up)
    __delay_ms(50);
    
    LCD_sendnibble(0b0011); 
    __delay_us(45);
    // Use LCD_sendbyte to operate in 4 bit mode. Initialization process below:
    LCD_sendbyte(0b00101100,0);
    __delay_us(45); // Min 39us execution time
    LCD_sendbyte(0b00101000,0); 
    __delay_us(45); // Min 39us execution time
    LCD_sendbyte(0b00101000,0); // Function Set: 4 bit, 2 line, 5x8 dots
    __delay_us(45); // Min 39us execution time
    LCD_sendbyte(0b00001000,0); // Display OFF, Cursor OFF, Blink OFF
    __delay_us(45); // Min 39us execution time
    LCD_sendbyte(0b00000001,0); // Clear display
    __delay_ms(5); // Min 1.53ms execution time
    LCD_sendbyte(0b00000110,0); //Entry Mode Set, enable cursor moving direction, display shift OFF
    __delay_us(45); // Min 39us execution time
	//remember to turn the LCD display back on at the end of the initialisation (not in the data sheet)
    LCD_sendbyte(0b00001100,0); // Display ON, Cursor ON, Blink ON
    __delay_us(45); // Min 39us execution time
}

/************************************
 * Function to set the cursor to beginning of line 1 or 2
************************************/
void LCD_setline (char line)
{
    if(line==1)
    {
        LCD_sendbyte(0x80,0); //Send 0x80 to set line to 1 (0x00 ddram address)
        __delay_us(45);
    }
    if(line==2){
        LCD_sendbyte(0xC0,0); //Send 0xC0 to set line to 2 (0x40 ddram address)
        __delay_us(45);
    }
}

/************************************
 * Function to send string to LCD screen
************************************/
void LCD_sendstring(char *string)
{	  
	while(*string != 0){  // While the data pointed to isn?t a 0x00 do below (strings in C must end with a NULL byte) 
		LCD_sendbyte(*string++,1); 	//Send out the current byte pointed to and increment the pointer
	}
}

/************************************
 * Function takes current date and time, then displays on LCD screen.
 * Useful for debugging and reference.
************************************/
void LCD_Disp(char buf[16], volatile int year, volatile int month, volatile int day, volatile int hour, volatile int min, volatile int sec, volatile int weekday)
{
    LCD_setline(1); // Set LCD Line 1
    sprintf(buf,"Date %02d/%02d/%04d",day,month,year); // Replace %d with relevant D/M/Y, padded with 2 zeros
    LCD_sendstring(buf); // Update Date in LCD line1
    LCD_setline(2); // Set LCD Line 2
    sprintf(buf,"Time %02d:%02d:%02d  %d",hour,min,sec,weekday); // Replace %d with relevant h:m:s, padded with 2 zeros
    LCD_sendstring(buf); // Update Time in LCD line2
    __delay_ms(98); // 98ms delay along with execution time. Update values every 100ms
}
