/*
 * WTD_prog.c
 *
 * Created: 8/28/2023 4:32:30 AM
 *  Author: Omar
 */ 

#include <avr/io.h>
#include"../../LIB/BIT_MATH.h"

void WDT_voidInit(void){
	
	
	
	//enable 
	SET_BIT(WDTCR,3);
	
	// sleep time 2.2 sec
	SET_BIT(WDTCR,0);
	SET_BIT(WDTCR,1);
	SET_BIT(WDTCR,2);
}

void WDT_voidDisable(void){
	
	WDTCR |= (1<<4) | (1<<3) ;
	WDTCR=0;
}

void WDT_voidReset(void){
	
	__asm__ __volatile__ ("wdr");
	
}