/*
IBEX UK LTD http://www.ibexuk.com
Electronic Product Design Specialists
RELEASED SOFTWARE

The MIT License (MIT)

Copyright (c) 2013, IBEX UK Ltd, http://ibexuk.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//Project Name:		MICROCHIP PIC18 INTERNAL EEPROM DRIVER





//*************************************
//*************************************
//********** DRIVER REVISION **********
//*************************************
//*************************************
//
//V1.00
//- Original release



//*********************************
//*********************************
//********** USAGE NOTES **********
//*********************************
//*********************************

//Include this header file in any .c files within your project from which you wish to use it's functions.


//############################
//############################
//########## TO USE ##########
//############################
//############################

//##### MEMORY DEFINITIONS FOR C18 COMPILER #####
/*
//Non volatile variables
struct NON_VOLATILE_STRUCT
{
	BYTE nv_value0;
	BYTE spare[19];
} non_volatile;



//Non volatile variables
extern struct NON_VOLATILE_STRUCT
{
	BYTE nv_value0;
	BYTE spare[19];
} non_volatile;
*/


//##### ADD TO APPLICATION INITIALISE FUNCTION #####
/*
	//-----------------------------------
	//----- CHECK FOR FACTORY START -----
	//-----------------------------------
	if (check_factory_start())
	{
		non_volatile.nv_value0 = 0;
		non_volatile.spare[0] = 0;
		//...
		non_volatile.spare[18] = 0;
		write_pic_eeprom(EEPROM_NON_VOLATILE_MEMORY, &non_volatile.nv_value0, EEPROM_NON_VOLATILE_MEMORY_LEN, EEPROM_NON_VOLATILE_MEMORY_OFFSET);

		store_factory_start_signature();
	}

	//----- READ NON VOLATILE -----
	read_pic_eeprom(EEPROM_NON_VOLATILE_MEMORY, &non_volatile.nv_value0, EEPROM_NON_VOLATILE_MEMORY_LEN, EEPROM_NON_VOLATILE_MEMORY_OFFSET);
*/


//##### ADD TO HEARTBEAT TIMER #####
	//---------------------------
	//----- HERE EVERY 10mS -----
	//---------------------------
	
	//----- DO NV MEMORY STORE TIMER -----






//*****************************
//*****************************
//********** DEFINES **********
//*****************************
//*****************************
#ifndef PIC_EEPROM_C_INIT		//Do only once the first time this file is used
#define	PIC_EEPROM_C_INIT


#define	FACTORY_START_SIG_0_VALUE	0x55
#define	FACTORY_START_SIG_1_VALUE	0xaa

//#define	EEPROM_16_BIT_ADDRESSING		//Comment out for devices with only 8 bits of eeprom address


//--------------------------------------
//--------------------------------------
//----- INTERNAL EEPROM ALLOCATION -----
//--------------------------------------
//--------------------------------------
//256 bytes of eeprom memory.  0x00 - 0xFF
//1024 bytes of eeprom memory.  0x0000 - 0x03FF

#define	EEPROM_FACTORY_START_SIG			(WORD) 0x0000
#define	EEPROM_FACTORY_START_SIG_LEN		2				//(Excluding checksum if used)

#define	EEPROM_NON_VOLATILE_MEMORY			(WORD) 0x0002
#define	EEPROM_NON_VOLATILE_MEMORY_LEN		20				//(Excluding checksum if used)
#define	EEPROM_NON_VOLATILE_MEMORY_OFFSET	1				//(0 = no checksum)


#endif


//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef PIC_EEPROM_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------



//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)
BYTE check_factory_start (void);
void store_factory_start_signature (void);
BYTE read_pic_eeprom (WORD eeprom_start_address, BYTE *ram_start_address, BYTE len, BYTE checksum_offset);
void write_pic_eeprom (WORD eeprom_start_address, BYTE *ram_start_address, BYTE len, BYTE checksum_offset);


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------
extern BYTE check_factory_start (void);
extern void store_factory_start_signature (void);
extern BYTE read_pic_eeprom (WORD eeprom_start_address, BYTE *ram_start_address, BYTE len, BYTE checksum_offset);
extern void write_pic_eeprom (WORD eeprom_start_address, BYTE *ram_start_address, BYTE len, BYTE checksum_offset);


#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef PIC_EEPROM_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------



//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)



#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------



#endif






