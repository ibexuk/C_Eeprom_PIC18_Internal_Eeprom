
######################################################################
##### Open sourced by IBEX, an electronic product design company #####
#####    based in South East England.  http://www.ibexuk.com     #####
######################################################################

Many Microchip PIC18 devices incorporate eeprom non-volatile memory.  This driver provides functions to read and write the eeprom memory with checkum error detection.


This driver provides the following functions:


CHECK FOR FACTORY START

BYTE check_factory_start (void)

Returns 1 if this is a factory start (eeprom signature is not present), 0 if not


STORE FACTORY START SIGNATURE

void store_factory_start_signature (void)


After carying out factory start tasks, such as storing default eeprom values, call this fucntion to store the factory start doen signature in eeprom.


READ PIC EEPROM

BYTE read_pic_eeprom (WORD eeprom_start_address, BYTE *ram_start_address, BYTE len, BYTE checksum_offset)

Returns 0 if checkum was OK, 1 if it failed (the ram_start_address buffer will have been set to 0×00)
If no checksum is used set checksum_offset to 0.


WRITE PIC EEPROM

void write_pic_eeprom (WORD eeprom_start_address, BYTE *ram_start_address, BYTE len, BYTE checksum_offset)

To not store a checksum set checksum_offset to 0.


Please view the sample source code files for full information regarding the driver.