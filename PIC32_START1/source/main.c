/**********************************************************************************
 CONTROLLO SCHEDA LONGBOARD ELETTRICO
***********************************************************************************
Autore              :  Buso Marco
Sistema Sviluppo    :  MPLAB X IDE v3.55
                    :  Compilatore PIC32 vers. 1.34
***********************************************************************************

Versione   ------->   0.1.0.0
Vers. Prec.------->   0.0.9.0
 
***********************************************************************************
Modifiche :     1-  
                2-  
***********************************************************************************/

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING

#include <xc.h>
#include <sys/attribs.h>
#include <plib.h>
#include "GenericTypeDefs.h"

#include "timer.h"
#include "pwm.h"
#include "global_var.c"
#include "global_define.h"
#include "config.h"
#include "uart.h"
#include "adc.h"
#include "EEPROM.h"

#define SYS_FREQ (80000000L)

void boardInit() {  
    // PORTA B
    TRISBbits.TRISB0 = 1;           // Balance port [1]
    TRISBbits.TRISB1 = 1;           // Balance port [2]
    TRISBbits.TRISB2 = 1;           // Balance port [3]
    TRISBbits.TRISB3 = 1;           // Balance port [4]
    TRISBbits.TRISB4 = 1;           // Current sensor
    TRISBbits.TRISB5 = 1;           // Current sensor
    TRISBbits.TRISB6 = 0;           // ICSP [5]
    TRISBbits.TRISB7 = 0;           // ICSP [4]
    TRISBbits.TRISB8 = 1;           // Balance port [1]
    TRISBbits.TRISB9 = 1;           // Balance port [2]
    TRISBbits.TRISB10 = 1;          // Balance port [3]
    TRISBbits.TRISB11 = 1;          // Balance port [4]
    TRISBbits.TRISB12 = 0;          // ---------------
    TRISBbits.TRISB13 = 0;          // ---------------
    TRISBbits.TRISB14 = 0;          // ---------------
    TRISBbits.TRISB15 = 0;          // ---------------
    ODCBSET = 0b0000000000000000;   // Open Collector

    // PORTA C
    TRISCbits.TRISC12 = 0;          // ---------------
    TRISCbits.TRISC13 = 0;          // ---------------
    TRISCbits.TRISC14 = 0;          // ---------------
    TRISCbits.TRISC15 = 0;          // ---------------
    ODCCSET = 0b0000000000000000;   // Open Collector
    
    // PORTA D
    TRISDbits.TRISD0 = 0;           // ESC1
    TRISDbits.TRISD1 = 0;           // ESC2
    TRISDbits.TRISD2 = 1;           // HM-10 [RX]
    TRISDbits.TRISD3 = 0;           // HM-10 [TX]
    TRISDbits.TRISD4 = 0;           // ---------------
    TRISDbits.TRISD5 = 0;           // ---------------
    TRISDbits.TRISD6 = 0;           // ---------------
    TRISDbits.TRISD7 = 0;           // ---------------
    TRISDbits.TRISD8 = 1;           // RPM1
    TRISDbits.TRISD9 = 1;           // RPM2
    TRISDbits.TRISD10 = 1;          // HM-10 [STATE]
    TRISDbits.TRISD11 = 0;          // ---------------
    ODCDSET = 0b1000000000110000;   // Open Collector
    
    // PORTA E
    TRISEbits.TRISE0 = 0;           // LED [VERDE]
    TRISEbits.TRISE1 = 0;           // LED [GIALLO]
    TRISEbits.TRISE2 = 0;           // LED [ROSSO]
    TRISEbits.TRISE3 = 0;           // ---------------
    TRISEbits.TRISE4 = 0;           // ---------------
    TRISEbits.TRISE5 = 0;           // ---------------
    TRISEbits.TRISE6 = 0;           // ---------------
    TRISEbits.TRISE7 = 0;           // ---------------
    ODCESET = 0b0000000000000000;   // Open Collector

     // PORTA F
    TRISFbits.TRISF0 = 0;           // ---------------
    TRISFbits.TRISF1 = 0;           // ---------------
    TRISFbits.TRISF3 = 0;           // ---------------
    TRISFbits.TRISF4 = 0;           // ---------------
    TRISFbits.TRISF5 = 0;           // ---------------
    ODCFSET = 0b0000000000000000;   // Open Collector
   
    // PORTA G
    TRISGbits.TRISG2 = 0;           // ---------------
    TRISGbits.TRISG3 = 0;           // ---------------
    TRISGbits.TRISG6 = 0;           // ---------------
    TRISGbits.TRISG7 = 0;           // ---------------
    TRISGbits.TRISG8 = 0;           // ---------------
    TRISGbits.TRISG9 = 0;           // ---------------
    ODCGSET = 0b0000000000000000;   // Open Collector
}

