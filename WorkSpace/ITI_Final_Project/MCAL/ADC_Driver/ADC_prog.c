/*
 * ADC_prog.c
 *
 * Created: 8/22/2023 6:15:23 PM
 *  Author: Omar
 */ 
#include <avr/io.h>

#include"../../LIB/BIT_MATH.h"
#include"../../LIB/STD_TYPES.h"

#include"ADC_interface.h"

void ADC_voidInit(){
	/*
	1- set ref voltage
	2- 8bit res --> left adjustment
	3- prescaler
	4- enable ADC
	*/
	// use AVCC
	SET_BIT(ADMUX,6);
	CLR_BIT(ADMUX,7);
	// left adjust
	SET_BIT(ADMUX,5);
	// prescaler  --> /64
	CLR_BIT(ADCSRA,0);
	SET_BIT(ADCSRA,1);
	SET_BIT(ADCSRA,2);
	// enable 
	SET_BIT(ADCSRA,7);
	
}

u8 ADC_u8StartConversion(u8 Copy_u8ChannelNumber){
	// channel select
	ADMUX &= 0b11100000;
	ADMUX |=Copy_u8ChannelNumber;
	
	// start conversion
	SET_BIT(ADCSRA,6);
	
	// wait until conversion complete
	// by checking the flag
	while(GET_BIT(ADCSRA,4)==0);
	
	//clear flag 
	SET_BIT(ADCSRA,4);
	
	return ADCH;
}
