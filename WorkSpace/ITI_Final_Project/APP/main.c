/*
 * main.c
 *
 * Created: 8/20/2023 3:27:04 PM
 *  Author: Omar
 */ 
#define F_CPU 8000000UL
#include "util/delay.h"
#include"..\MCAL\DIO_Driver\DIO_interface.h"
#include"../HAL/LCD_Driver/LCD_interface.h"
#include"../HAL/KEYPAD_Driver/KPD_interface.h"
#include "../MCAL/GIE_Driver/GIE_interface.h"
#include"../MCAL/EXTI_Driver/EXTI_interface.h"
#include"../MCAL/ADC_Driver/ADC_interface.h"
#include "../MCAL/TIMER_Driver/TIMER_interface.h"
#include "../MCAL/USART_Driver/USART_interface.h"
#include"../MCAL/WDT_Driver/WDT_interface.h"
#include "../MCAL/SPI_Driver/SPI_interface.h"
#include "../MCAL/TWI_Driver/TWI_interface.h"
#include "../HAL/EPROM_Driver/EPROM_interface.h"
#include "../HAL/Servo_Driver/Servo_Interface.h"
#include "../LIB/BIT_MATH.h"

#define		Max_Pass_Digits		4

u8 Global_u8SavedDoorPassLowByte;
u8 Global_u8SavedDoorPassHighByte;
u16 Global_u16EPROMDoorPass;
u16 Global_u16EPROMPassAddress=50;
void TempSensor(void);
void DoorPass(void);
void InitDoorPass(u16 );
void main(void)
{	
	ADC_voidInit();
	DIO_voidSetPinDirection(DIO_PORTD,DIO_PIN6,DIO_OUTPUT);
	DIO_voidSetPinDirection(DIO_PORTD,DIO_PIN7,DIO_OUTPUT);
	
	LCD_voidInit();
	KPD_voidInit();
	//servo pin
	DIO_voidSetPinDirection(DIO_PORTD,DIO_PIN5,DIO_OUTPUT);
	Servo_VoidInit();
	
	TWI_voidMasterInit(0);
	// save initial password = 100
	/*Global_u16EPROMDoorPass=100;
	InitDoorPass(Global_u16EPROMDoorPass);*/
	
	// read pass low byte
	EEPROM_voidReadDataByte(&Global_u8SavedDoorPassLowByte,Global_u16EPROMPassAddress);
	TIMER_delay_ms(300);
	// read pass high byte
	EEPROM_voidReadDataByte(&Global_u8SavedDoorPassHighByte,Global_u16EPROMPassAddress+10);
	TIMER_delay_ms(300);
	// combine the high and low byte
	Global_u16EPROMDoorPass= ( (u16)(Global_u8SavedDoorPassHighByte<<8) )| Global_u8SavedDoorPassLowByte;
	LCD_voidSendNumber(Global_u16EPROMDoorPass);
	while(1){
		
		DoorPass();
		TempSensor();

	}

}

void TempSensor(void){
	u8 Temp = ADC_u8StartConversion(0);
	/*
	 * LM35 Temperatur Sensor Vout Calculations with connection applied in proteus:
	 * Vout = 10 mV/C
	 * 25 C ===> (10*(10^-3)*25*2^8) / 5 = 12.8
	 * 35 C ===> (10*(10^-3)*35*2^8) / 5 = 17.9
	 */
	if(Temp>=18){
		DIO_voidSetPinValue(DIO_PORTD,DIO_PIN6,DIO_HIGH);
		DIO_voidSetPinValue(DIO_PORTD,DIO_PIN7,DIO_LOW);
	}
	else if (Temp>=13){
		DIO_voidSetPinValue(DIO_PORTD,DIO_PIN6,DIO_LOW);
		DIO_voidSetPinValue(DIO_PORTD,DIO_PIN7,DIO_HIGH);
	}
	else{
		DIO_voidSetPinValue(DIO_PORTD,DIO_PIN6,DIO_LOW);
		DIO_voidSetPinValue(DIO_PORTD,DIO_PIN7,DIO_LOW);
	}
}

