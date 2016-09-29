/*
 * movement.c
 *
 *  Created on: Mar 24, 2015
 *      Author: Christopher
 */

#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
/* Example/Board Header files */
#include "Board.h"
#include "motor.h"
#include "cmd_int.h"
#include "encoder_button.h"
#include "movement.h"
#include "MazeAlg.h"
#include "CGraph.h"
#include "inc/hw_pwm.h"
#include "driverlib/pwm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_types.h"
#include "GlobalVar.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
//#include <stdbool.h>


#define NOBUMP 1400
#define BUMP   1900
#define WALLAHD 1687//22 TOO L

int cameraFace;//variable used to indicate the last position of the camera


extern UART_Handle uart;
extern UART_Params uartParams;

extern UART_Handle uartA;
extern UART_Params uartParamsA;
int bypassFlag=0;

void verticalAlign(void){
	//while((BC != 1)&&(C != 1)){
		ReadSensorsW();
        if(vision==0){
            if(RotationFlag==1){
                frontLeft();
            }
            else{
                frontRight();
            }
        }
		if(Bvision==100){
	        backLeft();
	    }
	    else if(Bvision==001){
	        backRight();
	    }
        else if(Bvision==0){
            //ROTATE A FEW ENCODER COUNTS TO LEFT TILL OBVIOUS NOTHING IS THERE OR
            //SOMETHING IS THERE. iN CASE NOTHING, DOUBLE THE OTHER WAY.
            //IN THE MEAN TIME SEARCH FOR A LINE DURING EACH SWEEP
        }
        if(vision==100){
            frontLeft();
        }
        else if(vision==001){
            frontRight();
        }

}


int movementProtocol(void){
    int temp=0;
    //CheckIR();
    if(readingL<500){
        temp= LEFT;
    }
    else if(readingF<500){
        temp=FORWARD;
    }
    else if(readingR<500){
        temp=RIGHT;
    }
    else if(readingB<500){
        temp=REVERSE;
    }
    return temp;
}





