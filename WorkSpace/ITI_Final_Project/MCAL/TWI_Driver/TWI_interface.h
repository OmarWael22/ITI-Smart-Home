/*
 * TWI_interface.h
 *
 * Created: 8/29/2023 9:25:26 PM
 *  Author: Omar
 */ 


#ifndef TWI_INTERFACE_H_
#define TWI_INTERFACE_H_

#include"../../LIB/STD_TYPES.h"
// Action Status
#define START_ACK        			0x08
#define REP_START_ACK    			0x10
#define SLAVE_ADD_AND_WR_ACK  		0x18
#define MSTR_WR_BYTE_ACK  			0x28
#define MSTR_RD_BYTE_WITH_NACK  	0x58
#define MSTR_RD_BYTE_WITH_ACK   	0x50
#define SLAVE_ADD_AND_RD_ACK    	0x40

typedef enum{
	NoError,
	StartConditionErr,
	RepeatedStartErr,
	SlaveAddressWithWriteErr,
	SlaveAddressWithReadErr,
	MasterWriteDataByteErr,
	MasterReadDataByteErr,
}TWI_ErrStatus;

//Pass Zero if Master will not be addressed
void TWI_voidMasterInit(u8 Copy_MasterAddress);

void TWI_voidSlaveInit(u8 Copy_SlaveAddress);

TWI_ErrStatus TWI_SendStartCondition(void);

TWI_ErrStatus TWI_SendRepeatedStart(void);

TWI_ErrStatus TWI_SendSlaveAddressWithWrite(u8 Copy_SlaveAddress);

TWI_ErrStatus TWI_SendSlaveAddressWithRead(u8 Copy_SlaveAddress);

TWI_ErrStatus TWI_MasterWriteDataByte(u8 Copy_u8DataByte);

TWI_ErrStatus TWI_MasterReadDataByte(u8* Copy_Pu8DataByte);  //Pointer to save data in

void TWI_SendStopCondition(void);





#endif /* TWI_INTERFACE_H_ */