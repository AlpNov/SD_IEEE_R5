#ifndef MAZEALG_H_
#define MAZEALG_H_
#include "CGraph.h"
#include <stdint.h>


int DirectionFnc(uint32_t* SensorLeft, uint32_t* SensorRight, uint32_t* SensorRear, uint32_t* SensorFront, int* Tolerance);
void CurrentAdjacent(uint32_t* SensorLeft, uint32_t* SensorRight, uint32_t* SensorRear, uint32_t* SensorFront, int* Tolerance, int* OrientationValue, unsigned short* currentCellValue, int* Dimension, Cell** graph);
int ReturnDirectionFromCellValue(unsigned short* currentCellValue, Cell** CriticalPath, int* OrientationValue, int* Dimension);
void updateCurrentCell(int* OrientationValue, unsigned short* currentCellValue, int* movement, int* Dimension);
void updateOrientation(int* OrientationValue, int* movement);
#endif /* MAZEALG_H_ */