void Drive_Task(void){
		int direction;
		ReadSensorsW();
		CheckIR();
		Tolerance = 500;
		startcell = 49;
		currentcell = startcell;
		endcell = 1;
		endfound = 0;
		Dimension = 7;
		Orientation = NORTH;
		int nextmove = 0;
		UnvisitedStack = NULL;
		unsigned short temp2;
			Cell* adjList= NULL;
			Cell* Path = NULL;
			int Mode = MAP;
			nbool visited[SIZE];
				int i;
				for ( i = 0; i < SIZE; i++)
				{
				visited[i] = incorrect;
				}
				CurrentGraph.body = createGraph(SIZE);


    while(1){

        Semaphore_pend(DriveSema,BIOS_WAIT_FOREVER);
    	if(ready2go == 3 && Mode != DONE && Mode != CRITICALPATH)
        	{
        	Mode = STOP;
        	LEDRed();
        	}
        checkFlag=1;
        ReadSensorsW();
        verticalAlign();
        CheckIR();
       // movement=movementProtocol();

        UART_write(uart,"\r\n",sizeof("\r\n"));
        UART_write(uart,"cell:\t",sizeof("cell:\t"));
        UART_print_int(currentcell,uart);
        Semaphore_post(RXSema);

        switch(Mode)
        		{
        		case MAP:

        			direction = DirectionFnc(&readingL,&readingR,&readingB,&readingF,&Tolerance);

        		     //first time through the loop check -- set orientation equal to direction

        			CurrentAdjacent(&readingL,&readingR,&readingB,&readingF,&Tolerance,&Orientation,&currentcell,&Dimension,CurrentGraph.body);
/*****Camera Direction*********/
        			cameraDirection();
        			cameraTurn(visited[currentcell]);
//Set Current Cell to Visited
        			visited[currentcell] = correct;
//Look at the Map -- Look at the Cells adjacent to the current cell
        		    adjList = CurrentGraph.body[currentcell];
//If the adjacent value has not been visited push it onto the Unvisited Stack
        			while (adjList != NULL)
        			{
        				if (visited[adjList->x] == incorrect)
        				{

        					push_front(&UnvisitedStack,adjList->x);

        				}

        			       adjList = adjList->next;
        			   }
//First time the endcell is found
        					if(currentcell == endcell && endfound == 0)
        					{

        						endfound = 1;
        						Mode = EXPLORE;
        						LEDRed();

        					}
        					// This is the state in which it returns from Explore in order to check
        					//if there are any more cells to map
        					else if(endfound == 1 && direction == REVERSE)
        					{

        						Mode = EXPLORE;
        					}
        					//default case-- drive the robot with direction determined from distance sensors

        					else
        					{
        						//move in direction determined
        						DriveRobotWithDirection(&direction);
        						//move made update currentcell
        						updateCurrentCell(&Orientation,&currentcell, &direction,&Dimension);
        						updateOrientation(&Orientation,&direction);

        					}

        		break;

        		case EXPLORE:


        			temp2 = 0;
        				//get first value of Unvisited and set it to end point. Use the currentcell as start point
        				while(UnvisitedStack != NULL && Mode == EXPLORE)
        				{
        					 temp2= pop(&UnvisitedStack);
        					if(temp2 != 0)
        					{
        						if(visited[temp2] == incorrect )
        						{

        				 Path = CriticalPath(CurrentGraph.body,SIZE,&currentcell,temp2);

        					while(Path != NULL)
        					{
        						//read direction from path
        						direction = ReturnDirectionFromCellValue(&currentcell, &Path,&Orientation,&Dimension);
        						//if value is not NULL then update the current cell and drive

        						if(direction != 999)
        						{
        							DriveRobotWithDirection(&direction);
        							updateCurrentCell(&Orientation, &currentcell, &direction, &Dimension);
        							updateOrientation(&Orientation, &direction);


        						}
        					}
        								    CheckIR();
        					        		nextmove = DirectionFnc(&readingL,&readingR,&readingB,&readingF,&Tolerance);
        					        		if(nextmove != REVERSE)
        					        		{
        					        						//more cells to visit and map
        					        		Mode = MAP;
        					        				//break out of the while loop for Unvisited

        					        		}
        					        					//pop is done in while, so check if NULL to switch modes
        					        	if(UnvisitedStack == NULL)
        					        		{
        					        			Mode = DONE;

        					        		}
        							}

        					}



        				}



        			break;
        		case STOP:
        			Motors_stop();
        			if(ready2go == 5)
        			{
        				Mode = CRITICALPATH;
        			}
        			else if(ready2go==4)
        			{
        				LEDGreen();
        				Mode = STOP;
        			}
        			break;
        			case DONE:
        				Motors_stop();
        				if(ready2go == 4)
        				{
        					Mode = CRITICALPATH;
        				}
        				else if(ready2go == 3)
        				{
        					LEDGreen();
        					Mode = DONE;
        				}
        		break;
        			case COMPLETE:
        				LEDRed();
        				Motors_stop();
        				break;
        			case CRITICALPATH:
        				Orientation = NORTH;
        				Path = CriticalPath(CurrentGraph.body,SIZE,&startcell,endcell);
        						currentcell = startcell;
        				while(Path!= NULL)
        				{
        					direction = ReturnDirectionFromCellValue(&currentcell, &Path,&Orientation,&Dimension);
        				        						//if value is not NULL then update the current cell and drive

        				        						if(direction != 999)
        				        						{
        				        							DriveRobotWithDirection(&direction);
        				        							updateCurrentCell(&Orientation, &currentcell, &direction, &Dimension);
        				        							updateOrientation(&Orientation, &direction);


        				        						}
        				}
        				Mode = COMPLETE;
        			break;

        		} //end switch case


    } //end while(1)

}
void DriveRobotWithDirection(int* direction)
{
    switch(*direction){
        case LEFT:
            rotateLeft_Sense();
            forwardACell();
        break;
        case RIGHT:
            rotateRight_Sense();
            forwardACell();
        break;
        case FORWARD:
            forwardACell();
       break;
       case REVERSE:
           rotate180_Sense();
           forwardACell();
       break;
       default:
       break;
    }

}


