#include <xc.h>

unsigned char throttleValue;
unsigned char rxTimeout;

unsigned char decellerate;
unsigned char emergencyStop;

unsigned int risingCount_1;
unsigned int risingCount_2;
unsigned short RPM1;
unsigned short RPM2;

unsigned char txBuffer[100];
unsigned int txLength;

struct LipoBattery {
    float cell_1;
    float cell_2;
    float cell_3;
    float cell_4;
};
struct LipoBattery battery_1;
struct LipoBattery battery_2;
unsigned char batteryPercentage;

float current_sensor_1;
float current_sensor_2;

float strain_gauge_1;
float strain_gauge_2;

unsigned char STATUS_CODE;
int MODE_CODE;

char BoardName[20];
int BoardNameLength;