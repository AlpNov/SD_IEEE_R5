#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Motor.h
 *
 *  Created on: Oct 25, 2014
 *      Author: kdvan */

//#ifndef MOTOR_H_
#define MOTOR_H_
//Put Driver Codes here

#ifndef MOTOR_H
#define MOTOR_H

extern void Motors_Init(void);
void Motors_shift(uint32_t r1, uint32_t r2, uint32_t l1, uint32_t l2);
void InitialPinSet(void);
void ReversePinSet(void);
void RotateLeft(void);
void RotateRight(void);
void shiftRight(void);
void shiftLeft(void);
void veerRight(int vision);
void veerLeft(int vision);
void rotateRight_Sense(void);
void rotateLeft_Sense(void);
void diagonalRight(int n);
void diagonalLeft(int n);
void frontLeft(void);
void frontRight(void);
void backLeft(void);
void backRight(void);
void rotate180_Sense(void);
void ServoControl(void);
void ServoTune(void);
void Motors_stop();
#ifdef __cplusplus
}
#endif
#endif /* MOTOR_H_ */
