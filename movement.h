/*
 * movement.h
 *
 *  Created on: Mar 24, 2015
 *      Author: Christopher
 */
#include "CGraph.h"
#ifndef MOVEMENT_H_
#define MOVEMENT_H_

void centerProtocol(void);
void crossDetect(void);
void forwardACell(void);
void flagCheck(void);
void cellMath(void);
void DriveRobotWithDirection(int* direction);
void verticalAlign(void);
void cameraDirection(void);
void cameraTurn(nbool visited);
#endif /* MOVEMENT_H_ */
