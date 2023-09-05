/*
 * EPROM_interface.h
 *
 * Created: 8/29/2023 11:25:02 PM
 *  Author: Omar
 */ 


#ifndef EPROM_INTERFACE_H_
#define EPROM_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"

#define GND	0
#define VCC 1
//Set this Macro to VCC or GND
#define A2	GND

void EEPROM_voidSendDataByte(u8 Copy_u8DataByte, u16 Copy_u16LocationAdd);

void EEPROM_voidReadDataByte(u8* Copy_u8DataByte, u16 Copy_u16LocationAdd);



#endif /* EPROM_INTERFACE_H_ */