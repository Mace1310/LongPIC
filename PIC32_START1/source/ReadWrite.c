#include <plib.h>
#include "SD\FSIO.H"
#include "eeprom.h"
extern char STRINGHE[64][32];
extern char NOMI_ERRORI[32][32];
extern char NOMI_ALLARMI[32][32];

FSFILE * pfile;

FSFILE * pfiletext;




 short int TestFile(void)
    {
    short int i;
    unsigned int pp;

    if (pfile == NULL)
        return 0;
        
    pp=100000;
    i=FSfseek(pfile, pp, SEEK_SET);

    if(i==0)
        return 1;    // tutto OK
    else
        return 0;    //
    }


short int AggiornaFile(void)
    {
   // long dimFILE;
    short int i;
    unsigned int pp;
    char dato=2;
    char BufferVuoto[64]={
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                         };

    FSFILE * pfile1;
    pfile1 = FSfopen ("dati.bin", "a+");

    FSfseek(pfile1, 0, SEEK_END);
   // dimFILE=FSftell(pfile1);
   // if(dimFILE<59391)
   //     {
   //     PutText(5,20, "Upgrade .....   ");
        for(i=0;i<642;i++)
            {
            // scrive  da 0 a 63 --> 64 byte vuoti
            if(FSfwrite( &BufferVuoto, sizeof(BufferVuoto), 1, pfile1)!=1)
                while(1);
            }
        FSfclose (pfile1);
     //   PutText(5,20, "Upgrade ..... OK");
        
     //   }
   // else
   //    FSfclose (pfile1);

   }


void creafile(void)
    {
    pfile = FSfopen ("xxx.bin", "w");
    }



short int OpenFile(void)
    {
    if (pfile != NULL)
        return 1;
    pfile = FSfopen ("dati.bin", "a+");
    if (pfile == NULL)
        return 0;
    else
        {
        return 1;
        }
    }

short int CloseFile(void)
    {
    FSfclose (pfile);
        return 1;
    }

short int OpenFileText(void)
    {
    if (pfiletext != NULL)
        return 1;
    pfiletext = FSfopen ("note.txt", "r");
    if (pfiletext == NULL)
        return 0;
    else
        return 1;
    }


short int READ_DATI(unsigned short int Ind, unsigned short int N_Byte, unsigned char *Buff)
    {
    unsigned char i;
    
  //  pfile = FSfopen ("dati.bin", "a+");
    if (pfile == NULL)
        return 0;
    FSfseek(pfile, Ind, SEEK_SET);
    FSfread(Buff,N_Byte,1,pfile);
   // FSfclose (pfile);
    return 1;
    }


short int WRITE_DATI(unsigned short int Ind, unsigned short int  N_Byte, unsigned char *Buff)
   {
    unsigned char i;

    FSfseek(pfile, Ind, SEEK_SET);
    if(FSfwrite( Buff, N_Byte, 1, pfile)!=1)
        {
        return 0;
        }
    return 1;
    }
    


