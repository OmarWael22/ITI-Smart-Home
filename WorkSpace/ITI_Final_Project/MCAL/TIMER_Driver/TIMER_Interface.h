/*
 * TIMER_Interface.h
 *
 *  Created on: Aug 23, 2023
 *      Author: Hardware
 */

#ifndef MCAL_TIMER_DRIVER_TIMER_INTERFACE_H_
#define MCAL_TIMER_DRIVER_TIMER_INTERFACE_H_



#define NORMAL	0
#define CTC		1

#define ENABLE		2
#define DISABLE		3
#define RISING_EDGE 		4
#define FALLING_EDGE		5



#define PWM		2
//************************************************
//TIMER0

void TIMER_Timer0Init(u8 Copy_Mode,u8 Copy_Prescaller);  //Function to initialize Timer0 to be in Normal or CTC Mode

void TIMER_Timer0RegValue(u8 Copy_Preload_Compare_Value); //Function to save input value as preload or compare depending on Mode


void TIMER_VoidTimer0ISR_Normal(void(*ISR11_PTR)(void)); //pointer to the ISR Normal Function


void TIMER_VoidTimer0ISR_CTC(void(*ISR10_PTR)(void)); //pointer to the ISR CTC Function
//************************************************
//Timer1
void TIMER_VoidTimer1Init(u8 Copy_Mode, u8 Copy_Prescaller);

void TIMER_VoidSetOverFlow(u16 Copy_u16OverFlow);

void TIMER_VoidTimer1ChannelASetCompareValue(u16 Copy_u16CompareValue);

void TIMER_VoidTimer1SetPreloadValue(u16 Copy_u16Value);

u16 Timer_u16Timer1GetTimerValue(void);

void ICU_VoidSetSenseControl(u8 Copy_u8SenseControl);

void ICU_VoidInterruptControl(u8 Copy_u8InterruptState);

void ICU_VoidSetCallBackFunction(void(*Copy_PTR_ICUFunction)(void));

u16 ICU_u16ReadInputCapture(void);

//************************************************

//TIMER Delay
void TIMER_delay_ms(u32 Copy_delayTime);   //delay with only two for loops
//************************************************

//TIMER2 Fast PWM Mode
void TIMER_VoidTimer2FastPWM(void);

void TIMER_VoidTimer2FastPWMCompareValue(u8 Copy_u8CompareMatchValue);
//************************************************

#endif /* MCAL_TIMER_DRIVER_TIMER_INTERFACE_H_ */
