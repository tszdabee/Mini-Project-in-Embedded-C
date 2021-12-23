#ifndef _date_H
#define _date_H

#include <xc.h>

#define _XTAL_FREQ 64000000


void seccount(volatile int *sec);
void mincount(volatile int *min);
void datecount(volatile int *hour, volatile int *day, volatile int *month, volatile int *year, int *bwdcheck);
int weekdaycheck(int day, int month, int year);
void dst(volatile int *hour, volatile int day, volatile int month, int weekday, int *bwdcheck);

#endif

