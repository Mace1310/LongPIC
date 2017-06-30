#include <xc.h>
#include <sys/attribs.h>

#include "global_var.c"
#include "global_define.h"

#define VFS 3.3
#define MAX_BUF 1023
#define NO_CURRENT_1 2.484
#define NO_CURRENT_2 2.487

struct ADC1_BUFFER {
    int BUF0;
    int BUF1;
    int BUF2;
    int BUF3;
    int BUF4;
    int BUF5;
    int BUF6;
    int BUF7;
    int BUF8;
    int BUF9;
    int BUFA;
    int BUFB;
    int BUFC;
    int BUFD;
    int BUFE;
    int BUFF;
} ADC1;

struct LipoCell {
    float Voltage;
    float Percentage;
} CellState[] = {
    3.27, 0,
    3.61, 5,
    3.69, 10,
    3.71, 15,
    3.73, 20,
    3.75, 25,
    3.77, 30,
    3.79, 35,
    3.80, 40,
    3.82, 45,
    3.84, 50,
    3.85, 55,
    3.87, 60,
    3.91, 65,
    3.95, 70,
    3.98, 75,
    4.02, 80,
    4.08, 85,
    4.11, 90,
    4.15, 95,
    4.20, 100
}; 

struct LipoBattery battery1_new;
struct LipoBattery battery2_new;

float balancePort[8];

/********************** Inizzializzazione **************************/

void adcInit() {
    AD1PCFG = 0;                // Set all ANx pins to analog input
    AD1CHSbits.CH0NA = 0;       // Mux A negative voltage reference: VREFL(GND)
    AD1CON1bits.FORM = 0;       // Output mode: 16 bit Integer
    AD1CON1bits.SSRC = 7;       // Conversion mode: Auto
    AD1CON2bits.VCFG = 0;       // Voltage reference: +AVdd(3.3V); -AVdd(GND)
    AD1CON2bits.CSCNA = 1;      // Enable Scan Mode
    AD1CSSLbits.CSSL = 0x0F3F;  // Scan from pin AN0 to pin AN11
    AD1CON2bits.SMPI = 10;      // Generate interrupt after 10 conversions
    AD1CON2bits.BUFM = 0;       // Buffer fill mode: single 16-bit word
    AD1CON3bits.ADRC = 0;       // Clock derived from Peripheral Bus Clock (PBCLK)
    AD1CON3bits.ADCS = 0xFF;    // Prescaler value: 512
    AD1CON3bits.SAMC = 0b11111; // Sample Frequency: every 31 TAD
    
    AD1CON1bits.ON = 1;
    
    IFS1CLR = (1 << 1);         // Clear AD Interrupt Flag
    IEC1SET = (1 << 1);         // Enable AD Interrupt
    IPC6bits.AD1IP = 5;         // Interrupt Priority: 5
    
    AD1CON1bits.ASAM = 1;       // Start converting
}

/********************** Funzioni **************************/

void readADC1BUFx() {
    ADC1.BUF0 = ADC1BUF0;
    ADC1.BUF1 = ADC1BUF1;
    ADC1.BUF2 = ADC1BUF2;
    ADC1.BUF3 = ADC1BUF3;
    ADC1.BUF4 = ADC1BUF4;
    ADC1.BUF5 = ADC1BUF5;
    ADC1.BUF6 = ADC1BUF6;
    ADC1.BUF7 = ADC1BUF7;
    ADC1.BUF8 = ADC1BUF8;
    ADC1.BUF9 = ADC1BUF9;
    ADC1.BUFA = ADC1BUFA;
    ADC1.BUFB = ADC1BUFB;
    ADC1.BUFC = ADC1BUFC;
    ADC1.BUFD = ADC1BUFD;
    ADC1.BUFE = ADC1BUFE;
    ADC1.BUFF = ADC1BUFF;
}

float cellVoltageToPercentage(float cellVoltage) {
    int BatteryStateLength = sizeof(CellState) / sizeof(CellState[0]);
    if(cellVoltage < CellState[0].Voltage || cellVoltage > CellState[BatteryStateLength - 1].Voltage) return 101;
    else {
        int i = 0;
        for(i = 0; i < BatteryStateLength; i++) {
            if(cellVoltage == CellState[i].Voltage) return CellState[i].Percentage;
            else if(cellVoltage < CellState[i].Voltage) {
                float deltaVoltage = CellState[i].Voltage - CellState[i - 1].Voltage;
                float deltaPercentage = CellState[i].Percentage - CellState[i - 1].Percentage;
                float deltaCellVoltage = CellState[i].Voltage - cellVoltage;
                float deltaCellPercentage = (deltaPercentage * deltaCellVoltage) / deltaVoltage;
                return CellState[i].Percentage - deltaCellPercentage;
            }
        }
    }
}