short int CARICA_LINGUA( unsigned char ln)
    {
    unsigned char i;
    unsigned char j;
    unsigned char car,car_MSB;
    unsigned char UNICODE;

    switch(ln)
        {
        case 0:
            pfiletext = FSfopen ("IT_LS.TXT", "r");
            break;
        case 1:
            pfiletext = FSfopen ("EN_LS.TXT", "r");
            break;
        case 2:
            pfiletext = FSfopen ("DE_LS.TXT", "r");
            break;
        case 3:
            pfiletext = FSfopen ("FR_LS.TXT", "r");
            break;

        case 4:
            pfiletext = FSfopen ("ES_LS.TXT", "r");
            break;

        case 5:
            pfiletext = FSfopen ("RU_LS.TXT", "r");
            break;


        default:
            pfiletext=0;
            break;
        }

   	if (pfiletext == NULL)
        return 1;          // file lingua non trovato



    // posiziona inizio file
    FSfseek(pfiletext,0, SEEK_SET);
    FSfread(&car,1,1,pfiletext);
    FSfread(&car_MSB,1,1,pfiletext);
    if(car==0xFF && car_MSB==0xFE)
        UNICODE=1;
    else
        {
        UNICODE=0;
        FSfseek(pfiletext,0, SEEK_SET);
        }

	Nop();
	Nop();
   
    // lettura 80 stringhe
	for(i=0;i<80;i++)
        {
        j=0;
        while(1)
            {
            if(UNICODE==0)
                {
                FSfread(&car,1,1,pfiletext);
			    if(car==0x0D)
                    {
                    STRINGHE[i][j]=0; // termina stringa
                    FSfread(&car,1,1,pfiletext); // legge carattere return
                    break;
                    }
                else
                    {STRINGHE[i][j]=car; j++;}
                }
            else
                {
                FSfread(&car,1,1,pfiletext);
                FSfread(&car_MSB,1,1,pfiletext);
			    if(car==0x0D && car_MSB==0)  // controlla se codice CR
                    {
                    STRINGHE[i][j]=0; // termina stringa
                    FSfread(&car,1,1,pfiletext); // legge carattere LF
                    FSfread(&car,1,1,pfiletext); // legge carattere LF MSB
                    break;
                    }
                else
                    {
                    if(car_MSB==0)
                        STRINGHE[i][j]=car;
                    else
                        STRINGHE[i][j]=car+127;
                    j++;
                    }
                }
            }
    
        }

     Nop();
     Nop();

    // lettura 32 stringhe per NOMI ERRORI
   	for(i=0;i<32;i++)
        {
        j=0;
         while(1)
            {
            if(UNICODE==0)
                {
                FSfread(&car,1,1,pfiletext);
			    if(car==0x0D)
                    {
                    NOMI_ERRORI[i][j]=0; // termina stringa
                    FSfread(&car,1,1,pfiletext); // legge carattere return
                    break;
                    }
                else
                    {NOMI_ERRORI[i][j]=car; j++;}
                }
            else
                {
                FSfread(&car,1,1,pfiletext);
                FSfread(&car_MSB,1,1,pfiletext);
			    if(car==0x0D && car_MSB==0)  // controlla se codice CR
                    {
                    NOMI_ERRORI[i][j]=0; // termina stringa
                    FSfread(&car,1,1,pfiletext); // legge carattere LF
                    FSfread(&car,1,1,pfiletext); // legge carattere LF MSB
                    break;
                    }
                else
                    {
                    if(car_MSB==0)
                        NOMI_ERRORI[i][j]=car;
                    else
                        NOMI_ERRORI[i][j]=car+127;
                    j++;
                    }
                }
            }
    
        }


    Nop();
    Nop();
     // lettura 32 stringhe per ALLARMI
   	for(i=0;i<32;i++)
        {
        j=0;
        while(1)
            {
            if(UNICODE==0)
                {
                FSfread(&car,1,1,pfiletext);
			    if(car==0x0D)
                    {
                    NOMI_ALLARMI[i][j]=0; // termina stringa
                    FSfread(&car,1,1,pfiletext); // legge carattere return
                    break;
                    }
                else
                    {NOMI_ALLARMI[i][j]=car; j++;}
                }
            else
                {
                FSfread(&car,1,1,pfiletext);
                FSfread(&car_MSB,1,1,pfiletext);
			    if(car==0x0D && car_MSB==0)  // controlla se codice CR
                    {
                    NOMI_ALLARMI[i][j]=0; // termina stringa
                    FSfread(&car,1,1,pfiletext); // legge carattere LF
                    FSfread(&car,1,1,pfiletext); // legge carattere LF MSB
                    break;
                    }
                else
                    {
                    if(car_MSB==0)
                        NOMI_ALLARMI[i][j]=car;
                    else
                        NOMI_ALLARMI[i][j]=car+127;
                    j++;
                    }
                }
            }
    
        
        
        }

    Nop();
    Nop();

    FSfclose (pfiletext);
    return 0;
    }
