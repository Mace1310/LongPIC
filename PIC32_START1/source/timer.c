#include <xc.h>
#include <sys/attribs.h>

#include "timer.h"

static int SW_Timer[NUM_TIMER];

static int SW_Timer_Reload[NUM_TIMER] = {
    1,              // 0    1 ms
    10,             // 1    10 ms
    75,             // 2    75 ms
    200,            // 3    200 ms
    500,            // 4    500 ms
    1000,           // 5    1 sec
    750,            // 6    750 ms
    5000,           // 7    5 sec
    10000,          // 8    10 sec
    20000           // 9    20 sec
};
static char SW_Timer_Flag[NUM_TIMER];

/********************** Inizzializzazioni **************************/

void timer1Init(void) {
    T1CON = 0;                              // Clear Timer 1 Register
	T1CONbits.TCKPS = 0;                    // Input Clock = PBCLK/1 = 20 MHz
	PR1 = 20000;                            // Timer Period : PR1/PBCLK = 20K/20M = 1 ms

    IFS0bits.T1IF = 0;                      // Clear Interrupt Flag
	IEC0SET = (1 << 4);                     // Timer 1 Interrupt Enable
	IPC1bits.T1IP = 4;                      // Timer 1 Priority : 4
    
    unsigned char i;
    for(i = 0; i < NUM_TIMER; i++) {   
        SW_Timer[i] = SW_Timer_Reload[i];   // Set SoftwareTimer Ticks
        SW_Timer_Flag[i] = 0;               // Clear SoftwareTimer Flags
    }
}   // SW_Timers Timer

void timer2Init(void) {
    T2CON = 0;                              // Clear Timer 2 Register
	T2CONbits.TCKPS = 3;                    // Input Clock : PBCLK/8 = 2.5 MHz
	PR2 = 8250;                             // Timer Period : PR2/(PBCLK/8) = 50000/2.5M = 20 ms
}    // PWM Timer

void timer3Init(void) {
    T3CON = 0;                              // Clear Timer 3 Register
	T3CONbits.TCKPS = 7;                    // Input Clock = PBCLK/256 = 78125 Hz Period = 1.28E-5 s
	PR3 = 0xFFFF;                           // Timer Period : PR3/PBCLK = 10K/10M = 1 ms
}    // Capture Timer
    
void timer4Init(void) {
    T4CON = 0;                              // Clear Timer 4 Register
	T4CONbits.TCKPS = 6;                    // Input Clock : PBCLK/64 = 312500 Hz Period = 3.2E-6 s
	PR4 = 234375;                            // Timer Period : PR4/(PBCLK/64) = 234375/312500 = 750 ms
}

void timer5Init(void) {
    T5CON = 0;                              // Clear Timer 5 Register
	T5CONbits.TCKPS = 0;                    // Input Clock : PBCLK/1 = 20 MHz
	PR5 = 20000;                            // Timer Period : PR5/PBCLK = 20K/20M = 1 ms
    
	IFS0bits.T5IF = 0;                      // Clear Interrupt Flag
    IEC0SET = (1 << 20);                    // Timer 5 Interrupt Enable
    IPC5bits.T5IP = 4;                      // Timer 5 Priority : 4
}

/********************** Funzioni **************************/

void delay_ms(unsigned int ms) {
	unsigned int i;

	while(ms--) {
		i = 10000;
		while(i--) Nop();
    }	
}

char Timer_Tick(unsigned char t) {
    return(SW_Timer_Flag[t]);
}
 
void Clear_Timer_Tick(unsigned char t) {
    SW_Timer_Flag[t] = 0;
}

/********************** Interrupts **************************/

void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1InterruptHandler(void) {
    IFS0bits.T1IF = 0;                              // Clear Interrupt Flag
    
    // SOFTWARE TIMERS
    unsigned char i;
    for(i = 0; i < NUM_TIMER; i++) {                // Check all SW_Timers
        if(!SW_Timer[i]--) {                        // If SW_Timer reaches 0
            SW_Timer[i] = SW_Timer_Reload[i];       // Reload SW_Timer
            SW_Timer_Flag[i] = 1;                   // Set the correspondent flag
        }
    } 
}