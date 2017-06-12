#ifndef __TIMER_H
#define __TIMER_H
// SI POSSONO TOGLIERE
#include "GenericTypeDefs.h"
#include <p32xxxx.h>
//--------------------+
#define NUM_TIMER  10

void timer1Init(void);
void timer2Init(void);
void timer3Init(void);
void timer4Init(void);
void timer5Init(void);

char Timer_Tick(unsigned char t);
void Clear_Timer_Tick(unsigned char t);
void delay_ms(unsigned int ms);

#endif