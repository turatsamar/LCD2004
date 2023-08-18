/*
 * ultrasonic.h
 *
 *  Created on: 3 Aug 2023
 *      Author: turabek
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_


#define TRIG_PIN BIT2
#define ECHO_PIN BIT3

uint16_t UltrasonicGetDistance();
void UltrasonicInit();


#endif /* ULTRASONIC_H_ */
