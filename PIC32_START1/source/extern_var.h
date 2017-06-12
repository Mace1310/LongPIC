 extern union {
    struct {
        unsigned char U00:1;
        unsigned char U01:1;
        unsigned char U02:1;
        unsigned char U03:1;
        unsigned char U04:1;
        unsigned char U05:1;
        unsigned char U06:1;
        unsigned char U07:1;
        unsigned char U08:1;
        unsigned char U09:1;
        unsigned char U10:1;
        unsigned char U11:1;
        unsigned char NONE:4;
    } bits;
    unsigned short int word;
} Uscite;

extern unsigned short int Ing;
extern unsigned short int Ing_old;
extern unsigned short int Ing_FRH;
extern unsigned short int Ing_FRL;

extern struct {
    unsigned char rx_over:1;
    unsigned char NONE:7;
    unsigned char Trama_OK;
    unsigned char rx_num_car;
    unsigned char rx_buffer[32];
    unsigned char rx_stato;
    unsigned char tx_buffer[32];
    unsigned char * tx_msg;
    unsigned char tx_CNT_read;
    unsigned char tx_CNT_write;

    unsigned char msg_fase;
    unsigned char msg_TENTATIVI;
    unsigned char msg_NUM_TENTATIVI;
    unsigned char msg_TIMEOUT;
    unsigned char msg_START;
    unsigned char msg_CMD;
    } com1;