#define _SUPPRESS_PLIB_WARNING
#include <p32xxxx.h>
#include "EEPROM.h"



void Delay_ee(int n)
   {
   while(n--);
   }

void BIT_START (void)
{
 _SCL_L;
 Delay_ee(DD);
 _SDA_OUT;                /* sda come output    */
 Delay_ee(DD);
 _SDA_H;                  /* sda=1     */
 Delay_ee(DD);
 _SCL_H;                 /* scl=1  */
 Delay_ee(DD);
 _SDA_L;                 /* sda=0     */
 Delay_ee(DD);
 _SCL_L;                 /* scl=0  ( condizione di start)  */
 Delay_ee(DD);
 }

void  BIT_STOP (void)
{

  _SCL_L;
  _SDA_OUT;          /* sda come output    */
 Delay_ee(DD);
 _SDA_L;            /* sda=0     */
 Delay_ee(DD);
 _SCL_H;            /* scl=1  */
 Delay_ee(DD);
 _SDA_H;            /* sda=1     */
 Delay_ee(DD);
 _SCL_L;
 Delay_ee(DD);
_SDA_IN;           /* sda in input  */
Delay_ee(DD);
 }

void GEN_ACK(void)
{
   _SCL_L;
   _SDA_OUT;
   _SDA_L;
   Delay_ee(DD);
   _SCL_H;
   Delay_ee(DD);
   _SCL_L;
   _SDA_IN;
}

int EE_write (unsigned char d)
    {
    int  n;
    _SCL_L;
    _SDA_OUT;
    Delay_ee(DD);
    for(n=7; n>=0; n--)
        {
        if((0x01 & (d>>n)))
            _SDA_H;
        else
            _SDA_L;
        Delay_ee(DD);       // setup time
        _SCL_H;
        Delay_ee(DD);
        _SCL_L;
        Delay_ee(DD);    // HOLD TIME ( puo' essere 0)
        }

    _SDA_IN;                /* sda come input */
    Delay_ee(DD);
    n=1000;

    while(1)                /* attesa ACK  */
        {
        if (_SDA_LOW)        //
            {
            Delay_ee(DD);
            _SCL_H;    /* scl=1      */
            Delay_ee(DD);
            _SCL_L;    /* scl=0      */
            Delay_ee(DD);
            return 0;            // tutto OK
            }
        n--;
        if(n==0)
        break;               // ritorna con errore
        }

  // Errore_EEPROM=1;
   return 1;
    while(1)
        {
        //LATAbits.LATA7^=1;
        delay_ms(50);
        };
    return 1;
    }//  fine funzione


unsigned char  EE_read ()
    {
    unsigned char  Dato,d;
    int   n;
    _SCL_L;
    _SDA_IN;
    for(n=7; n>=0; n--)
        { 
        Dato=Dato << 1;
        Delay_ee(DD);
        _SCL_H;  /* scl=1  */
        Delay_ee(DD);
        if(_SDA_HIGH)
            d=1;
        else
            d=0;
        Dato=Dato | d;

        _SCL_L;
        }// fine FOR

    return(Dato);
    }


/********************************************
          LETTURA n-Byte
********************************************/
int READ_N(int Ind, unsigned char N_Byte, unsigned char *Buff)
   {
    int i;
    BIT_START();
    EE_write(0xA0) ;
    EE_write(Ind>>8);
    EE_write(Ind) ;
    BIT_START();
    EE_write(0xA1);
    i=0;
    while( N_Byte)
    {
    N_Byte--;
    Buff[i]=EE_read();
    if(N_Byte>0)
       GEN_ACK();
    i++;

    }
    BIT_STOP();
    Delay_ee(DD);
    BIT_STOP();
    return 0;
    }



int WRITE_N(int Ind, unsigned char N_Byte, unsigned char *Buff)
   {
    unsigned char i;
    BIT_START();
    EE_write(0xA0);
    EE_write(Ind>>8);
    EE_write(Ind);
    i=0;
    while( N_Byte)
    {
    N_Byte--;
    EE_write(Buff[i]);
    i++;
    }
    BIT_STOP();
    Delay_ee(DD);
    BIT_STOP();
    return 1;
    }
    


char WRITE_DS(unsigned int Ind, unsigned char N_Byte, unsigned char *Buff)
   {
    unsigned char i;
    BIT_START();
    EE_write(0xD0);
    EE_write(Ind);
    i=0;
    while( N_Byte)
    {
    N_Byte--;
    EE_write(Buff[i]);
    i++;
    }
    BIT_STOP();
    Delay_ee(DD);
    BIT_STOP();
    return 1;
    }

char  READ_DS(unsigned int Ind, unsigned char N_Byte, unsigned char *Buff)
   {
    int i;
    BIT_START();
    EE_write(0xD0) ;
    EE_write(Ind) ;
    BIT_START();
    EE_write(0xD0|0x01);
    i=0;
    while( N_Byte)
    {
    N_Byte--;
    Buff[i]=EE_read();
    if(N_Byte>0)
       GEN_ACK();
    i++;

    }
    BIT_STOP();
    Delay_ee(DD);
    BIT_STOP();
    return 0;
    }







