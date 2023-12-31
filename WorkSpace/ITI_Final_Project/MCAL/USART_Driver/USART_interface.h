/*
 * USART_interface.h
 *
 * Created: 8/27/2023 8:02:41 PM
 *  Author: Omar
 */ 


#ifndef USART_INTERFACE_H_
#define USART_INTERFACE_H_
#include "../../LIB/STD_TYPES.h"


#define TIMEOUTVALUE	40000
typedef enum{
	CorrectReception,
	TimeOUT_Occured,
}TimeOut;
void USART_voidInit(void);

void USART_voidTransmitData(u8 Copy_u8Data);

u8 USART_u8ReceiverData(void);

TimeOut USART_u8TimeOUTReceiverData(u8* Copy_u8PTRReceivingData);

void USART_voidTransmitString(u8* Copy_String);

void USART_voidCallBackFunction(void(*PTR_CallBackFunc)(void));

void USART_voidEnableRxINT(void);

void USART_voidDisableRxINT(void);

#endif /* USART_INTERFACE_H_ */
