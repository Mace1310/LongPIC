#include <xc.h>

#include "dee_emulation_pic32.h"
#include "global_var.c"
#include "global_define.h"

/********************** Inizzializzazione **************************/

void EEPROMInit() {
    if(DataEEInit()) STATUS_CODE = EEPROM_ERROR;
}

/********************** Funzioni **************************/

void saveSettings() {
    DataEEWriteArray(BoardName, 0, 20);
    DataEEWrite(BoardNameLength, 20);
    DataEEWrite(MODE_CODE, 21);
}

void updateSettings() {
    DataEEReadArray(BoardName, 0, 20);
    DataEERead(&BoardNameLength, 20);
    if(BoardName[0] == 0x00) {
        strcpy(BoardName, "MY BOARD");
        BoardNameLength = 8;
    }
    DataEERead(&MODE_CODE, 21);
}