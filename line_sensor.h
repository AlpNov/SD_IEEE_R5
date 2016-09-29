/*
 * line_sensor.h
 *
 *  Created on: Feb 18, 2015
 *      Author: cmahn
 */

#ifndef LINE_SENSOR_H_
#define LINE_SENSOR_H_

void LineInit(void);
void LineClock(void);
void ReadSensorsW(void);
void LineSensorInput_ISR(void);
void LineSensorRead_ISR(void);
void FollowSequence(int vision, int prevision);
void checkAnalogLine(void);
void printAnalogLine(void);
void checkVision(void);
void printVision(void);
int calcVision(int L, int C, int R);
void sideLineDetect(void);
void alignment(void);
void analogCheckCount(void);

#endif /* LINE_SENSOR_H_ */
