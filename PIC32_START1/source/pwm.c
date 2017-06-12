#include <xc.h>
#include <math.h>

#include "global_define.h"
#include "global_var.c"

/********************** Inizzializzazione **************************/

void pwmInit(void) {
    timer2Init();               // Initialize Timer 2
    
    T2CONSET = 0x8000;          // Start Timer 2
    
    // ESC1
    OC1CON = 0;                 // Compare 1 Timer Source : Timer 2
    OC1R = 0;
    OC1RS = 0;
    OC1CONbits.OCM = 6;         // PWM Mode Set
    OC1CONbits.ON = 1;          // PWM ON

    // ESC2
    OC2CON = 0;                 // Compare 2 Timer Source : Timer 2
    OC2R = 0;
    OC2RS = 0;
    OC2CONbits.OCM = 6;         // PWM Mode Set
    OC2CONbits.ON = 1;          // PWM ON
}

/********************** Funzioni **************************/

double round(double d) {
    return floor(d + 0.5);
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void WRITE_ESC1(unsigned char Val) {
    if (Val == 0) OC1RS = 2500;
    else {
        switch(MODE_CODE) {
            case BEGINNER:
            case ECO:
                OC1RS = map(Val, 1, 100, 3000, 4000);
                break;
            default:
                OC1RS = map(Val, 1, 100, 3000, 5000);
                break;
        } 
    }
}

void WRITE_ESC2(unsigned char Val) {
    if (Val == 0) OC2RS = 2500;
    else {
        switch(MODE_CODE) {
            case BEGINNER:
            case ECO:
                OC2RS = map(Val, 1, 100, 3000, 4000);
                break;
            default:
                OC2RS = map(Val, 1, 100, 3000, 5000);
                break;
        }
    }
}