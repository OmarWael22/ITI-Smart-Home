/*
 * main.c
 *
 * Created: 8/20/2023 3:27:04 PM
 *  Author: Omar
 */ 

#define F_CPU 8000000UL
#include"..\MCAL\DIO_Driver\DIO_interface.h"
#include"../HAL/LCD_Driver/LCD_interface.h"
#include"../HAL/KEYPAD_Driver/KPD_interface.h"
#include "../MCAL/GIE_Driver/GIE_interface.h"
#include"../MCAL/ADC_Driver/ADC_interface.h"
#include "../MCAL/TIMER_Driver/TIMER_interface.h"
#include "../MCAL/USART_Driver/USART_interface.h"
#include "../MCAL/TWI_Driver/TWI_interface.h"
#include "../HAL/EPROM_Driver/EPROM_interface.h"
#include "../HAL/Servo_Driver/Servo_Interface.h"
#include "../LIB/BIT_MATH.h"

#define		Max_Pass_Digits			4
#define		INPUT_DECIDED_LENGTH	7  // As INP Format *i*x*x#

#define	LIGHT1	DIO_PIN5
#define LIGHT2	DIO_PIN6
#define LIGHT3	DIO_PIN7

//Door Pass max value is 4 so saving it in two u8 variables to send to EEPROM
u8 Global_u8SavedDoorPassLowByte;
u8 Global_u8SavedDoorPassHighByte;
//Door Pass in u16
u16 Global_u16EPROMDoorPass;
//First Address to save Pass Low Byte in
u16 Global_u16EPROMPassAddress=50;


typedef enum{
	NoErrors=0,
	ROOM_LIGHT_ON,
	ROOM_LIGHT_OFF,
	Input_Format_Is_Incorrect,
}ConditionsState;

ConditionsState USART_voidProcessCommand(u8 command,u8 Local_LightNum);
void USART_Start(void);
void Time_out(void);

void TempSensor(void);

void DoorPass(void);
void InitDoorPass(u16);

void main(void)
{
	ADC_voidInit();
	DIO_voidSetPinDirection(DIO_PORTD,DIO_PIN6,DIO_OUTPUT);
	DIO_voidSetPinDirection(DIO_PORTD,DIO_PIN7,DIO_OUTPUT);

	LCD_voidInit();
	KPD_voidInit();
	//servo pin
	DIO_voidSetPinDirection(DIO_PORTD,DIO_PIN5,DIO_OUTPUT);
	Servo_voidInit();

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

	// Initialize USART
	USART_voidInit();
	//Initialize Call Back Function
	USART_voidCallBackFunction(&USART_Start);
	//Enable USART PIE
	USART_voidEnableRxINT();
	//Enable GIE
	GIE_voidEnableGlobalInt();
	// LED Pins
	DIO_voidSetPinDirection(DIO_PORTC,DIO_PIN5,DIO_OUTPUT);
	DIO_voidSetPinDirection(DIO_PORTC,DIO_PIN6,DIO_OUTPUT);
	DIO_voidSetPinDirection(DIO_PORTC,DIO_PIN7,DIO_OUTPUT);

	while(1){
		/*USART is Controlled by ISR TIMER1_COMPB*/
		GIE_voidEnableGlobalInt();
		USART_voidEnableRxINT();
		//Call Door Control Function
		DoorPass();
		//Call Temperature Sensor Control Function
		TempSensor();

	}

}

