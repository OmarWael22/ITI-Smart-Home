/*
 * LCD_interface.h
 *
 * Created: 8/20/2023 9:37:27 PM
 *  Author: Omar
 */ 


#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_

/*
 * RS(Register Select): 0 ==>  command   1==>  Data
 * RW(Read/Write): 0 ==> Write   1==>Read
 * EN(Enable)
 * */

#define LCD_DATA_PORT	DIO_PORTB

#define LCD_RS_PORT		DIO_PORTA
#define LCD_RS_PIN		DIO_PIN5

#define LCD_RW_PORT		DIO_PORTA
#define LCD_RW_PIN		DIO_PIN6

#define LCD_EN_PORT		DIO_PORTA
#define LCD_EN_PIN		DIO_PIN7

#define LCD_SHIFT_L			0
#define LCD_SHIFT_R			1



void LCD_voidSendData(u8 Copy_u8Data);

void LCD_voidSendCommand(u8 Copy_u8Command);

void LCD_voidInit(void);

void LCD_voidClearDisplay(void);

void LCD_voidGoTOXY(u8 Copy_u8X,u8 Copy_u8Y);

u8 LCD_u8ReadAC();

void LCD_voidShiftDisplay(u8 Copy_u8Direction);

void LCD_voidSendString(u8* Copy_pcString);

void LCD_voidSendNumber(u32 Copy_u32Number);


#endif /* LCD_INTERFACE_H_ */
