#ifndef __TRANSMISSIONS_H
#define __TRANSMISSIONS_H

void setESC(unsigned char arg);
void setMode(unsigned char arg);
void getRPM();
void getBatteryPercentage();
void getCellVoltages();
void checkSystem();

#endif