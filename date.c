#include <xc.h>
#include "date.h"

extern volatile int year, month, day, hour, min, sec, bwdcheck;
extern char light;

/*********************************************
 * Function for incrementing minute every 60s
*********************************************/
void seccount(volatile int *sec){ // Use pointers to modify value
    if(*sec==60){ // Every 60 sec
        ++min; // Increment min
        *sec = 0; // Reset sec
    }
}

/*********************************************
 * Function for incrementing hour every 60 min
*********************************************/
void mincount(volatile int *min){
    if(*min==60){ // Every 60 min
        ++hour; // Increment hour
        *min = 0; // Reset min
    }
}

/*********************************************
 * Function for incrementing day every 24 hours
*********************************************/
void datecount(volatile int *hour, volatile int *day, volatile int *month, volatile int *year, int *bwdcheck){
    if(*hour==24){ // Every 24 hours, update dates
        if(*day > 0 && *day < 28){ // For 'initial' days of month
            ++*day; // Increment day
        }else if(*day == 28){ // On 28th, do Feb leap check
            if(*month==2){ // If Feb
                if(((*year%400==0)&&(*year%100!=0))||(*year%4==0)){ // Leap year check, Divisible by 400 and 4, but not divisible by 100.
                    ++*day; // 29th Feb for leap year
                }else{ // Non-leap year
                    *day = 1; // Reset day (Feb28>Mar1)
                    ++*month; // Increment month to March
                }
            }else{
                ++*day; // Non-Feb, increment day
            }
        }else if(*day == 29){ // On 29th, check last day for Feb leap year
            if(*month==2){ // If Feb 29, leap year
                *day = 1; // Reset day (Feb29>Mar1)
                ++*month; // Increment month to March
            }else{
                ++*day; // Non-Feb, increment day
            }
        }else if(*day == 30){ // On 30th, check last day for Apr/Jun/Sep/Nov
            if(*month == 4 || *month == 6 || *month == 9 || *month == 11){ //Apr/Jun/Sep/Nov
                *day = 1; // Reset day
                ++*month; // Increment month
            }else{
                ++*day; // Increment day if Jan/Mar/May/Jul/Aug/Oct/Dec
            }
        }else if (*day == 31){ // On 31st, check last day for Jan/Mar/May/Jul/Aug/Oct/Dec
            *day = 1; // Rest day, no other last days exist
            if(*month == 12){ // Dec check to increment year
                *month = 1; // Reset month (Dec31>Jan1)
                ++*year; // Increment year
                *bwdcheck = 0; // Reset bwdcheck, clocks go backwards only once per year
            }else{
                ++*month; // Increment month if Jan/Mar/May/Jul/Aug/Oct
            }
        }
        *hour = 0; // Reset hour
    }
}

/*********************************************
 * Function for calculating day of week (0=Sun, 1=Mon, 2=Tues etc.)
*********************************************/
int weekdaycheck(int d, int m, int y){
    return ((d+=m < 3 ? y-- : y-2, 23*m/9+d+4+y/4-y/100+y/400)%7); // Expression by Michael Keith and Tom Craver in 1990. Min keystrokes needed to convert Gregorian date to day of week.
} // Read more about this on https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week

/*********************************************
 * Function to adjust for Daylight Savings
 * In UK, for last Sunday in March (they go forward an hour at 1AM) 
 * In UK, for last Sunday in October (they go back an hour at 2AM).
*********************************************/
void dst(volatile int *hour, volatile int day, volatile int month, int weekday, int *bwdcheck){
    if(day >= 25 && day <= 31 && month == 3 && weekday == 0){ // Last Sunday in March
        if(*hour == 1){ // If 1AM for last Sunday in March
            *hour = 2; // Clock go forward 1 hour
        }
    }else if(day >= 25 && day <= 31 && month == 10 && weekday == 0 && *bwdcheck == 0){ // Last Sunday in Oct
        if(*hour == 2){ // If 2AM for last Sunday in Oct
            *hour = 1; // Clock go backwards 1 hour
            *bwdcheck = 1; // Set bwdcheck to true, prevent stuck in infinite backwards loop. Code runs once per year
        }
    }
}