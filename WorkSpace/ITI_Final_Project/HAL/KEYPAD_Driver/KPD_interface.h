/*
 * KPD_interface.h
 *
 * Created: 8/21/2023 3:34:42 AM
 *  Author: Omar
 */ 


#ifndef KPD_INTERFACE_H_
#define KPD_INTERFACE_H_

#define KPD_KEY_ARRAY   \
{ \
	{7,8,9},	\
	{4,5,6},	\
	{1,2,3},	\
	{KPD_CLEAR,0,KPD_ENTER}	\
}
#define KPD_NCOL			3
#define KPD_NROW			4
#define KPD_COL_PORT		DIO_PORTC

#define KPD_COL0_PIN		DIO_PIN4
#define KPD_COL1_PIN		DIO_PIN5
#define KPD_COL2_PIN		DIO_PIN6
#define KPD_COL3_PIN		DIO_PIN7

#define KPD_ROW_PORT		DIO_PORTA

#define KPD_ROW0_PIN		DIO_PIN1
#define KPD_ROW1_PIN		DIO_PIN2
#define KPD_ROW2_PIN		DIO_PIN3
#define KPD_ROW3_PIN		DIO_PIN4

#define KPD_NO_PRESS		0xAA
#define KPD_PLUS			14
#define KPD_MINUS			15
#define KPD_DIVIDE			16
#define KPD_TIMES			17
#define KPD_CLEAR			13
#define KPD_ENTER			12


#define KPD_ISOperator(key)		key==KPD_PLUS || key==KPD_MINUS || key==KPD_DIVIDE ||  key==KPD_TIMES


void KPD_voidInit();

u8 KPD_u8GetPressedKey(void);


#endif /* KPD_INTERFACE_H_ */