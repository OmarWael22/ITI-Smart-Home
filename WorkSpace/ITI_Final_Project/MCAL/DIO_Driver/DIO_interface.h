/*
 * DIO_interface.h
 *
 * Created: 8/20/2023 3:29:24 PM
 *  Author: Omar
 */ 

#ifndef _DIO_INTERFACE_H_
#define _DIO_INTERFACE_H_

#include "..\..\LIB\BIT_MATH.h"
#include "..\..\LIB\STD_TYPES.h"

#define DIO_PORTA   0
#define DIO_PORTB   1
#define DIO_PORTC   2
#define DIO_PORTD   3


#define DIO_PIN0    0
#define DIO_PIN1    1
#define DIO_PIN2    2
#define DIO_PIN3    3
#define DIO_PIN4    4
#define DIO_PIN5    5
#define DIO_PIN6    6
#define DIO_PIN7    7

#define DIO_OUTPUT   1
#define DIO_INPUT    0

#define DIO_HIGH     1
#define DIO_LOW      0

#define DIO_OUTPUT_PORT 	0xFF
#define DIO_INPUT_PORT 		0




//convention
// return type ModuleName_returntypeFunctionname(datatype copy_datatypeName)


void DIO_voidSetPinDirection(u8 Copy_u8PortName ,u8 Copy_u8PinNumber ,u8 Copy_u8PinState );

void DIO_voidSetPinValue(u8 Copy_u8PortName ,u8 Copy_u8PinNumber ,u8 Copy_u8PinValue );

u8 DIO_u8GetPinValue(u8 Copy_u8PortName ,u8 Copy_u8PinNumber);

void DIO_voidSetPortDirection(u8 Copy_u8PortName ,u8 Copy_u8PortValue );

void DIO_voidSetPortValue(u8 Copy_u8PortName  ,u8 Copy_u8PortValue );

u8 DIO_u8GetPortValue(u8 Copy_u8PortName );



#endif /* DIO_INTERFACE_H_ */


//convention
// return type ModuleName_returntypeFunctionname(datatype copy_datatypeName)