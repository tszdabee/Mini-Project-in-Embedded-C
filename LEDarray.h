#ifndef _LEDarray_H
#define _LEDarray_H

#include <xc.h>

#define _XTAL_FREQ 64000000

//function prototypes
void LEDarray_init(void);
void RH3_RD7_init(void);
void LEDarray_disp_bin(volatile int number);


#endif
