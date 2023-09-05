/*
 * TWI_prog.c
 *
 * Created: 8/29/2023 9:25:00 PM
 *  Author: Omar
 */ 
#include <avr/io.h>
#include"../../LIB/BIT_MATH.h"
#include "TWI_interface.h"

void TWI_voidMasterInit(u8 Copy_MasterAddress){
	//Set Baud Rate value ==> 400 Kbps
	/*
	 * SCL = CPU Clock Freq /(16 + 2(TWBR) * (4 ^ TWPS))
	 * TWBR = 2
	 * TWPS = 0
	 */
	TWBR = 150;
	CLR_BIT(TWSR,TWPS1);
	CLR_BIT(TWSR,TWPS0);

	//Set Master Address
	if(Copy_MasterAddress !=0){
		TWAR = Copy_MasterAddress <<1;
	}
	/*enable TWI */
	SET_BIT(TWCR, TWEN);

}

void TWI_voidSlaveInit(u8 Copy_SlaveAddress){
	//Set Slave Address
	TWAR = Copy_SlaveAddress <<1;

	//Enable TWI
	SET_BIT(TWCR,TWEN);
}

TWI_ErrStatus TWI_SendStartCondition(void){
	TWI_ErrStatus 	Local_ErrStatus = NoError;
	//Send Start Condition
	SET_BIT(TWCR,TWSTA);

	//Clear Flag to start previous operation
	SET_BIT(TWCR,TWINT);

	//Wait Until Interrupt Flag is Raised Again and previous operation is completed
	while(!GET_BIT(TWCR,TWINT));

	//Check Status  ,  Status is in bit3 => 7 in  TWSR Reg
	if((TWSR & 0xF8)!=START_ACK){
		Local_ErrStatus = StartConditionErr;
		}
	return Local_ErrStatus;
}

TWI_ErrStatus TWI_SendRepeatedStart(void){
	TWI_ErrStatus 	Local_ErrStatus = NoError;
	//Send Start Condition
	SET_BIT(TWCR,TWSTA);

	//Clear Flag to start previous operation
	SET_BIT(TWCR,TWINT);

	//Wait Until Interrupt Flag is Raised Again and previous operation is completed
	while(!GET_BIT(TWCR,TWINT));

	//Check Status  ,  Status is in bit3 => 7 in  TWSR Reg
	if((TWSR & 0xF8)!=REP_START_ACK){
		Local_ErrStatus = RepeatedStartErr;
		}
	return Local_ErrStatus;
}

TWI_ErrStatus TWI_SendSlaveAddressWithWrite(u8 Copy_SlaveAddress){
	TWI_ErrStatus 	Local_ErrStatus = NoError;

	//Clear Start Condition
	//CLR_BIT(TWCR,TWSTA);

	//Set Slave Address
	TWDR = Copy_SlaveAddress <<1;
	//For Write Operation
	CLR_BIT(TWDR,0);

	//Clear Flag to start previous operation  //Enable TWI
	TWCR=(1<<TWINT)|(1<<TWEN);

	//Wait Until Interrupt Flag is Raised Again and previous operation is completed
	while(!GET_BIT(TWCR,TWINT));

	//Check Status  ,  Status is in bit3 => 7 in  TWSR Reg
	if((TWSR & 0xF8)!=SLAVE_ADD_AND_WR_ACK){
		Local_ErrStatus = SlaveAddressWithWriteErr;
		}
	return Local_ErrStatus;
}

TWI_ErrStatus TWI_SendSlaveAddressWithRead(u8 Copy_SlaveAddress){
	TWI_ErrStatus 	Local_ErrStatus = NoError;

	//Clear Start Condition
	//CLR_BIT(TWCR,TWSTA);

	//Set Slave Address
	TWDR = Copy_SlaveAddress <<1;
	//For Read Operation
	SET_BIT(TWDR,0);

	//Clear Flag to start previous operation
	TWCR=(1<<TWINT)|(1<<TWEN);

	//Wait Until Interrupt Flag is Raised Again and previous operation is completed
	while(!GET_BIT(TWCR,TWINT));

	//Check Status  ,  Status is in bit3 => 7 in  TWSR Reg
	if((TWSR & 0xF8)!=SLAVE_ADD_AND_RD_ACK){
		Local_ErrStatus = SlaveAddressWithReadErr;
		}
	return Local_ErrStatus;
}

TWI_ErrStatus TWI_MasterWriteDataByte(u8 Copy_u8DataByte){
	TWI_ErrStatus 	Local_ErrStatus = NoError;

	//Write Data to data byte
	TWDR = Copy_u8DataByte;

	//Clear Flag to start previous operation
	SET_BIT(TWCR,TWINT);

	//Wait Until Interrupt Flag is Raised Again and previous operation is completed
	while(!GET_BIT(TWCR,TWINT));

	//Check Status  ,  Status is in bit3 => 7 in  TWSR Reg
	if((TWSR & 0xF8)!=MasterWriteDataByteErr){
		Local_ErrStatus = MasterWriteDataByteErr;
		}
	return Local_ErrStatus;

}

TWI_ErrStatus TWI_MasterReadDataByte(u8* Copy_Pu8DataByte)
{
	TWI_ErrStatus 	Local_ErrStatus = NoError;

	//Enable ACK
	SET_BIT(TWCR,TWEA);
	//Clear Flag to start previous operation
	SET_BIT(TWCR,TWINT);

	//Wait Until Interrupt Flag is Raised Again and previous operation is completed
	while(!GET_BIT(TWCR,TWINT));

	//Check Status  ,  Status is in bit3 => 7 in  TWSR Reg
	if((TWSR & 0xF8)!=MSTR_RD_BYTE_WITH_ACK){
		Local_ErrStatus = MasterReadDataByteErr;
		}
	else{
		// Read Data
		*Copy_Pu8DataByte = TWDR;

	}
	return Local_ErrStatus;
}

void TWI_SendStopCondition(void)
{
    /* generate stop condition on the bus /

    / Clear the interrupt flag to start the previous operation */
    TWCR = (1<<TWINT)|(1<<TWEN)|
    (1<<TWSTO);
}
