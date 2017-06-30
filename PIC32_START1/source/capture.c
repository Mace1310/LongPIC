#include <xc.h>
#include <sys/attribs.h>

#include "global_var.c"
#include "global_define.h"

//float periodo_1;
//float periodo_2;

/********************** Inizzializzazione **************************/
/*
void captureInit() {
    timer3Init();               // Initialize Timer 3
    
    T3CONSET = 0x8000;          // Start Timer 3

    //RPM1
    IC1CON = 0;                 // Clear Capture Registers
    IC1BUF = 0;
    IC1CONbits.ICI = 1;         // Interrupt on every second capture event
    IC1CONbits.ICM = 3;         // Capture mode : every rising edge
    
    IFS0bits.IC1IF = 0;         // Clear Interrupt Flag
    IEC0SET = (1 << 5);         // Enable Interrupt
    IPC1bits.IC1IP = 2;         // Capture Priority : 2
    
    IC1CONbits.ON = 1;          // Capture ON
    
    //RPM2
    IC2CON = 0;                 // Clear Capture Registers
    IC2BUF = 0;
    IC2CONbits.ICI = 1;         // Interrupt on every second capture event
    IC2CONbits.ICM = 3;         // Capture mode : every rising edge
    
    IFS0bits.IC2IF = 0;         // Clear Interrupt Flag
    IEC0SET = (1 << 9);         // Enable Interrupt
    IPC2bits.IC2IP = 2;         // Capture Priority : 2
    
    IC2CONbits.ON = 1;          // Capture ON
    
    periodo_1 = 0;
    periodo_2 = 0;
    RPM1 = 0;
    RPM2 = 0;
}*/

void captureInit() {
    INTCONbits.INT3EP = 0;
    IPC3bits.INT3IP = 5;
    IFS0CLR = (1 << 15);
    IEC0SET = (1 << 15);
    
    INTCONbits.INT1EP = 1;
    IPC1bits.INT1IP = 5;
    IFS0CLR = (1 << 7);
    IEC0SET = (1 << 7);
    
    INTCONbits.INT2EP = 1;
    IPC2bits.INT2IP = 5;
    IFS0CLR = (1 << 11);
    IEC0SET = (1 << 11);
}

/********************** Funzioni **************************/

/********************** Interrupts **************************/
/*
void __ISR(_INPUT_CAPTURE_1_VECTOR, IPL2AUTO) Capture1InterruptHandler(void) {
    unsigned int T1, T2;
    unsigned int NTick;
    T1 = IC1BUF;
    T2 = IC1BUF;
    IFS0bits.IC1IF = 0;
    if (T2 > T1) NTick = T2 - T1;
    else NTick = (PR3 - T1) + T2;
    periodo_1 = (float)(NTick * 0.0000128);             // 0.0000128 = 1 / TIMER_3_FREQ
    RPM1 = (short)(60 / (periodo_1 * 9));
}

void __ISR(_INPUT_CAPTURE_2_VECTOR, IPL2AUTO) Capture2InterruptHandler(void) {
    unsigned int T1, T2;
    unsigned int NTick;
    T1 = IC2BUF;
    T2 = IC2BUF;
    IFS0bits.IC2IF = 0;
    if (T2 > T1) NTick = T2 - T1;
    else NTick = (PR3 - T1) + T2;
    periodo_2 = (float)(NTick * 0.0000128);             // 0.0000128 = 1 / TIMER_3_FREQ
    RPM2 = (short)(60 / (periodo_2 * 9));
}*/

void __ISR(_EXTERNAL_3_VECTOR, IPL5AUTO) External3InterruptHandler(void) {
    decellerate = 1;
    IFS0CLR = (1 << 15);
}

void __ISR(_EXTERNAL_1_VECTOR, IPL5AUTO) External1InterruptHandler(void) {
    risingCount_1++;
    IFS0CLR = (1 << 7);
}

void __ISR(_EXTERNAL_2_VECTOR, IPL5AUTO) External2InterruptHandler(void) {
    risingCount_2++;
    IFS0CLR = (1 << 11);
}