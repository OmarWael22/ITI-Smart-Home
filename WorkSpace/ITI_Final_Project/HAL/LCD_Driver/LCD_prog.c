/*
 * LCD_prog.c
 *
 * Created: 8/20/2023 9:37:08 PM
 *  Author: Omar
 */ 
#define F_CPU 8000000UL
#include<stdlib.h>
#include<avr/io.h>
#include <util/delay.h>
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"
#include"../../MCAL/DIO_Driver/DIO_interface.h"

#include"LCD_interface.h"

void LCD_voidSendData(u8 Copy_u8Data){
	
	DIO_voidSetPinValue(LCD_RS_PORT, LCD_RS_PIN, DIO_HIGH); // Send Command
	DIO_voidSetPinValue(LCD_RW_PORT, LCD_RW_PIN, DIO_LOW); // Write
	
	DIO_voidSetPortValue(LCD_DATA_PORT,Copy_u8Data);
	
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, DIO_HIGH);
	_delay_ms(2);
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, DIO_LOW);
}

void LCD_voidSendCommand(u8 Copy_u8Command){
	DIO_voidSetPinValue(LCD_RS_PORT, LCD_RS_PIN, DIO_LOW); // Send Command
	DIO_voidSetPinValue(LCD_RW_PORT, LCD_RW_PIN, DIO_LOW); // Write
	
	DIO_voidSetPortValue(LCD_DATA_PORT,Copy_u8Command);
	
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, DIO_HIGH);
	_delay_ms(2);
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, DIO_LOW);
}

void LCD_voidInit(void){
	DIO_voidSetPortDirection(LCD_DATA_PORT,DIO_OUTPUT_PORT);
	DIO_voidSetPinDirection(LCD_RS_PORT, LCD_RS_PIN, DIO_OUTPUT);
	DIO_voidSetPinDirection(LCD_RW_PORT, LCD_RW_PIN, DIO_OUTPUT);
	DIO_voidSetPinDirection(LCD_EN_PORT, LCD_EN_PIN, DIO_OUTPUT);
	_delay_ms(40);
	LCD_voidSendCommand(0b00111000);
	LCD_voidSendCommand(0b00001100);
	LCD_voidSendCommand(0b00000001);
	LCD_voidSendCommand(0b00000110);
}

void LCD_voidClearDisplay(void){
	LCD_voidSendCommand(0b00000001);
}

void LCD_voidGoTOXY(u8 Copy_u8X,u8 Copy_u8Y){
	
	u8 Local_add = 0;
	Local_add=Copy_u8X*(0x40)+Copy_u8Y;

	SET_BIT(Local_add, 7); // lcd understands that this is an address of position
	LCD_voidSendCommand(Local_add);
}

u8 LCD_u8ReadAC(void)
{
	// Configure RS and RW pins for reading
	DIO_voidSetPinValue(LCD_RS_PORT, LCD_RS_PIN, DIO_LOW); // RS = 0 (Command mode)
	DIO_voidSetPinValue(LCD_RW_PORT, LCD_RW_PIN, DIO_HIGH); // RW = 1 (Read mode)

	// Send the command to read the address counter
	LCD_voidSendCommand(0b10000000);

	// Configure data pins as input (DB0-DB7)
	DIO_voidSetPortDirection(LCD_DATA_PORT, DIO_INPUT_PORT);

	// Enable LCD to output the address counter value
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, DIO_HIGH);
	_delay_us(1); // Delay for a short time to ensure data is ready
	u8 ac = DIO_u8GetPortValue(LCD_DATA_PORT); // Read the address counter value
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, DIO_LOW);

	// Set data pins back to output mode
	DIO_voidSetPortDirection(LCD_DATA_PORT, DIO_OUTPUT_PORT);
	

	return ac;
}

void LCD_voidShiftDisplay(u8 Copy_u8Direction){
	
	static u8 Local_u8AddressCounter=0;
	switch(Copy_u8Direction){
		
		case LCD_SHIFT_L:
			// Shift the entire display to the left (S/C=1, R/L=1)
			LCD_voidSendCommand(0b00011000);
			break;
		case LCD_SHIFT_R:
			LCD_voidSendCommand(0b00011100);
			break;
			
	}
	if(Local_u8AddressCounter>=16){
		LCD_voidSendCommand(0b00000010);
		Local_u8AddressCounter=0;
		
	}
	else
		Local_u8AddressCounter++;
		
		
}

void LCD_voidSendString(u8* Copy_pcString)
{
	u8 Local_u8Index=0;

	while(Copy_pcString[Local_u8Index]!='\0')
	{
		LCD_voidSendData(Copy_pcString[Local_u8Index]);
		Local_u8Index++;
	}
}

void LCD_voidSendNumber(u32 Copy_u32Number)
{
	u8 Local_u8Str[10];
	itoa(Copy_u32Number, Local_u8Str,10);
	LCD_voidSendString(Local_u8Str);

}


