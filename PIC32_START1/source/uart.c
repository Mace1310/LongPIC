#include <xc.h>
#include <sys/attribs.h>

#include "global_var.c"
#include "global_define.h"
#include "transmissions.h"

#define BAUDRATE    9600
#define PBCLK       20000000
#define BRGVAL      (PBCLK / (16 * BAUDRATE)) - 1

#define STX         0x02
#define ETX         0x03
#define DLE         0x10

unsigned char txBufferDLE[100];
unsigned char rxBufferDLE[100];
unsigned char rxBuffer[100];
unsigned int rxManagerState = 0;
unsigned int rxCounter = 0;
int i = 0;

/********************** Inizzializzazioni **************************/

void uart1Init(void) {
    U1MODE = 0;                     // Clear UART 1 Register
	U1MODEbits.UEN = 0;             // Enable U1TX and U1RX pins
 	U1STAbits.UTXEN = 1;            // Enable TX Transmission
    U1STAbits.URXEN = 1;            // Enable RX Transmission
    U1STAbits.UTXISEL = 2;          // Enable TX Interrupt
    
    U1BRG = BRGVAL;                 // Set BaudRate

	U1MODESET = (1 << 15);          // UART 1 ON
 
    IFS0CLR = (1 << 27);            // Clear the RX interrupt Flag
	IEC0SET = (1 << 27);            // Enable RX Interrupt
 	IPC6bits.U1IP = 5;              // UART 1 Priority : 5
}

/********************** Funzioni **************************/

void writeByte(unsigned char var) {
    U1TXREG = var;
}

void flushTxBuffer() {
    txBufferDLE[0] = STX;
    applyDLE();
    txBufferDLE[txLength] = ETX;
    txLength++;
    
    i = 0;
    fillTxReg();
}

void applyDLE() {
    int t = 1;
    int i = 0;
    for (i = 0; i < txLength; i++) {
        if(txBuffer[i] != STX && txBuffer[i] != ETX && txBuffer[i] != DLE) txBufferDLE[t] = txBuffer[i];
        else if(txBuffer[i] == STX) { 
            txBufferDLE[t] = DLE; 
            t++; 
            txBufferDLE[t] = 'A';
        } else if(txBuffer[i] == ETX) {
            txBufferDLE[t] = DLE; 
            t++; 
            txBufferDLE[t] = 'B';
        } else if(txBuffer[i] == DLE) {
            txBufferDLE[t] = DLE;
            t++;
            txBufferDLE[t] = 'C';
        }
        t++;
    }
    txLength = t;
}

void fillTxReg() {
    while(txLength) {
        U1TXREG = txBufferDLE[i];
        txLength--;
        i++;
        if(U1STAbits.UTXBF) break;
    }
}

void rxManager(unsigned char inByte) {
    switch(rxManagerState) {
        case 0:
            if(inByte == STX) {
                rxManagerState = 1;
                rxCounter = 0;
            }
            break;
        case 1:
            if(inByte == ETX) {
                removeDLE();
                analyzeRxBuffer();
                rxManagerState = 0;
            }
            else if(inByte == STX) rxCounter = 0;
            else {
                rxBufferDLE[rxCounter] = inByte;
                rxCounter++;  
            }
            break;
    }
}

void removeDLE() {
    int t = 0;
    int i = 0;
    for(i = 0; i < rxCounter; i++) {
        if(rxBufferDLE[t] != DLE) rxBuffer[i] = rxBufferDLE[t];
        else {
            t++;
            rxCounter--;
            if(rxBufferDLE[t] == 'A') rxBuffer[i] = STX;
            else if(rxBufferDLE[t] == 'B') rxBuffer[i] = ETX;
            else if(rxBufferDLE[t] == 'C') rxBuffer[i] = DLE;
        }
        t++;
    }
}

void analyzeRxBuffer() {
    char args[21];
    switch(rxBuffer[0]) {
        case 0x00:
            if(rxCounter == 2) setESC(rxBuffer[1]);
            break;
        case 0x01:
            if(rxCounter == 2) setMode(rxBuffer[1]);
            break;
        case 0x02:
            memcpy(&args, &rxBuffer[1], rxBuffer[1] + 1);
            setBoardName(args);
            break;
        case 0x03:
            emergencyStopON();
            break;
        case 0x10:
            getRPM();
            break;
        case 0x11:
            getBatteryPercentage();
            break;
        case 0x12:
            getCellVoltages(rxBuffer[1]);
            break;
        case 0x13:
            getCurrentValues();
            break;
        case 0x14:
            getBoardName();
            break;
        case 0x15:
            getSystemStatus();
            break;
        case 0x16:
            getMode();
            break;
    }
}

/********************** Interrupts **************************/

void __ISR(_UART_1_VECTOR, IPL5AUTO) UART1InterruptHandler(void) {
	if(IFS0 & (1 << 27)) {                              // Check if it is an RX Interrupt                
        //throttleValue = U1RXREG;
        rxManager(U1RXREG);
        rxTimeout = TIMEOUT_SEC;
        decellerate = 0;
        LATEbits.LATE2 = 0;
    	IFS0CLR = (1 << 27);                            // Clear the RX interrupt Flag
	}

	if(IFS0 & (1 << 28)) {                              // Check if it is a TX Interrupt
        while(U1STAbits.TRMT == 0);                     // Wait until the shift register is empty
        
        fillTxReg();
        
        IFS0CLR = (1 << 28);                            // Clear the TX interrupt Flag
    }

}