void DoorPass(){
	/*To set new pass enter your old pass and press clear*/
	/* max password digits is 4*/
	
	// pressed key
	u8 Local_u8Keypad_Key=KPD_u8GetPressedKey();
	// door status flag to indicate if the door is opened or closed
	static u8 Local_u8DoorStatus=0;
	// the password entered by the user
	static u16 Local_u16DoorPassword=0;
	// counter to the number of digits entered
	static Local_u8DigitsCount=0;
	
	/* we have three cases in this function
		* First : clear is pressed
		* Second : Enter is pressed
		* Third : digit is pressed
	*/
	
	/*******************************************************************************************************************************************************************/
	/* case user entered clear */
		/* two cases to enter clear
		* First : To set new pass
		* Second : clear screen and clear the entered password 
		*/
	if(Local_u8Keypad_Key==KPD_CLEAR){
		
		// First case : set new pass
		if(Local_u16DoorPassword==Global_u16EPROMDoorPass){
			
			LCD_voidClearDisplay();
			// setting new password
				// clear all saved passes and digit counter
				Global_u16EPROMDoorPass=0;
				Local_u16DoorPassword=0;
				Local_u8DigitsCount=0;
				
				LCD_voidSendString("Set new pass:");
				Local_u8Keypad_Key=KPD_NO_PRESS;
				while(Local_u8Keypad_Key != KPD_ENTER ){
					
					Local_u8Keypad_Key=KPD_u8GetPressedKey();
					// check if the user entered more than 4 digits
					if(Local_u8DigitsCount==Max_Pass_Digits+1){
						LCD_voidClearDisplay();
						LCD_voidSendString("ERROR !!!!");
						LCD_voidGoTOXY(1,0);
						LCD_voidSendString("Max Digits is 4");
						Local_u8DigitsCount=0;
						Global_u16EPROMDoorPass=0;
					}
					
					if(Local_u8Keypad_Key==KPD_CLEAR){
						Local_u8DigitsCount=0;
						LCD_voidClearDisplay();
						Global_u16EPROMDoorPass=0;
					
					}
					else if(Local_u8Keypad_Key !=KPD_NO_PRESS && Local_u8Keypad_Key!=KPD_ENTER){
						// if it is the first digit that user enter --> clear any previous message on the screen
						if(Local_u8DigitsCount==0)
							LCD_voidClearDisplay();
						LCD_voidSendData('*');
						Local_u8DigitsCount++;
						Global_u16EPROMDoorPass=Global_u16EPROMDoorPass*10 +Local_u8Keypad_Key;
					}
					
				}
				LCD_voidClearDisplay();
				LCD_voidSendString("pass is updated");
				// reset counter
				Local_u8DigitsCount=0;
				Local_u8Keypad_Key=KPD_NO_PRESS;
				// save the new pass to eeprom
				// save low byte
				Global_u8SavedDoorPassLowByte=(u8)Global_u16EPROMDoorPass;
				EEPROM_voidSendDataByte(Global_u8SavedDoorPassLowByte,Global_u16EPROMPassAddress);
				TIMER_delay_ms(300);
				// save high byte
				Global_u8SavedDoorPassHighByte=(u8)(Global_u16EPROMDoorPass>>8);
				EEPROM_voidSendDataByte(Global_u8SavedDoorPassHighByte,Global_u16EPROMPassAddress+10);
				
		}
		
		// Second case : to clear Screen or typed password
		else{
		LCD_voidClearDisplay();
		Local_u16DoorPassword=0;
		Local_u8DigitsCount=0;
		
		}
	}
	/******************************************************************************************************************************************************************/
	/* case user pressed enter or equal */
	else if(Local_u8Keypad_Key==KPD_ENTER){
		
		LCD_voidClearDisplay();
		// check if entered pass is correct
		 if(Local_u16DoorPassword==Global_u16EPROMDoorPass){
			// check door status
			if(Local_u8DoorStatus==0){
				LCD_voidSendString("Door is opened");
				// run servo
				Servo_VoidDegree(180);
				Local_u8DoorStatus=1;
				
			}
			else{
				LCD_voidSendString("Door is Closed");
				// stop servo 
				Servo_VoidDegree(0);
				Local_u8DoorStatus=0;
				
			}
			
		}
		else
			LCD_voidSendString("Wrong password !");
			
			// reset the entered password after typing enter 
			Local_u16DoorPassword=0;
			Local_u8DigitsCount=0;
	}
	
	/****************************************************************************************************************************************************************/
	/* Case user pressed a digit key*/
	else if(Local_u8Keypad_Key != KPD_NO_PRESS){
		// if it is the first digit that user enter --> clear any previous message on the screen
		if(Local_u8DigitsCount==0)
			LCD_voidClearDisplay();
			
		LCD_voidSendData('*');
		
		Local_u8DigitsCount++;
		Local_u16DoorPassword=Local_u16DoorPassword*10 + Local_u8Keypad_Key;
		
		
	}
	
	
}
void InitDoorPass(u16 Copy_Globalu16EPROMDoorPass ){
	
	Global_u8SavedDoorPassLowByte=(u8)Copy_Globalu16EPROMDoorPass;
	
	Global_u8SavedDoorPassHighByte=(u8)(Copy_Globalu16EPROMDoorPass>>8);
	EEPROM_voidSendDataByte(Global_u8SavedDoorPassLowByte,Global_u16EPROMPassAddress);
	TIMER_delay_ms(300);
	
	EEPROM_voidSendDataByte(Global_u8SavedDoorPassHighByte,Global_u16EPROMPassAddress+10);
	TIMER_delay_ms(300);
	
}




