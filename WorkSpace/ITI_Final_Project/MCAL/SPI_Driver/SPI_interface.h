/*
 * SPI_interface.h
 *
 * Created: 8/28/2023 6:15:10 PM
 *  Author: Omar
 */ 


#ifndef SPI_INTERFACE_H_
#define SPI_INTERFACE_H_

#include"../../LIB/STD_TYPES.h"

void SPI_voidMasterInit(void);

void SPI_voidSlaveInit(void);

u8 SPI_u8Transceiver(u8 Copy_u8Data);



#endif /* SPI_INTERFACE_H_ */