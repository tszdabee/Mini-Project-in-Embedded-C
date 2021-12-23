# Tsz's ECM Individual project - Energy saving automatic outside light

## Table of Contents
- [Objectives](#objectives)
- [Video Demonstration and Explanation](#video-demonstration-and-explanation)
- [Brief description of code](#brief-description-of-code)
- [User inputs and instructions](#user-inputs-and-instructions)

## Objectives

External lights that respond to ambient light are extremely common (i.e. porch lights, street lights, garden lights etc). Such lights switch on at dusk and then turn off at dawn. However, both energy and money can be saved if these lights are switched off during the morning (e.g. between 1am and 5am) with minimal pedestrians around. Many councils in the UK have implemented/trialled this idea for street lights (https://www.bbc.co.uk/news/uk-england-derbyshire-16811386). The objective of this task is to apply the knowledge of microcontrollers and hardware from this module to develop a fully automated solution.

The automatic light should satisfy the following objectives:
1. Monitoring light level with the LDR and turns on an LED in low light conditions (i.e. night-time) and off in bright conditions (i.e. daytime)
1. Displaying the current hour of day on the LED array in binary
1. Turning off the light between approx. 1am and 5am
1. Adjusting for daylight savings time
1. Maintaining synchronicity with the sun indefinitely
1. Be fully automatic (requires zero maintenance after installation)

Code in C was written to [MPLABX](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-x-ide), and [XC8 Compiler](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers) was used to compile the code. [Realterm](https://realterm.sourceforge.io/) serial terminal program used to support debugging efforts. [Github](https://github.com/) was used to support version control of the project by managing software development.

## Video Demonstration and Explanation
View the video demonstration and explanation of the automatic light here: 

https://youtu.be/GaFm1AWEGs0

## Brief description of code
The current date and time is recorded and displayed on LCD display, along with the current day of week indicated in the bottom right (0 = Sunday, 1 = Monday, 2 = Tuesday etc.). Leap years are properly accounted for and will automatically update along with the initialization date/time. The hour value is taken and displayed in the LED array in binary. 

In low light conditions, the lights RD7 and RH3 turn on and are off in bright light conditions, with the expection of being constantly off between hours 1AM and 5AM. 

Daylight savings is accounted for by using the internal timer system, with the clocks going forward by an hour on the last Sunday of March at 1AM and the clocks going backwards by an hour on the last Sunday of October at 2AM. A check was implemented to ensure an infinite loop does not occur when the clock goes backwards.

Synchronicity with the sun is adjusted by averaging the minute values between the time values of the lights turning on and the lights turning off, adjusting daily to maintain synchronicity with current time of +-30mins.

## User inputs and instructions
There are two inputs required by the user: Current initialization date/time and Real/Test Mode.
- Defining current date/time under line 8 in main.c
- Defining real/test mode as "#define TEST_MODE" in interrupts.c

