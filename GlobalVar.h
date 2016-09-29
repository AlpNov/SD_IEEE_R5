/*
 * GlobalVar.h
 *
 *  Created on: Mar 16, 2015
 *      Author: Christopher
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
/* Example/Board Header files */
#include "Board.h"
#include "motor.h"
#include "line_sensor.h"
#include "uart.h"
#include "CGraph.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include <stdint.h>

#ifndef GLOBALVAR_H_
#define GLOBALVAR_H_
//Movement Constants
#define LEFT 0
#define RIGHT 1
#define FORWARD 2
#define REVERSE 3
#define EAST 0
#define WEST 1
#define NORTH 2
#define SOUTH 3
#define MAP 5
#define EXPLORE 6
#define DONE 7
#define CRITICALPATH 8
#define STOP 9
#define COMPLETE 10
#define SIZE 50
#define VEERA 9//18//28//38//48//58
#define VEERB 68
#define OVEERA 8//17//27//37//47//57
#define OVEERB 4//13//23//33//43//53
#define DV 1//10//20//30//40//50
#define ROTATE 49//50//48//43->MIGHT BE TOO FAST
#define BKE 99
#define SHIFTN 1026
//SERVO CONTROL VALUES
#define SERVOF 505
#define SERVOL 190
#define SERVOR 400
#define SERVOB 290



int cell;
int directionFlag;
//EncoderCount
volatile int32_t Encount;
UART_Handle uart;
UART_Params uartParams;
uint32_t ui32LoadServo;

//IR Distance
uint32_t readingF,readingR,readingL, readingB;
int FCamFlag,LCamFlag,RCamFlag,BCamFlag;
//line following
int checkFlag;
uint32_t readingLine;
uint32_t readingLineR;
int sideLine;
int sideLineCount;
int vision,prevision;
int Bvision,Bprevision;
int L,C,R;
int BL,BC,BR;
int VAFlagF,VAFlagB;
int status_light_flag;
//encoder_button
int button;
int ready2go;//counter flag used for the button hardware ISR
//line following
int lost,Blost,RotationFlag;

int bumpFlag;


//movement
int veerFlagF,veerFlagB;
//hector added
int Orientation;
unsigned short currentcell;  //starting cell number
unsigned short endcell;
unsigned short startcell;
int endfound;
//Dimension will be either 5,6, or 7
int Dimension;
int Tolerance;
Cell* UnvisitedStack;
Graph CurrentGraph;
#endif /* GLOBALVAR_H_ */
