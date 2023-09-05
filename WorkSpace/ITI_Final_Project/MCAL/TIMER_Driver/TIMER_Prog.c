/*
 * TIMER_Prog.c
 *
 *  Created on: Aug 23, 2023
 *      Author: Hardware
 */
#include<avr/io.h>

#include"../../LIB/BIT_MATH.h"
#include"../../LIB/STD_TYPES.h"

#include"../../MCAL/DIO_Driver/DIO_interface.h"

#include "../TIMER_Driver/TIMER_Interface.h"

//Global Function PTR
void (*TIMER0_Normal_ISR_PTR)(void) = 0x00;
void (*TIMER0_CTC_ISR_PTR)(void) = 0x00;

void (*GLOBAL_ICU_ISR)(void) = 0x00;

u8 Mode_flag=0;

//Prescaller Table
/**		CS22 CS21 CS20 Description

		0 0 0	No clock source (Timer/Counter stopped).
		0 0 1 	clkT2S/(No prescaling)
		0 1 0 	clkT2S/8 (From prescaler)
		0 1 1 	clkT2S/32 (From prescaler)
		1 0 0 	clkT2S/64 (From prescaler)
		1 0 1 	clkT2S/128 (From prescaler)
		1 1 0 	clkT2S/256 (From prescaler)
		1 1 1 	clkT2S/1024 (From prescaler)
  */
void TIMER_Timer0Init(u8 Copy_Mode,u8 Copy_Prescaller){

	//Clearing bit 6 in TCCR0 for both Modes
	CLR_BIT(TCCR0,6);
	switch (Copy_Mode) {
		case NORMAL:
			CLR_BIT(TCCR0,3);
			//Enable OverFlow Interrupt
			SET_BIT(TIMSK,0);
			//Disable CTC Interrupt
			CLR_BIT(TIMSK,1);
			Mode_flag = 0;
			break;
		case CTC:
			SET_BIT(TCCR0,3);
			//Enable CTC Interrupt
			SET_BIT(TIMSK,1);
			//Disable OverFlow Interrupt
			CLR_BIT(TIMSK,0);
			Mode_flag = 1;
			break;
	}
	//Set Prescaller Value
	for(u8 i=0;i<3;i++){
		if((Copy_Prescaller%10)==1)
			SET_BIT(TCCR0,i);
		else
			CLR_BIT(TCCR0,i);
		Copy_Prescaller = Copy_Prescaller /10;
	}
}

void TIMER_Timer0RegValue(u8 Copy_Preload_Compare_Value){
	switch (Mode_flag){
		case 0:
			//Set Preload Value
			TCNT0=Copy_Preload_Compare_Value;
			break;
		case 1:
			//set Compare Match Value
			OCR0 = Copy_Preload_Compare_Value;
	}
}



void TIMER_VoidTimer0ISR_Normal(void(*ISR11_PTR)(void)){
	TIMER0_Normal_ISR_PTR = ISR11_PTR;
}



void TIMER_VoidTimer0ISR_CTC(void(*ISR10_PTR)(void)){
	TIMER0_CTC_ISR_PTR = ISR10_PTR;
}



void TIMER_delay_ms(u32 Copy_delayTime){
	for(u32 count2=0;count2<Copy_delayTime;count2++){
	for(u16 count1=0;count1<350;count1++){}
	}
}

void TIMER_VoidTimer2FastPWM(void){
	//Set Fast PWM Mode
	SET_BIT(TCCR2,6);
	SET_BIT(TCCR2,3);

	//Set Prescaller ==>8
	CLR_BIT(TCCR2,2);
	SET_BIT(TCCR2,1);
	CLR_BIT(TCCR2,0);

	//Normal(Non-Inverted) Mode  Clear on Compare and Set at top
	//Normal Increasing Compare Match, Increase Duty Cycle (Increase Ton)
	SET_BIT(TCCR2,5);
	CLR_BIT(TCCR2,4);

}


void TIMER_VoidTimer2FastPWMCompareValue(u8 Copy_u8CompareMatchValue){
	OCR2=Copy_u8CompareMatchValue;
}


void TIMER_VoidTimer1Init(u8 Copy_Mode, u8 Copy_Prescaller){
	//Set Prescaller Value
	for(u8 i=0;i<3;i++){
		if((Copy_Prescaller%10)==1)
			SET_BIT(TCCR1B,i);
		else
			CLR_BIT(TCCR1B,i);
		Copy_Prescaller = Copy_Prescaller /10;
	}
	switch(Copy_Mode){
		case NORMAL:
			CLR_BIT(TCCR1A,0);
			CLR_BIT(TCCR1A,1);
			CLR_BIT(TCCR1B,3);
			CLR_BIT(TCCR1B,4);
			break;
		case PWM:
			//Set PWM Mode Clear on compare Set at top
			CLR_BIT(TCCR1A,6);
			SET_BIT(TCCR1A,7);

			//Set Mode 14
			CLR_BIT(TCCR1A,0);
			SET_BIT(TCCR1A,1);
			SET_BIT(TCCR1B,3);
			SET_BIT(TCCR1B,4);
			break;
	}

}

void TIMER_VoidSetOverFlow(u16 Copy_u16OverFlow){
	ICR1 = Copy_u16OverFlow;
}

void TIMER_VoidTimer1ChannelASetCompareValue(u16 Copy_u16CompareValue){
	OCR1A = Copy_u16CompareValue;
}

void TIMER_VoidTimer1SetPreloadValue(u16 Copy_u16Value){
	TCNT1 = Copy_u16Value;
}

u16 Timer_u16Timer1GetTimerValue(void){
	return TCNT1;
}

void ICU_VoidSetSenseControl(u8 Copy_u8SenseControl){
	switch(Copy_u8SenseControl){
		case RISING_EDGE:
			SET_BIT(TCCR1B,6);
			break;
		case FALLING_EDGE:
			CLR_BIT(TCCR1B,6);
			break;
	}
}

void ICU_VoidInterruptControl(u8 Copy_u8InterruptState){
	switch(Copy_u8InterruptState){
		case ENABLE:
			SET_BIT(TIMSK,5);
			break;
		case DISABLE:
			CLR_BIT(TIMSK,5);
			break;
	}
}

void ICU_VoidSetCallBackFunction(void(*Copy_PTR_ICUFunction)(void)){
	GLOBAL_ICU_ISR = Copy_PTR_ICUFunction;
}

u16 ICU_u16ReadInputCapture(void){
	return ICR1;
}





//Normal ISR
void __vector_11(void) __attribute__((signal));  //minus 1 from Datasheet as it should started from 0 not 1
void __vector_11(void){
	if(TIMER0_Normal_ISR_PTR!=0x00)
		TIMER0_Normal_ISR_PTR();
}

//CTC ISR
void __vector_10(void) __attribute__((signal));  //minus 1 from Datasheet as it should started from 0 not 1
void __vector_10(void){
	if(TIMER0_CTC_ISR_PTR!=0x00)
		TIMER0_CTC_ISR_PTR();

}

//ICU
void __vector_6 (void) __attribute__((signal));
void __vector_6 (void){
	if(GLOBAL_ICU_ISR!=0x00)
		GLOBAL_ICU_ISR();
}



