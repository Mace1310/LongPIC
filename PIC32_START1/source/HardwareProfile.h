/*********************************************************************
 *
 *	Hardware specific definitions
 *
 *********************************************************************
 * FileName:        HardwareProfile_xxx.h
 * Dependencies:    None
 * Processor:       PIC24F, PIC24H, dsPIC, PIC32
 * Compiler:        Microchip C32 v1.00 or higher
 *	                 Microchip C30 v3.01 or higher
 * Company:         Microchip Technology, Inc.
 *
 *
 *
 * Author               Date		Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Howard Schlunder		10/03/06	Original, copied from Compiler.h
 * Jayanth Murthy       06/25/09    dsPIC & PIC24H support 
 * Pradeep Budagutta	15 Sep 2009 Added PIC24FJ256DA210 Development Board Support
 ********************************************************************/

/*
******************************************************************************
* Revision:
* This file modified for Solomon SSD1963 driver development
* Programmer: John Leung
* Company: TechToys Co.
* Web: www.TechToys.com.hk
* Date: 16th Jan 2010
* (1) Add 	support for the hardware platform PIC24/32 Eval Rev2C with
*			PIC32MX360F512L MCU onboard + SSD1963 Eval Rev2A.
*			TFT panels can be TY700TFT800480 (7" TFT), TY430TFT480272 (4.3" TFT),
*			LVC75Z779V1S (3.5" TFT) with #define in this file
******************************************************************************
*/

#ifndef __HARDWARE_PROFILE_H
    #define __HARDWARE_PROFILE_H

/*********************************************************************
* GetSystemClock() returns system clock frequency.
* GetPeripheralClock() returns peripheral clock frequency.
* GetInstructionClock() returns instruction clock frequency.
********************************************************************/

// PIC32MX processor
#define	GetSystemClock()			(80000000ul)         // 8 MHz/2 x 20 PLL
#define	GetPeripheralClock()		(GetSystemClock()/4)
#define	GetInstructionClock()	(GetSystemClock())



/*********************************************************************
* START OF GRAPHICS RELATED MACROS
********************************************************************/

#define USE_16BIT_PMP

#define COLOR_DEPTH 16


/*********************************************************************
* DISPLAY SETTINGS 
********************************************************************/

	/*********************************************************************
	* Overview: Image orientation (can be 0, 90, 180, 270 degrees).
	*********************************************************************/	
  	#define DISP_ORIENTATION    0
	/*********************************************************************
	* Overview: Panel Data Width (R,G,B) in (5,6,5)
	*********************************************************************/
	#define DISP_DATA_WIDTH                 16
	/*********************************************************************
	* Overview: Horizontal and vertical display resolution
	*                  (from the glass datasheet).
	*********************************************************************/
	#define DISP_HOR_RESOLUTION 640
	#define DISP_VER_RESOLUTION 480
	/*********************************************************************
	* Overview: Horizontal synchronization timing in pixels
	*                  (from the glass datasheet).
	*********************************************************************/
	#define DISP_HOR_PULSE_WIDTH		1
	#define DISP_HOR_BACK_PORCH			210
	#define DISP_HOR_FRONT_PORCH		45
	/*********************************************************************
	* Overview: Vertical synchronization timing in lines
	*                  (from the glass datasheet).
	*********************************************************************/
	#define DISP_VER_PULSE_WIDTH		1
	#define DISP_VER_BACK_PORCH			34
	#define DISP_VER_FRONT_PORCH		10
	/*********************************************************************
	* Definition for SPI interface for SSD1963 
	* Hardware dependent!
	*********************************************************************/
	#define GPIO3 3
	#define GPIO2 2
	#define GPIO1 1
	#define GPIO0 0
	#define LCD_RESET 	0
	#define LCD_SPENA	0
	#define LCD_SPCLK	0
	#define LCD_SPDAT	0
	/* End of definition for DISPLAY_PANEL == TY700TFT800480 */



/*********************************************************************
* IOS FOR THE DISPLAY CONTROLLER
*********************************************************************/

	/*
	*****************************************************************************
	* Use 8080 system interface. PMP for PIC32MX series & PIC24 series
	* The mother board is PIC24/32-Eval RevC2
	*
	* Function		mcu pins		LCD pins
	* ==========	========		========
	* TE			RG15			TE
	* DATA LINES	PMD[15:0]		D[15:0] in 16-bit addressing
	* RESET			RA7				/RESET
	* CHIP SELECT	RD11/PMCS1		/CS
	* COMMAND/DATA	RA6				RS 
	* WR STROBE		RD4/PMPWR		/WR
	* RD STROBE		RD5/PMPRD		/RD
	*
	*****************************************************************************
	*/
	//Definitions for TE pin of SSD1963
	#define TE_TRIS_BIT	   	TRISGbits.TRISG15
	#define TE_PORT_BIT	   	PORTGbits.RG15

	// Definitions for reset pin
	#define RST_TRIS_BIT       TRISGbits.TRISG14
	#define RST_LAT_BIT        LATGbits.LATG14


	//Definition for RS pin
	#define RS_TRIS_BIT			TRISGbits.TRISG12
	#define RS_LAT_BIT			LATGbits.LATG12

	// Definitions for CS pin
	#define CS_TRIS_BIT        TRISGbits.TRISG13
	#define CS_LAT_BIT         LATGbits.LATG13

	// Definition for WR pin
	#define WR_TRIS_BIT		   TRISDbits.TRISD4
	#define WR_LAT_BIT		   LATDbits.LATD4
	
	// Definition for RD pin
	#define RD_TRIS_BIT		   TRISDbits.TRISD5
	#define RD_LAT_BIT		   LATDbits.LATD5

	//Definition for RS pin
    #define DC_TRIS_BIT      	TRISGbits.TRISG12
    #define DC_LAT_BIT			    LATGbits.LATG12   



#endif // __HARDWARE_PROFILE_H