void TempSensor(void){
	//Read Voltage value from ADC conversion
	u8 Temp = ADC_u8StartConversion(0);
	/*
	 * VOLTAGE TO TEMP CONVERSION:
	 * LM35 Temperature Sensor Vout Calculations with connection applied in proteus:
	 * Vout = 10 mV/C
	 * Vref = 5v
	 * Temp Calculation (Voltage Value*Voltage ref)/(10*(10^-3)*25*2^8)
	 */
	Temp = (u8)((Temp*5)/ (.01*256));
	//Check on Temp if over 35C ===> AC will work and turn off Fan if was on
	if(Temp>=35){
		DIO_voidSetPinValue(DIO_PORTD,DIO_PIN6,DIO_HIGH);
		DIO_voidSetPinValue(DIO_PORTD,DIO_PIN7,DIO_LOW);
	}
	//Check on Temp if over 25C ===> Fan will work and turn off AC if was on
	else if (Temp>=25){
		DIO_voidSetPinValue(DIO_PORTD,DIO_PIN6,DIO_LOW);
		DIO_voidSetPinValue(DIO_PORTD,DIO_PIN7,DIO_HIGH);
	}//Else State declare that Temp is lower than 25 so turning off both AC and Fan
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
	static u8 Local_u8DigitsCount=0;

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
				Servo_voidDegree(180);
				Local_u8DoorStatus=1;

			}
			else{
				LCD_voidSendString("Door is Closed");
				// stop servo
				Servo_voidDegree(0);
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

void USART_Start(void){
	//Save First Receieved Input AS It MUST be '*' If not return from function
	u8 FirstReceived = USART_u8ReceiverData();
	if(FirstReceived!='*')
		return;
	//Start Display Message
	USART_voidTransmitString("Expected INP Format \" *i*x*x# \" :\r");

	//Display first '*'
	USART_voidTransmitData(FirstReceived);

	//Clear LCD to Start Displaying on it
	LCD_voidClearDisplay();
	LCD_voidSendData('*');

	//data Receiving variable
	u8 DataReceived[INPUT_DECIDED_LENGTH-1];

	//Variable to contain index of command
	u8 DataIdx = 0;

	//Containing max enter value
	u8 Max_Num = INPUT_DECIDED_LENGTH-1;

	// Loop until reaching Max val of Input, if stucked Timer will INT and Time out
	while (Max_Num) {
		//Receive data element by element
		TimeOut TimeOut_Err=100;
		 TimeOut_Err = USART_u8TimeOUTReceiverData(&DataReceived[DataIdx]);
		//DataReceived[DataIdx] = USART_u8ReceiverData();
		if(TimeOut_Err == TimeOUT_Occured){
			/* Initializing Timer1 inside USART_u8TimeOUTReceiverData to make sure there's no stuck
			 * 1- SET TCNT1 with 0 before initializing timer
			 * 2- SET Comparing Value by TIMEOUTVALUE Macro  e.g. 40000====> 5.12 sec "time waiting before time out"
			 * 3- Initialize Timer with 1024 Prescaler
			 */
			//Sending Warning Message if TIMEOUT and return from function
			USART_voidTransmitData('\r');
			USART_voidTransmitString("##ERROR: Time Out");
			USART_voidTransmitData('\r');
			//Clear LCD
			LCD_voidClearDisplay();
			//ReCall Servo Initialization as both USART and Servo uses Timer1 with different modes
			Servo_voidInit();
			return;
		}
		//Send Received Data to Display it on Virtual Terminal
		USART_voidTransmitData(DataReceived[DataIdx] );
		//Display Data on LCD
		LCD_voidSendData(DataReceived[DataIdx]);
		//Increment Idx
		DataIdx++;
		//Decrement Max_Num only to loop Max_Num Inside While LOOP
		Max_Num --;
		//Set Timer Counter to 0 with Each Entered Input
		TIMER_voidTimer1SetPreloadValue(0);
			}

	//Display Enter in the terminal
	USART_voidTransmitData('\r');

	//Checking Input Format
	ConditionsState Con_St=NoErrors;
	//Check all input Defined values * and i and * and * and #
	if(DataReceived[0]!='i' || DataReceived[1]!='*' || DataReceived[3]!='*' || DataReceived[5]!='#'){
		Con_St = Input_Format_Is_Incorrect;
		USART_voidTransmitString("##INCORRECT INPUT FORMAT\r");
		//ReCall Servo Initialization as both USART and Servo uses Timer1 with different modes
		Servo_voidInit();
		return;
	}
	// Check First Undefined Val to be 1 or 2 or 3
	// also, sending it to USART_voidProcessCommand to light correct led
	// Receiving return value as indicator to uncorrect second undefined Val
	if(DataReceived[2]=='1')
		Con_St = USART_voidProcessCommand(DataReceived[4],LIGHT1);
	else if(DataReceived[2]=='2')
		Con_St = USART_voidProcessCommand(DataReceived[4],LIGHT2);
	else if (DataReceived[2]=='3')
		Con_St = USART_voidProcessCommand(DataReceived[4],LIGHT3);
	else// If First Undefined Val is not correct
		Con_St = Input_Format_Is_Incorrect;
	//Check on Con_St Condition
	if(Con_St == Input_Format_Is_Incorrect)//There's an Error in the Format Entered
		USART_voidTransmitString("##INCORRECT ROOM Num or State\r");
	else{//Check is Completed and there's no Error
		USART_voidTransmitString("ROOM ");
		USART_voidTransmitData(DataReceived[2]);
		if(Con_St==ROOM_LIGHT_ON)
			USART_voidTransmitString(" Light Is ON\r");
		else
			USART_voidTransmitString(" Light Is OFF\r");
	}
	//ReCall Servo Initialization as both USART and Servo uses Timer1 with different modes
	Servo_voidInit();
}

ConditionsState USART_voidProcessCommand(u8 command,u8 Local_LightNum) {
	ConditionsState Con_St = NoErrors; // Check Variable
	//Check Command to turn Light on or off
	if (command=='1') {
		// Turning ON ROOM 1 LIGHT
		DIO_voidSetPinValue(DIO_PORTC,Local_LightNum,DIO_HIGH);
		Con_St = ROOM_LIGHT_ON;
	}
	else if ( command=='0') {
		// Turning OFF ROOM 1 LIGHT
		DIO_voidSetPinValue(DIO_PORTC,Local_LightNum,DIO_LOW);
		Con_St = ROOM_LIGHT_OFF;
	}
	else{ // IF input is not 0 nor 1 ==> INCORRECT INP
		Con_St = Input_Format_Is_Incorrect;
	}
	return Con_St;
}



