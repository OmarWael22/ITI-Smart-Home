/*
 * KPD_prog.c
 *
 * Created: 8/21/2023 3:34:20 AM
 *  Author: Omar
 */ 

#include<avr/io.h>

#include"../../LIB/BIT_MATH.h"
#include"../../LIB/STD_TYPES.h"

#include"../../MCAL/DIO_Driver/DIO_interface.h"

#include"KPD_interface.h"


			
void KPD_voidInit(){
	
	
	// set col s output high
	DIO_voidSetPinDirection(KPD_COL_PORT,KPD_COL0_PIN,DIO_OUTPUT);
	DIO_voidSetPinDirection(KPD_COL_PORT,KPD_COL1_PIN,DIO_OUTPUT);
	DIO_voidSetPinDirection(KPD_COL_PORT,KPD_COL2_PIN,DIO_OUTPUT);
	//DIO_voidSetPinDirection(KPD_COL_PORT,KPD_COL3_PIN,DIO_OUTPUT);

	DIO_voidSetPinValue(KPD_COL_PORT,KPD_COL0_PIN,DIO_HIGH);
	DIO_voidSetPinValue(KPD_COL_PORT,KPD_COL1_PIN,DIO_HIGH);
	DIO_voidSetPinValue(KPD_COL_PORT,KPD_COL2_PIN,DIO_HIGH);
	//DIO_voidSetPinValue(KPD_COL_PORT,KPD_COL3_PIN,DIO_HIGH);

	// set rows input pull up
	DIO_voidSetPinDirection(KPD_ROW_PORT,KPD_ROW0_PIN,DIO_INPUT);
	DIO_voidSetPinDirection(KPD_ROW_PORT,KPD_ROW1_PIN,DIO_INPUT);
	DIO_voidSetPinDirection(KPD_ROW_PORT,KPD_ROW2_PIN,DIO_INPUT);
	DIO_voidSetPinDirection(KPD_ROW_PORT,KPD_ROW3_PIN,DIO_INPUT);


	DIO_voidSetPinValue(KPD_ROW_PORT,KPD_ROW0_PIN,DIO_HIGH);
	DIO_voidSetPinValue(KPD_ROW_PORT,KPD_ROW1_PIN,DIO_HIGH);
	DIO_voidSetPinValue(KPD_ROW_PORT,KPD_ROW2_PIN,DIO_HIGH);
	DIO_voidSetPinValue(KPD_ROW_PORT,KPD_ROW3_PIN,DIO_HIGH);

}

u8 KPD_u8GetPressedKey(void){

	u8 Local_u8Pressedkey= KPD_NO_PRESS;
	u8 Local_u8RowxValue ;


	u8 Local_u8ColumnsArray[]={KPD_COL0_PIN,KPD_COL1_PIN,KPD_COL2_PIN};
	u8 Local_u8RowsArray[]={KPD_ROW0_PIN,KPD_ROW1_PIN,KPD_ROW2_PIN,KPD_ROW3_PIN};

	u8 Local_u8KeyArray[KPD_NROW][KPD_NCOL]=KPD_KEY_ARRAY;


	for(u8 Local_u8CloumnsIndex=0 ; Local_u8CloumnsIndex<KPD_NCOL ; Local_u8CloumnsIndex++){

		DIO_voidSetPinValue(KPD_COL_PORT,Local_u8ColumnsArray[Local_u8CloumnsIndex],DIO_LOW);

		for ( u8 Local_u8RowsIndex=0 ; Local_u8RowsIndex<KPD_NROW ; Local_u8RowsIndex++ ){

			Local_u8RowxValue= DIO_u8GetPinValue(KPD_ROW_PORT,Local_u8RowsArray[Local_u8RowsIndex]);

			if(Local_u8RowxValue==0){

				Local_u8Pressedkey=Local_u8KeyArray[Local_u8RowsIndex][Local_u8CloumnsIndex];
				while(Local_u8RowxValue==0){
					Local_u8RowxValue= DIO_u8GetPinValue(KPD_ROW_PORT,Local_u8RowsArray[Local_u8RowsIndex]);

				}

				return Local_u8Pressedkey ;
			}


		}

		DIO_voidSetPinValue(KPD_COL_PORT,Local_u8ColumnsArray[Local_u8CloumnsIndex],DIO_HIGH);



	}


	return Local_u8Pressedkey ;
}