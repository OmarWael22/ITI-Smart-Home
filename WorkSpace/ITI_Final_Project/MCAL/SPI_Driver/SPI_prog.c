/*
 * SPI_prog.c
 *
 * Created: 8/28/2023 6:14:53 PM
 *  Author: Omar
 */ 
#include<avr/io.h>
#include "../../LIB/BIT_MATH.h"
#include "../DIO_Driver/DIO_interface.h"
#include"SPI_interface.h"
void SPI_voidMasterInit(void){
	
	// master initialization 
	//clk
	DIO_voidSetPinDirection(DIO_PORTB,DIO_PIN7,DIO_OUTPUT);
	// MOSI
	DIO_voidSetPinDirection(DIO_PORTB,DIO_PIN5,DIO_OUTPUT);
	//miso
	DIO_voidSetPinDirection(DIO_PORTB,DIO_PIN6,DIO_INPUT);
	//ss
	DIO_voidSetPinDirection(DIO_PORTB,DIO_PIN4,DIO_INPUT);
	DIO_voidSetPinValue(DIO_PORTB,DIO_PIN4,DIO_HIGH);
	SET_BIT(SPCR,4);
	//clock prescaller
	CLR_BIT(SPSR,0);
	SET_BIT(SPCR,1);
	CLR_BIT(SPCR,0);
	//enable spi
	SET_BIT(SPCR,6);
}

void SPI_voidSlaveInit(void){
	//CLK
	DIO_voidSetPinDirection(DIO_PORTB,DIO_PIN7,DIO_INPUT);
	//MOSI
	DIO_voidSetPinDirection(DIO_PORTB,DIO_PIN5,DIO_INPUT);
	//MISO
	DIO_voidSetPinDirection(DIO_PORTB,DIO_PIN6,DIO_OUTPUT);
	//SS
	DIO_voidSetPinDirection(DIO_PORTB,DIO_PIN4,DIO_INPUT);
	//slave init
	CLR_BIT(SPCR,4);
	//enable SPI
	SET_BIT(SPCR,6);
}

u8 SPI_u8Transceiver(u8 Copy_u8Data){
	
	//send data 
	SPDR=Copy_u8Data;
	while(GET_BIT(SPSR,7)==0);
	
	return SPDR;
}