void updateBatteryPercentage() {
    float battery_1_cells_sum = battery_1.cell_1 + battery_1.cell_2 + battery_1.cell_3 + battery_1.cell_4;
    float battery_2_cells_sum = battery_2.cell_1 + battery_2.cell_2 + battery_2.cell_3 + battery_2.cell_4;
    float battery_cells_average = (battery_1_cells_sum + battery_2_cells_sum) / 8;
    float batteryPercentage_new = (unsigned char)cellVoltageToPercentage(battery_cells_average);
    if (batteryPercentage_new == 101) batteryPercentage = batteryPercentage_new;
    else batteryPercentage = batteryPercentage * K + batteryPercentage_new * (1 - K);
}

void updateBatteryCellsVoltage() {
    battery_1.cell_1 = battery_1.cell_1 * K + battery1_new.cell_1 * (1 - K);
    battery_1.cell_2 = battery_1.cell_2 * K + battery1_new.cell_2 * (1 - K);
    battery_1.cell_3 = battery_1.cell_3 * K + battery1_new.cell_3 * (1 - K);
    battery_1.cell_4 = battery_1.cell_4 * K + battery1_new.cell_4 * (1 - K);
    battery_2.cell_1 = battery_2.cell_1 * K + battery2_new.cell_1 * (1 - K);
    battery_2.cell_2 = battery_2.cell_2 * K + battery2_new.cell_2 * (1 - K);
    battery_2.cell_3 = battery_2.cell_3 * K + battery2_new.cell_3 * (1 - K);
    battery_2.cell_4 = battery_2.cell_4 * K + battery2_new.cell_4 * (1 - K);
}

/********************** Interrupts **************************/

void __ISR(_ADC_VECTOR, IPL5AUTO) ADCInterruptHandler(void) {
    readADC1BUFx();
    
    balancePort[0] = (ADC1.BUF3 * VFS) / (MAX_BUF * 0.7124390244);
    balancePort[1] = (ADC1.BUF2 * VFS) / (MAX_BUF * 0.3530487805);
    balancePort[2] = (ADC1.BUF1 * VFS) / (MAX_BUF * 0.228699187);
    balancePort[3] = (ADC1.BUF0 * VFS) / (MAX_BUF * 0.1746186699);
    balancePort[4] = (ADC1.BUF9 * VFS) / (MAX_BUF * 0.1497512438);
    balancePort[5] = (ADC1.BUF8 * VFS) / (MAX_BUF * 0.1265560166);
    balancePort[6] = (ADC1.BUF7 * VFS) / (MAX_BUF * 0.1074468085);
    balancePort[7] = (ADC1.BUF6 * VFS) / (MAX_BUF * 0.09037267081);
    
    battery1_new.cell_1 = balancePort[0];
    battery1_new.cell_2 = balancePort[1] - balancePort[0];
    battery1_new.cell_3 = balancePort[2] - balancePort[1];
    battery1_new.cell_4 = balancePort[3] - balancePort[2];
    
    battery2_new.cell_1 = balancePort[4] - balancePort[3];
    battery2_new.cell_2 = balancePort[5] - balancePort[4];
    battery2_new.cell_3 = balancePort[6] - balancePort[5];
    battery2_new.cell_4 = balancePort[7] - balancePort[6];

    current_sensor_1 = ((ADC1.BUF4 * VFS) / (MAX_BUF * 0.644149577) - NO_CURRENT_1) / 0.02;
    current_sensor_2 = ((ADC1.BUF5 * VFS) / (MAX_BUF * 0.6418810289) - NO_CURRENT_2) / 0.02;
    if(current_sensor_1 > -1 && current_sensor_1 < 1) current_sensor_1 = 0;
    if(current_sensor_2 > -1 && current_sensor_2 < 1) current_sensor_2 = 0;
    
    IFS1CLR = (1 << 1);         // Clear AD Interrupt Flag
}