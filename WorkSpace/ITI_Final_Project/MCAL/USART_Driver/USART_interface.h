/*
 * USART_interface.h
 *
 * Created: 8/27/2023 8:02:41 PM
 *  Author: Omar
 */ 


#ifndef USART_INTERFACE_H_
#define USART_INTERFACE_H_
#include "../../LIB/STD_TYPES.h"
void USART_voidInit(void);

void USART_voidTransmitData(u8 Copy_u8Data);

u8 USART_u8ReceiverData(void);

void USART_voidTransmitString(u8* Copy_String);

#endif /* USART_INTERFACE_H_ */