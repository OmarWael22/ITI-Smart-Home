/*
 * GIE_prog.c
 *
 * Created: 8/21/2023 3:40:19 PM
 *  Author: Omar
 */ 

#include<avr/io.h>
#include"../../LIB/BIT_MATH.h"

void GIE_voidEnableGlobalInt(void){
	
	SET_BIT(SREG,7);
	
}
void GIE_voidDisableGlobalInt(void){
	
	CLR_BIT(SREG,7);
	
}