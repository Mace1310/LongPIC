#ifndef __EEPROM_H
#define __EEPROM_H

#define _SCL_L   LATAbits.LATA2=0
#define _SCL_H   LATAbits.LATA2=1

#define _SDA_L   LATAbits.LATA3=0
#define _SDA_H   LATAbits.LATA3=1

#define _SDA_OUT   TRISAbits.TRISA3=0
#define _SDA_IN    TRISAbits.TRISA3=1
#define _SDA_LOW   PORTAbits.RA3==0
#define _SDA_HIGH  PORTAbits.RA3==1


#define  DD    80
#define _SCL_OUT   TRISAbits.TRISA2=0

/*
#define ADR_IMPOSTAZIONI            64
#define ADR_QUOTE_MACCHINA          128
#define ADR_OFFSET_PINZE            192
#define ADR_PROGRAMMI               1024
#define ADR_MOD_PRELIEVO            2048
#define ADR_MOD_DEPOSITO            10240
#define ADR_NOMI_TABELLE            18432
#define ADR_TABELLE                 19456
*/

void Delay_ee(int n);
void BIT_START(void);
void BIT_STOP(void);
void GEN_ACK(void);
int EE_write(unsigned char d);
unsigned char EE_read(void);
int READ_N(int Ind, unsigned char N_Byte, unsigned char *Buff);
int WRITE_N(int Ind, unsigned char N_Byte, unsigned char *Buff);

#endif

