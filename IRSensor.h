/*
 * IRSensor.h
 *
 *  Created on: Feb 9, 2015
 *      Author: cmahn
 */

#ifndef IRSENSOR_H_
#define IRSENSOR_H_

void IRSensorInit(void);
void CheckIR(void);
void CheckIR_Front(void);
void CheckIR_Back(void);
void CheckIR_Left(void);
void CheckIR_Right(void);

void IR_ISR_Front(void);
void IR_ISR_Back(void);
void IR_ISR_Left(void);
void IR_ISR_Right(void);

void wallCheck(void);



#endif /* IRSENSOR_H_ */
