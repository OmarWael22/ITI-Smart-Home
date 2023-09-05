/*
 * EPROM_prog.c
 *
 * Created: 8/29/2023 11:24:40 PM
 *  Author: Omar
 */ 
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/TWI_Driver/TWI_interface.h"
#include "EPROM_interface.h"

void EEPROM_voidSendDataByte(u8 Copy_u8DataByte,u16 Copy_u16LocationAddress)
{	u8 Address = ((0b1010<<3) | (A2<<2) | (Copy_u16LocationAddress>>8));

	TWI_SendStartCondition();
	TWI_SendSlaveAddressWithWrite(Address);
	//TWI_SendSlaveAddressWithWrite(0b1010000);/**/

	TWI_MasterWriteDataByte((u8)Copy_u16LocationAddress);
	TWI_MasterWriteDataByte(Copy_u8DataByte);
	TWI_SendStopCondition();
}

void EEPROM_voidReadDataByte(u8 *Copy_u8DataByte,u16 Copy_u16LocationAddress)
{	u8 Address = ((0b1010<<3) | (A2<<2) | (Copy_u16LocationAddress>>8));
	TWI_SendStartCondition();
	TWI_SendSlaveAddressWithWrite(Address);
	//TWI_SendSlaveAddressWithWrite(0b1010000);

	TWI_MasterWriteDataByte((u8)Copy_u16LocationAddress);

	TWI_SendRepeatedStart();

	//TWI_MasterWriteDataByte(0b10100001);/**/
	TWI_SendSlaveAddressWithRead(Address);
	TWI_MasterReadDataByte(Copy_u8DataByte);
	TWI_SendStopCondition();
}