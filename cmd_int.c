/*
 * cmd_int.c
 *
 *  Created on: Feb 3, 2015
 *      Author: cmahn
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
#include "LED.h"
#include "movement.h"
#include "inc/hw_pwm.h"
#include "driverlib/pwm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_types.h"
#include "GlobalVar.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "piComm.h"
//#include <stdbool.h>

extern int encodeCt;
extern int32_t coun;
//extern UART_Handle uart;
//extern UART_Params uartParams;


extern UART_Handle uart;
extern UART_Params uartParams;

extern UART_Handle uartA;
extern UART_Params uartParamsA;
int temp=99;
int n=400;
char inputC1[3];
int32_t meh=0;

void cmd_Init(void){
    InitialPinSet();
}



const struct cmd_to_funct director[]=
{
        {"FW",&forwardACell,"-Move the robot forwards"},
        {"RV",&ReverseV,"-Move the robot backwards"},
        {"L1",&Left1Test,"-test the robot's Front Left wheel"},
        {"L2",&Left2Test,"-test the robot's Back Left wheel"},
        {"R1",&Right1Test,"-test the robot's Front Right wheel"},
        {"R2",&Right2Test,"-test the robot's Back Right wheel"},
        {"BK",&Brake,"-command to brake robot"},
        {"IP",&InitialPinSet,"-set direction pins for forward movemnt"},
        {"RP",&ReversePinSet,"-set directions pins for backward movemnt"},
        {"TT",&Test,"-test for incrementing PWM by 5% Pulse width each time"},
        {"TC",&TestClear,"-reset test PWM to initial test value"},
        {"RR",&rotateRight_Sense,"-Rotate the robot to the right until line seen"},
        {"RL",&rotateLeft_Sense,"-Rotate the robot to the left until line seen"},
        {"18",&rotate180_Sense,"-ROTATE 180 DEGREES"},
        {"SR",&shiftRight,"-Shift Robot to the Right"},
        {"SL",&shiftLeft,"-Shift Robot to the Left"},
        {"DS",&Drive_Start,"-COMMENCE DRIVE TASK"},
        {"CV",&checkVision,"-display line sensor readings"},
        {"LS",&LineStop,"-STOP DRIVE TASK"},
        {"DP",&DistancePrint,"-print the distance seen by IR Distance sensors"},
        {"DL",&diagonalLeft,"DIAGONAL LEFT"},
        {"DR",&diagonalRight,"DIAGONAL RIGHT"},
        {"BL",&backLeft,"Back Left"},
        {"BR",&backRight,"Back Right"},
        {"FL",&frontLeft,"Front Left"},
        {"FR",&frontRight,"Front Right"},
        {"VA",&verticalAlign,"VERTICAL ALIGN"},
        {"GE",&getencoder,"-Get encoder count"},
		{"ST",&ServoControl,"-Test the Servo movement"},
		{"st",&ServoTune,"Increment servo duty cycle"},
		{"PT",&picTest,"Test camera"},
        {"HP",&Help,"-Help command"},

        {"00",&Help,"-to do nothing.... but print help"}
};


void InterpretTask(char *tag){
        int i=0;
        while((director[i].cmd[0] != '0') && (director[i].cmd[1] != '0')){
            if((director[i].cmd[0] == tag[0]) && (director[i].cmd[1] == tag[1])){
            (*director[i].fn)();
            Semaphore_post(RXSema);
            return;
            }
            i++;
        }
        Semaphore_post(RXSema);
}



void RXCmdTask(void){
	char char_buffer;
	int char_count=0;
	while(1){
	    //read until encountering newline or Carriage return
		while( (char_buffer != '\n') && (char_buffer != '\r') ) {
			UART_read(uart, &char_buffer,1);
			//do nothing if char_count is larger than 1, but keep reading
			//it just won't store anything
			if (char_count > 1) {}
			else {
				inputC1[char_count++] = char_buffer;
			}
		}
		//reset char_count and char_buffer back to 0
		char_count =  char_buffer = 0;
		//UART_read(uart, &inputC1,2);
		//COMMENTED OUT CMDSEMA CAUSE IT DOESN'T DO ANYTHING
		//Semaphore_post(CmdSema);
		InterpretTask((char*)inputC1);
		Semaphore_pend(RXSema,BIOS_WAIT_FOREVER);
	}
}

void Forward(void){
        InitialPinSet();
        Motors_shift(60,60,60,60);
        encoderCounter_N(500);
        Semaphore_post(RXSema);
}

void ReverseV(void){
        ReversePinSet();
        Motors_shift(60,60,60,60);
        encoderCounter_N(500);
        Semaphore_post(RXSema);

}
//void Motors_shift(uint32_t l1, uint32_t l2, uint32_t r1, uint32_t r2){
void Left1Test(void){
        Motors_shift(60,90,90,90);
        encoderCounter_N(500);
        Semaphore_post(RXSema);

}

void Left2Test(void){
        Motors_shift(90,60,90,90);
        encoderCounter_N(500);
        Semaphore_post(RXSema);

}

void Right1Test(void){
        Motors_shift(90,90,60,90);
        encoderCounter_N(500);
        Semaphore_post(RXSema);

}

void Right2Test(void){
        Motors_shift(90,90,90,60);
        encoderCounter_N(500);
        Semaphore_post(RXSema);

}

void Brake(void){
        Motors_stop();
        Semaphore_post(RXSema);
}

void Test(void){
    ServoControl();
    Semaphore_post(RXSema);
}

void TestClear(void){
    temp=99;
    Motors_shift(temp,temp,temp,temp);
    Semaphore_post(RXSema);
}

void Drive_Start(void){
    Clock_start(clock0);
}


void Drive_Clock(void){
    Semaphore_post(DriveSema);
}

void LineStop(void){
    Clock_stop(clock0);
    Brake();
    UART_write(uart,"BRAKED\n",sizeof("BRAKED\n"));
    Semaphore_post(RXSema);
}

void DistancePrint(void){
    CheckIR();
    UART_write(uart,"\r\n",sizeof("\r\n"));
    UART_write(uart,"FRONT:\t",sizeof("FRONT: "));
    UART_print_int(readingF,uart);
    UART_write(uart,"\r\n",sizeof("\r\n"));
    UART_write(uart,"BACK:\t",sizeof("BACK: "));
    UART_print_int(readingB,uart);
    UART_write(uart,"\r\n",sizeof("\r\n"));
    UART_write(uart,"LEFT:\t",sizeof("LEFT: "));
    UART_print_int(readingL,uart);
    UART_write(uart,"\r\n",sizeof("\r\n"));
    UART_write(uart,"RIGHT:\t",sizeof("RIGHT: "));
    UART_print_int(readingR,uart);
    UART_write(uart,"\r\n",sizeof("\r\n"));
}

void sideLinePrint(void){
    checkFlag=5;
    Semaphore_post(SensorSemaAlpha);
}

void picTest(void){
    currentcell=50;
    takePic();
    char dummy;
    UART_read(uartA,&dummy,1);
    while(dummy != '~'){  UART_read(uartA,&dummy,1);}
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,0);
    currentcell=0;
}

void Help(void){
    UART_write(uart,"\r\n",sizeof("\r\n"));
    int i = 0;
    while ( (director[i].cmd[0] != '0') && (director[i].cmd[1] != '0' ) ) {
        UART_write(uart,director[i].cmd, sizeof(director[i].cmd));
        UART_write(uart,director[i].explain,sizeof(director[i].explain));
        UART_write(uart,"\r\n",sizeof("\r\n"));
        i++;
    }
}

void Nothin(void){
}

