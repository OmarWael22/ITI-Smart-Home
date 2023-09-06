/*
 * USART_prog.c
 *
 * Created: 8/27/2023 8:02:21 PM
 *  Author: Omar
 */ 

#include<avr/io.h>
#include"USART_interface.h"
#include "../../LIB/BIT_MATH.h"
#include"../../LIB/STD_TYPES.h"

void (*GLOBAL_PTR_CallBack)(void) = NULL_PTR;

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
	
	u8 LocalRxData=UDR;
	return LocalRxData;
	
}

TimeOut USART_u8TimeOUTReceiverData(u8* Copy_u8PTRReceivingData){
	/* Initialize Timer to make sure there's no stuck
		 * Making sure it will Work Correctly
		 * 1- SET TCNT1 with 0 before initializing timer
		 * 2- SET Comparing Value to be 40000 ====> 5.12 sec (IF: 8Mhz) "time waiting before time out"
		 * 3- Initialize Timer with 1024 Prescaler
		 */
		//1
		TCNT1=0;
		//2 Timer1 Normal
		CLR_BIT(TCCR1A,0);
		CLR_BIT(TCCR1A,1);
		CLR_BIT(TCCR1B,3);
		CLR_BIT(TCCR1B,4);
		//3 Timer1 Prescaler 1024
		SET_BIT(TCCR1B,0);
		CLR_BIT(TCCR1B,1);
		SET_BIT(TCCR1B,2);
	while(GET_BIT(UCSRA,7)==0){
		if(TCNT1>=TIMEOUTVALUE){
			SET_BIT(UCSRA,7);
			TCCR1B &= 0xF8;
			return TimeOUT_Occured;
		}
	}
	//Initialize timer with 0 prescaler to stop it
	*Copy_u8PTRReceivingData = UDR;
	TCNT1=0;
	return CorrectReception;


}

void USART_voidTransmitString(u8* Copy_String){
	u8 Local_idx=0;
	while(Copy_String[Local_idx] != '\0'){
		USART_voidTransmitData(Copy_String[Local_idx]);
		Local_idx ++;
	}
}

void USART_voidCallBackFunction(void(*PTR_CallBackFunc)(void)){
	GLOBAL_PTR_CallBack = PTR_CallBackFunc;
}

void USART_voidEnableRxINT(void){
	SET_BIT(UCSRB,7);
}

void USART_voidDisableRxINT(void){
	CLR_BIT(UCSRB,7);
}

void __vector_13 (void) __attribute__((signal));
void __vector_13 (void){
	if(GLOBAL_PTR_CallBack!=0x00)
		GLOBAL_PTR_CallBack();
}


