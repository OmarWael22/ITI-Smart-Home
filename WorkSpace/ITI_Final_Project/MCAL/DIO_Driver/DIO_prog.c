/*
 * DIO_prog.c
 *
 * Created: 8/20/2023 3:29:02 PM
 *  Author: Omar
 */ 

/*
*
* 1- include libraries
* 2- include self layers ( drivers from same layer )
* 3- include lower layers
* 4-include self files (like .h file )
*
*/

#include <avr/io.h>


#include"DIO_interface.h"


void DIO_voidSetPinDirection(u8 Copy_u8PortName ,u8 Copy_u8PinNumber ,u8 Copy_u8PinState ){

	volatile u8* Local_u8PTR_DDRx;
	switch(Copy_u8PortName){

		case DIO_PORTA:
		Local_u8PTR_DDRx=&DDRA;
		break;
		case DIO_PORTB:
		Local_u8PTR_DDRx=&DDRB;
		break;
		case DIO_PORTC:
		Local_u8PTR_DDRx=&DDRC;
		break;
		case DIO_PORTD:
		Local_u8PTR_DDRx=&DDRD;
		break;
	}

	switch(Copy_u8PinState){

		case DIO_OUTPUT:
		SET_BIT(*Local_u8PTR_DDRx,Copy_u8PinNumber);
		break;
		case DIO_INPUT:
		CLR_BIT(*Local_u8PTR_DDRx,Copy_u8PinNumber);
		break;

	}

}
void DIO_voidSetPinValue(u8 Copy_u8PortName ,u8 Copy_u8PinNumber ,u8 Copy_u8PinValue ){

	volatile u8* Local_u8PTR_PORTx;
	switch(Copy_u8PortName){

		case DIO_PORTA:
		Local_u8PTR_PORTx=&PORTA;
		break;
		case DIO_PORTB:
		Local_u8PTR_PORTx=&PORTB;
		break;
		case DIO_PORTC:
		Local_u8PTR_PORTx=&PORTC;
		break;
		case DIO_PORTD:
		Local_u8PTR_PORTx=&PORTD;
		break;
	}

	switch(Copy_u8PinValue){

		case DIO_HIGH:
		SET_BIT(*Local_u8PTR_PORTx,Copy_u8PinNumber);
		break;
		case DIO_LOW:
		CLR_BIT(*Local_u8PTR_PORTx,Copy_u8PinNumber);
		break;

	}



}

void DIO_voidSetPortDirection(u8 Copy_u8PortName ,u8 Copy_u8PortValue ){

	switch(Copy_u8PortName){

		case DIO_PORTA:
		DDRA=Copy_u8PortValue;
		break;
		case DIO_PORTB:
		DDRB=Copy_u8PortValue;
		break;
		case DIO_PORTC:
		DDRC=Copy_u8PortValue;
		break;
		case DIO_PORTD:
		DDRD=Copy_u8PortValue;
		break;
	}

}
void DIO_voidSetPortValue(u8 Copy_u8PortName  ,u8 Copy_u8PortValue ){

	switch(Copy_u8PortName){

		case DIO_PORTA:
		PORTA=Copy_u8PortValue;
		break;
		case DIO_PORTB:
		PORTB=Copy_u8PortValue;
		break;
		case DIO_PORTC:
		PORTC=Copy_u8PortValue;
		break;
		case DIO_PORTD:
		PORTD=Copy_u8PortValue;
		break;
	}
}

u8 DIO_u8GetPinValue(u8 Copy_u8PortName ,u8 Copy_u8PinNumber){

	
	u8 Local_u8PinValue;
	switch(Copy_u8PortName){

		case DIO_PORTA:
		Local_u8PinValue=GET_BIT(PINA,Copy_u8PinNumber);
		break;
		case DIO_PORTB:
		Local_u8PinValue=GET_BIT(PINB,Copy_u8PinNumber);
		break;
		case DIO_PORTC:
		Local_u8PinValue=GET_BIT(PINC,Copy_u8PinNumber);
		break;
		case DIO_PORTD:
		Local_u8PinValue=GET_BIT(PIND,Copy_u8PinNumber);
		break;
	}

	

	return Local_u8PinValue;

}

u8 DIO_u8GetPortValue(u8 Copy_u8PortName ){

	u8 Local_u8PortVal;
	switch(Copy_u8PortName){

		case DIO_PORTA:
		Local_u8PortVal=PINA;
		break;
		case DIO_PORTB:
		Local_u8PortVal=PINB;
		break;
		case DIO_PORTC:
		Local_u8PortVal=PINC;
		break;
		case DIO_PORTD:
		Local_u8PortVal=PIND;
		break;
	}
	return Local_u8PortVal;
}
