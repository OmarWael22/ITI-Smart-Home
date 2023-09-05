/*
 * USART_prog.c
 *
 * Created: 8/27/2023 8:02:21 PM
 *  Author: Omar
 */ 

#include<avr/io.h>
#include"USART_interface.h"
#include "../../LIB/BIT_MATH.h"

void USART_voidInit(void){
	
	u8 Local_u8UCSRC=0;
	//select ucsrc reg 
	SET_BIT(Local_u8UCSRC,7);
	// async mode
	CLR_BIT(Local_u8UCSRC,6);
	//disable parity
	CLR_BIT(Local_u8UCSRC,5);
	CLR_BIT(Local_u8UCSRC,4);
	// 1 stop bit 
	CLR_BIT(Local_u8UCSRC,3);
	// select 8 bit size
	SET_BIT(Local_u8UCSRC,2);
	SET_BIT(Local_u8UCSRC,1);
	CLR_BIT(UCSRB,2);
	
	UCSRC=Local_u8UCSRC;
	// set baud rate
	UBRRL=51;
	
	//enable trans
	SET_BIT(UCSRB,3);
	
	//enable recev
	SET_BIT(UCSRB,4);
}

void USART_voidTransmitData(u8 Copy_u8Data){
	
	// wait untill transmit buffer is empty
	while(GET_BIT(UCSRA,5)==0);
	//clear flag
	SET_BIT(UCSRA,5);
	
	UDR=Copy_u8Data;
	
	
}

u8 USART_u8ReceiverData(void){
	
	while(GET_BIT(UCSRA,7)==0);
	CLR_BIT(UCSRA,7);
	
	u8 LocalRxData=UDR;
	return LocalRxData;
	
}

void USART_voidTransmitString(u8* Copy_String){
	u8 Local_idx=0;
	while(Copy_String[Local_idx] != '\0'){
		USART_voidTransmitData(Copy_String[Local_idx]);
		Local_idx ++;
	}
}
