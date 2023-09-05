/*
 * EXTI_interface.h
 *
 * Created: 8/21/2023 4:07:20 PM
 *  Author: Omar
 */ 



#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_

#define EXTI_INT0	0
#define EXTI_INT1	1
#define EXTI_INT2	2

#define EXTI_LOW_LEVEL		0
#define EXTI_ANY_LOGIC		1
#define EXTI_FALLING_EDGE	2
#define EXTI_RAISNG_EDGE	3

void EXTI_voidInit(u8 Copy_u8InterruptNumber , u8 Copy_u8SourceType);

void EXTI_voidSetCallBackINT0(void(*NotifyFunc0)(void));

void EXTI_voidSetCallBackINT1(void(*NotifyFunc1)(void));

void EXTI_voidSetCallBackINT2(void(*NotifyFunc2)(void));




#endif /* EXTI_INTERFACE_H_ */