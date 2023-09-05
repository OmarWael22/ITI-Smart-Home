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

u8 Global_u8SavedDoorPassLowByte;
u8 Global_u8SavedDoorPassHighByte;
u16 Global_u16EPROMDoorPass;
u16 Global_u16EPROMPassAddress=50;
void TempSensor(void);
void DoorPass(void);
void test(void){
	u8 var =0;u8 var2 = 0;
	
	
	TWI_voidMasterInit(0);
	EEPROM_voidSendDataByte(0x01,0x00FF);
	TIMER_delay_ms(300);
	EEPROM_voidSendDataByte(0xFF,0x00F0);
	TIMER_delay_ms(100);
	// FA ==> 0xF0,   FC ==> 0x03,  AD ==> 0x04,  0x03F0 ==> 0x07  0x0300 ==> 0x0F
	EEPROM_voidReadDataByte(&var,0x00FF);
	TIMER_delay_ms(300);
	EEPROM_voidReadDataByte(&var2,0x00F0);
	DIO_voidSetPortValue(DIO_PORTA,var);
	TIMER_delay_ms(3000);
	DIO_voidSetPortValue(DIO_PORTA,var2);
}
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
	// save initial password = 500
	/*Global_u16EPROMDoorPass=100;
	
	Global_u8SavedDoorPassLowByte=(u8)Global_u16EPROMDoorPass;
	
	Global_u8SavedDoorPassHighByte=(u8)(Global_u16EPROMDoorPass>>8);
	EEPROM_voidSendDataByte(Global_u8SavedDoorPassLowByte,Global_u16EPROMPassAddress);
	TIMER_delay_ms(300);
	
	EEPROM_voidSendDataByte(Global_u8SavedDoorPassHighByte,Global_u16EPROMPassAddress+10);
	TIMER_delay_ms(300);*/
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
	
	u8 Local_u8Keypad_Key=KPD_u8GetPressedKey();
	static u8 Local_u8DoorStatus=0;
	static u16 Local_u16DoorPassword=0;
	
	if(Local_u8Keypad_Key==KPD_CLEAR){
		
		//set new pass
		if(Local_u16DoorPassword==Global_u16EPROMDoorPass){
			
			LCD_voidClearDisplay();
			// setting new password
			
				Global_u16EPROMDoorPass=0;
				Local_u16DoorPassword=0;
				LCD_voidSendString("Set new pass:");
				Local_u8Keypad_Key=KPD_NO_PRESS;
				while(Local_u8Keypad_Key != KPD_ENTER ){
					
					Local_u8Keypad_Key=KPD_u8GetPressedKey();
					if(Local_u8Keypad_Key==KPD_CLEAR){
						LCD_voidClearDisplay();
						Global_u16EPROMDoorPass=0;
						LCD_voidSendString("Set new pass:");
					}
					else if(Local_u8Keypad_Key !=KPD_NO_PRESS && Local_u8Keypad_Key!=KPD_ENTER){
						
						LCD_voidSendData('*');
						
						Global_u16EPROMDoorPass=Global_u16EPROMDoorPass*10 +Local_u8Keypad_Key;
					}
					
				}
				LCD_voidClearDisplay();
				LCD_voidSendString("pass is updated");
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
		
		else{
		LCD_voidClearDisplay();
		Local_u16DoorPassword=0;
		
		}
	}
	else if(Local_u8Keypad_Key==KPD_ENTER){
		
		LCD_voidClearDisplay();
		 if(Local_u16DoorPassword==Global_u16EPROMDoorPass){
			
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
			
			Local_u16DoorPassword=0;
		
	}
	
	else if(Local_u8Keypad_Key != KPD_NO_PRESS){
		if(Local_u16DoorPassword==0)
			LCD_voidClearDisplay();
		LCD_voidSendData('*');
		Local_u16DoorPassword=Local_u16DoorPassword*10 + Local_u8Keypad_Key;
		
	}
	
	
}





