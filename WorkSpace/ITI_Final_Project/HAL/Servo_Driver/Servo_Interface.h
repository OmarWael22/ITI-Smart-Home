/*
 * Servo_Interface.h
 *
 *  Created on: Aug 27, 2023
 *      Author: Hardware
 */

#ifndef HAL_SERVO_DRIVER_SERVO_INTERFACE_H_
#define HAL_SERVO_DRIVER_SERVO_INTERFACE_H_

/*
 * Calculations Depends on 8 MHz clock speed
 *
 */
void Servo_voidInit(void);

void Servo_voidDegree(u8 Copy_u8Degree);

u16 Servo_u16GetTicks(void);

void Servo_voidReset(void);

#endif /* HAL_SERVO_DRIVER_SERVO_INTERFACE_H_ */
