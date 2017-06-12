#pragma config FNOSC    = FRCPLL        // Oscillator Selection
#pragma config FPLLMUL  = MUL_20        // PLL Multiplier XXX
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_4         // Peripheral Clock divisor

#pragma config FCANIO   =  OFF         // Peripheral Clock divisor

#pragma config UPLLEN   = ON            // USB PLL Enabled
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider

#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config POSCMOD  = HS            // Primary Oscillator

#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config IESO     = ON            // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect

#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select

#pragma config DEBUG    = ON            // Background Debugger Disable