void variablesInit() {
    LATEbits.LATE0 = 0;                     // Green LED OFF
    LATEbits.LATE1 = 0;                     // Yellow LED OFF
    LATEbits.LATE2 = 0;                     // Red LED OFF
    throttleValue = 0;
    rxTimeout = TIMEOUT_SEC;
    decellerate = 0;
    emergencyStop = 0;
    batteryPercentage = 0;
    STATUS_CODE = WARM_UP;
    updateSettings();
}
 
/********************** MAIN **************************/

void main(void) {
	unsigned int i;
  	i = 1000;
	while(i--);
    
    // Disable USB
    U1CON = 0;
	U1PWRC = 0;
    // Disable JTAG
    DDPCONbits.JTAGEN = 0;
    DDPCONbits.TROEN = 0;
    
    //SYSTEMConfigPerformance(SYS_FREQ);
    INTEnableSystemMultiVectoredInt();      // Enable Interrupts
    
    boardInit();                            // Initialize IN/OUT
	timer1Init();                           // Initialize SW_Timers
    pwmInit();                              // Initialize ESC1/ESC2
    uart1Init();                            // Initialize Bluetooth
    captureInit();                          // Initialize RPM1/RPM2
    adcInit();                              // Initialize ADC
    EEPROMInit();                           // Initialize EEPROM
    
    variablesInit();

    T1CONSET = 0x8000;                      // Start Timer 1
    
    while(1) {
         //--------------  TICK 1 ms -------------------
        if(Timer_Tick(0)) {
        	Clear_Timer_Tick(0);
            
        }

        //--------------  TICK 10 ms -------------------
        if(Timer_Tick(1)) {
        	Clear_Timer_Tick(1);
            WRITE_ESC1(throttleValue);
            WRITE_ESC2(throttleValue);
            checkSystem();
        }
        
        //--------------  TICK 75 ms ------------------
        if(Timer_Tick(2)) {
        	Clear_Timer_Tick(2);
            if(decellerate || emergencyStop) {
                LATEbits.LATE2 = 1;
                if(throttleValue != 0) throttleValue--;
                else {
                    decellerate = 0;
                    emergencyStop = 0;
                    LATEbits.LATE2 = 0;
                }
            }
        }
        
        //--------------  TICK 200 ms ------------------
        if(Timer_Tick(3)) {
            Clear_Timer_Tick(3);

        }
        
        //--------------  TICK 500 ms ------------------
        if(Timer_Tick(4)) {
            Clear_Timer_Tick(4);
            LATEbits.LATE0 ^= 1;
            RPM1 = (unsigned short)(risingCount_1 * 120) / 9;
            RPM2 = (unsigned short)(risingCount_2 * 120) / 9;
            risingCount_1 = 0;
            risingCount_2 = 0;
            updateBatteryPercentage();
            updateBatteryCellsVoltage();
        }
        
        //--------------  TICK 750 ms ------------------
        if(Timer_Tick(6)) {
            Clear_Timer_Tick(6);
            
        }
        
        //--------------  TICK 1 s ---------------------
        if(Timer_Tick(5)) {
            Clear_Timer_Tick(5);
            if(!rxTimeout--) {
                decellerate = 1;
                rxTimeout = TIMEOUT_SEC;
            }
        }
        
        //--------------  TICK 10 s ---------------------
        if(Timer_Tick(8)) {
            Clear_Timer_Tick(8);
            if(STATUS_CODE == WARM_UP) STATUS_CODE = READY;
        }
    }
}

int checkBatteryStatus() {
    if(batteryPercentage == 101) return BATTERY_ERROR;
    else {
        if(battery_1.cell_1 < 3.61 || battery_1.cell_2 < 3.61 || battery_1.cell_3 < 3.61 || battery_1.cell_4 < 3.61) return CELL_ERROR;
        else if(battery_2.cell_1 < 3.61 || battery_2.cell_2 < 3.61 || battery_2.cell_3 < 3.61 || battery_2.cell_4 < 3.61) return CELL_ERROR;
        else return READY;
    }
}

void checkSystem() {
    if(STATUS_CODE == WARM_UP) {
        if(checkBatteryStatus() == READY) STATUS_CODE = NORMAL;
    }
    else {
        if(STATUS_CODE != EEPROM_ERROR) STATUS_CODE = NORMAL;
        STATUS_CODE = (unsigned char)checkBatteryStatus();
    }
}