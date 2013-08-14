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



#include "main.h"					//Global data type definitions (see https://github.com/ibexuk/C_Generic_Header_File )
#define PIC_EEPROM_C
#include "pic-eeprom.h"



//*********************************************
//*********************************************
//********** CHECK FOR FACTORY START **********
//*********************************************
//*********************************************
//Returns 1 if factory start, 0 if not
BYTE check_factory_start (void)
{
	BYTE temp[2];
	
	read_pic_eeprom (EEPROM_FACTORY_START_SIG, &temp[0], EEPROM_FACTORY_START_SIG_LEN, 0);
	if ((temp[0] == FACTORY_START_SIG_0_VALUE) && (temp[1] == FACTORY_START_SIG_1_VALUE))
		return (0);
	else
		return (1);
}



//**************************************************
//**************************************************
//********** STORE FACTORY START SIGNATURE**********
//**************************************************
//**************************************************
void store_factory_start_signature (void)
{
	BYTE temp[2];

	temp[0] = FACTORY_START_SIG_0_VALUE;
	temp[1] = FACTORY_START_SIG_1_VALUE;

	write_pic_eeprom (EEPROM_FACTORY_START_SIG, &temp[0], EEPROM_FACTORY_START_SIG_LEN, 0);
}



//*************************************
//*************************************
//********** READ PIC EEPROM **********
//*************************************
//*************************************
//Returns 0 if checkum OK, 1 if failed (ram_start_address buffer will have been set to 0x00)
BYTE read_pic_eeprom (WORD eeprom_start_address, BYTE *ram_start_address, BYTE len, BYTE checksum_offset) 
{
	WORD checksum;
	BYTE count;
	BYTE *ram_start_address_copy;
	ram_start_address_copy = ram_start_address;

	//Write the address to eeadr
	EEADR = (eeprom_start_address & 0x00ff);		//(n.b. silicon bug for some devices - don't do directly before setting eecon1,rd)
	#ifdef EEPROM_16_BIT_ADDRESSING
		EEADRH = (eeprom_start_address >> 8);
	#endif

	checksum = (WORD) checksum_offset;


	//Set control bits
	EECON1bits.EEPGD = 0;							//Point to eeprom not program flash memory
	EECON1bits.CFGS = 0;							//Access eeprom registers, not configuration registers

	//Get the data from eeprom
	for (count = 0; count < len; count++)
	{
		EECON1bits.RD = 1;
		Nop();

		*ram_start_address++ = EEDATA;
		checksum += EEDATA;

		EEADR++;									//(n.b. silicon bug - don't do directly before setting eecon1,rd)
		#ifdef EEPROM_16_BIT_ADDRESSING
			if (EEADR == 0)
				EEADRH++;
		#endif
	}

	//Exit if not doing checksum bytes
	if (checksum_offset == 0)
		return (0);

	//----- DO THE CHECKSUM -----
	//Get and check checksum h
	EECON1bits.RD = 1;
	Nop();

	if (EEDATA != (BYTE) (checksum >> 8))
		goto read_pic_eeprom_fail;

	//Get and check checksum l
	EEADR++;										//(n.b. silicon bug - don't do directly before setting eecon1,rd)
	#ifdef EEPROM_16_BIT_ADDRESSING
		if (EEADR == 0)
			EEADRH++;
	#endif

	EECON1bits.RD = 1;
	Nop();

	if (EEDATA != (BYTE) (checksum & 0x00ff))
		goto read_pic_eeprom_fail;


	return (0);										//Checksum good

read_pic_eeprom_fail:
	//----- BAD CHECKSUM -----
	//Set all bytes to 0x00
	for (count = 0; count < len; count++)
	{
		*ram_start_address_copy++ = 0;
	}

	return (1);
}




//**************************************
//**************************************
//********** WRITE PIC EEPROM **********
//**************************************
//**************************************
void write_pic_eeprom (WORD eeprom_start_address, BYTE *ram_start_address, BYTE len, BYTE checksum_offset)
{
	WORD checksum;
	BYTE count;

	//Write the address to eeadr
	EEADR = (eeprom_start_address & 0x00ff);		//(n.b. silicon bug - don't do directly before setting eecon1,rd)
	#ifdef EEPROM_16_BIT_ADDRESSING
		EEADRH = (eeprom_start_address >> 8);
	#endif

	checksum = (WORD) checksum_offset;


	//Set control bits
	EECON1bits.EEPGD = 0;							//Point to eeprom not program flash memory
	EECON1bits.CFGS = 0;							//Access eeprom registers, not configuration registers
	EECON1bits.WREN = 1;							//Enable eeprom writes


	//Write the data to eeprom
	for (count = 0; count < len; count++)
	{
		EEDATA = *ram_start_address++;
		checksum += EEDATA;
		
		PIR2bits.EEIF = 0;							//Clear write complete flag

		DISABLE_INT;
		EECON2 = 0x55;								//Do the write enable sequence
		EECON2 = 0xaa;
		EECON1bits.WR = 1;							//Start write
		ENABLE_INT;

		while (PIR2bits.EEIF == 0);					//Wait for write to complete (do it above as faster)

		EEADR++;									//Increment address
		#ifdef EEPROM_16_BIT_ADDRESSING
			if (EEADR == 0)
				EEADRH++;
		#endif
	}



	//Exit if not doing checksum bytes
	if (checksum_offset == 0)
	{
		EECON1bits.WREN = 0;						//Disable eeprom writes
		return;
	}

	//----- DO THE CHECKSUM -----
	//Write checksum h
	EEDATA = (BYTE) (checksum >> 8);

	PIR2bits.EEIF = 0;								//Clear write complete flag

	DISABLE_INT;
	EECON2 = 0x55;									//Do the write enable sequence
	EECON2 = 0xaa;
	EECON1bits.WR = 1;								//Start write
	ENABLE_INT;

	while (PIR2bits.EEIF == 0);						//Wait for write to complete (do it above as faster)

	EEADR++;										//Increment address
	#ifdef EEPROM_16_BIT_ADDRESSING
		if (EEADR == 0)
			EEADRH++;
	#endif

	//Write checksum l
	EEDATA = (BYTE) (checksum & 0x00ff);

	PIR2bits.EEIF = 0;								//Clear write complete flag

	DISABLE_INT;
	EECON2 = 0x55;									//Do the write enable sequence
	EECON2 = 0xaa;
	EECON1bits.WR = 1;								//Start write
	ENABLE_INT;

	while (PIR2bits.EEIF == 0);						//Wait for write to complete (do it above as faster)

	EECON1bits.WREN = 0;							//Disable eeprom writes
	return;											//Checksum good
}
