void forwardACell(void){
	int sideLineCt=0;
    verticalAlign();
    InitialPinSet();
    clearCount();
    int initialForwardCount=NOBUMP;
    bypassFlag=0;
    CheckIR_Back();
    if(readingB>2220){
        initialForwardCount=1440;
    }
    while((bumpFlag != 1)&&(Encount<initialForwardCount)&&(readingF<WALLAHD)){
        ReadSensorsW();
        if(sideLine==1){
            initialForwardCount==BUMP;
        }
        FollowSequenceForward(vision,prevision);
        FollowSequenceBack(Bvision,Bprevision);
        CheckIR_Front();
    }
    /******************/
    Motors_stop();
    /*****************/
//    if(bumpFlag==1){
//        ReversePinSet();
//        Motors_shift(DV,DV,DV,DV);
//        encoderCounter_N(BUMPREVERSE);
//        InitialPinSet();
//        Motors_shift(DV,DV,DV,DV);
//        encoderCounter_N(BUMPFORWARD);
//    }
    clearCount();
    CheckIR_Front();
    if(readingF>=WALLAHD){
        bypassFlag=1;
    }
    //CheckIR();
    checkAnalogLine();
    while((readingLine<2990)&&(readingLineR<3900)&&(readingF<WALLAHD)&&(bypassFlag != 1)){
        ReadSensorsW();
        FollowSequenceForward(vision,prevision);
        FollowSequenceBack(Bvision,Bprevision);
        CheckIR_Front();
        checkAnalogLine();
    }

    Motors_stop();
    bumpFlag=0;
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_7);
    clearCount();
    flagCheck();
}

void flagCheck(void){
    if(checkFlag==0){
        Semaphore_post(RXSema);
    }
}


void cameraDirection(void){
    Motors_stop();
    FCamFlag=(readingF>500) ? 1 : 0 ;
    LCamFlag=(readingL>500) ? 1 : 0 ;
    RCamFlag=(readingR>500) ? 1 : 0 ;
    BCamFlag=(readingB>500) ? 1 : 0 ;

}

void cameraTurn(nbool visited){
    char st;
    if(visited == incorrect){
        /*in the case that there is a wall in front of the camera on it's previous camera face
        since the last cell, this switch case will allow the camera to take a picture of that
        wall without having to turn the camera on the typical priority order (Left, Front, Right)

        This switch case procedure will reduce any _CamFlag to zero, as the camera will have taken a picure
        of the wall in question, this taking it out of the typical priority call of the camera/servo rotations
        */

    if(LCamFlag){//LEFT
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (SERVOL*ui32LoadServo)/1000);
        Timer_start(timer3);//TIMER START
        Semaphore_pend(CameraSema,BIOS_WAIT_FOREVER);
//        UART_read(uartA,&st,1);//UARTA READ
        GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,0);
    }
    if(FCamFlag){//FRONT
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (SERVOF*ui32LoadServo)/1000);
        Timer_start(timer3);
        Semaphore_pend(CameraSema,BIOS_WAIT_FOREVER);
//        UART_read(uartA,&st,1);
        GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,0);
    }
    if(RCamFlag){//RIGHT
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (SERVOR*ui32LoadServo)/1000);//32
        Timer_start(timer3);
        Semaphore_pend(CameraSema,BIOS_WAIT_FOREVER);
//        UART_read(uartA,&st,1);
        GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,0);
    }
    if(BCamFlag){//BACK
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (SERVOB*ui32LoadServo)/1000);
        Timer_start(timer3);
        Semaphore_pend(CameraSema,BIOS_WAIT_FOREVER);
//        UART_read(uartA,&st,1);
        GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,0);
    }
    }

}


