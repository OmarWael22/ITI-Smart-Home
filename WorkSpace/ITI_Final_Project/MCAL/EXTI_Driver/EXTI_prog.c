/*
 * EXTI_prog.c
 *
 * Created: 8/21/2023 4:06:58 PM
 *  Author: Omar
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include"../../LIB/STD_TYPES.h"
#include"../../LIB/BIT_MATH.h"
#include "EXTI_interface.h"

void(*Global_pvINT0NotifyFunc)(void)=NULL_PTR;
void(*Global_pvINT1NotifyFunc)(void)=NULL_PTR;
void(*Global_pvINT2NotifyFunc)(void)=NULL_PTR;

void EXTI_voidInit(u8 Copy_u8InterruptNumber , u8 Copy_u8SourceType){
	/*
	*1- set source type
	* 2- set the INT pin 
	* 3- enable PIE
	* 4- enable GIE
	*/	
	u8 Local_u8ISC0;
	u8 Local_u8ISC1;
	u8 Local_u8PIE;
	
	switch (Copy_u8InterruptNumber){
		
		case EXTI_INT0:
			Local_u8ISC0=0;
			Local_u8ISC1=1;
			Local_u8PIE=6;
			break;
		case EXTI_INT1:
			Local_u8ISC0=2;
			Local_u8ISC1=3;
			Local_u8PIE=7;
			break;
		case EXTI_INT2:
			Local_u8PIE=5;
			switch(Copy_u8SourceType){
				case EXTI_RAISNG_EDGE:
					SET_BIT(MCUCSR,6);
					break;
				case EXTI_FALLING_EDGE:
					CLR_BIT(MCUCSR,6);
					break;
					
				SET_BIT(GICR,Local_u8PIE);
				
				return;
				
			}
	}
	
	switch (Copy_u8SourceType){
		case EXTI_LOW_LEVEL:
		CLR_BIT(MCUCR,Local_u8ISC0);
		CLR_BIT(MCUCR,Local_u8ISC1);
		break;
		
		case EXTI_ANY_LOGIC:
		SET_BIT(MCUCR,Local_u8ISC0);
		CLR_BIT(MCUCR,Local_u8ISC1);
		break;
		
		case EXTI_FALLING_EDGE:
		CLR_BIT(MCUCR,Local_u8ISC0);
		SET_BIT(MCUCR,Local_u8ISC1);
		break;
		
		case EXTI_RAISNG_EDGE:
		SET_BIT(MCUCR,Local_u8ISC0);
		SET_BIT(MCUCR,Local_u8ISC1);
		break;
	}
		SET_BIT(GICR,Local_u8PIE);
	
	
}

void EXTI_voidSetCallBackINT0(void(*NotifyFunc0)(void)){
	
	Global_pvINT0NotifyFunc=NotifyFunc0;
	
}

void EXTI_voidSetCallBackINT1(void(*NotifyFunc1)(void)){
	
	Global_pvINT1NotifyFunc=NotifyFunc1;
}

void EXTI_voidSetCallBackINT2(void(*NotifyFunc2)(void)){
	
	Global_pvINT2NotifyFunc=NotifyFunc2;
}

ISR(INT0_vect){
	
	if(Global_pvINT0NotifyFunc!=NULL_PTR)
			Global_pvINT0NotifyFunc();
	
}

ISR(INT1_vect){
	
	if(Global_pvINT1NotifyFunc!=NULL_PTR)
			Global_pvINT1NotifyFunc();
	
}
ISR(INT2_vect){
	
	if(Global_pvINT2NotifyFunc!=NULL_PTR)
			Global_pvINT2NotifyFunc();
	
}