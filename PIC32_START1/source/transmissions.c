#include <xc.h>

#include "global_var.c"
#include "EEPROM.h"

void setESC(unsigned char arg) {
    if (!emergencyStop) throttleValue = arg;
}

void setMode(unsigned char arg) {
    MODE_CODE = arg;
    saveSettings();
}

void emergencyStopON() {
    emergencyStop = 1;
}

void getRPM() {
    txBuffer[0] = 0x10;
    memcpy(&txBuffer[1], &RPM1, sizeof(RPM1));
    memcpy(&txBuffer[3], &RPM2, sizeof(RPM2));
    txLength = 5;
    flushTxBuffer();
}

void getBatteryPercentage() {
    txBuffer[0] = 0x11;
    txBuffer[1] = batteryPercentage;
    txLength = 2;
    flushTxBuffer();
}

void getCellVoltages(unsigned char arg) {
    txBuffer[0] = 0x12;
    txBuffer[1] = arg;
    switch(arg) {
        case 0x01:
            memcpy(&txBuffer[2], &battery_1.cell_1, sizeof(battery_1.cell_1));
            break;
        case 0x02:
            memcpy(&txBuffer[2], &battery_1.cell_2, sizeof(battery_1.cell_2));
            break;
        case 0x03:
            memcpy(&txBuffer[2], &battery_1.cell_3, sizeof(battery_1.cell_3));
            break;
        case 0x04:
            memcpy(&txBuffer[2], &battery_1.cell_4, sizeof(battery_1.cell_4));
            break;
        case 0x05:
            memcpy(&txBuffer[2], &battery_2.cell_1, sizeof(battery_2.cell_1));
            break;
        case 0x06:
            memcpy(&txBuffer[2], &battery_2.cell_2, sizeof(battery_2.cell_2));
            break;
        case 0x07:
            memcpy(&txBuffer[2], &battery_2.cell_3, sizeof(battery_2.cell_3));
            break;
        case 0x08:
            memcpy(&txBuffer[2], &battery_2.cell_4, sizeof(battery_2.cell_4));
            break;
    }
    txLength = 6;
    flushTxBuffer();
}

void getCurrentValues() {
    txBuffer[0] = 0x13;
    
    memcpy(&txBuffer[1], &current_sensor_1, sizeof(current_sensor_1));
    memcpy(&txBuffer[5], &current_sensor_2, sizeof(current_sensor_2));
    txLength = 9;
    flushTxBuffer();
}

void getBoardName() {
    txBuffer[0] = 0x14;
    memcpy(&txBuffer[1], &BoardName, sizeof(BoardName));
    txLength = BoardNameLength + 1;
    flushTxBuffer();
}

void setBoardName(char args[]) {
    BoardNameLength = args[0];
    memcpy(&BoardName, &args[1], BoardNameLength);
    saveSettings();
}

void getSystemStatus() {
    txBuffer[0] = 0x15;
    txBuffer[1] = STATUS_CODE;
    txLength = 2;
    flushTxBuffer();
}

void getMode() {
    txBuffer[0] = 0x16;
    txBuffer[1] = (unsigned char)MODE_CODE;   
    txLength = 2;
    flushTxBuffer();
}