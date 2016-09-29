/*
 * piComm.c
 *
 *  Created on: Apr 13, 2015
 *      Author: Daniel
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
#include "uart.h"
#include "inc/hw_pwm.h"
#include "driverlib/pwm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_types.h"
#include "GlobalVar.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

extern UART_Handle uartA;
extern UART_Params uartParamsA;

void piInit(void){
	//declare Port A, Pin 3 as an output pin
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_3);
    //set PortA, Pin 3 low: Raspberry Pi goes through the OCR when the pin is set high
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3, 0);


}

void takePic(void){//char cellNum[], int size){
	const char tildi = '~';
	//set high so the pi can take a picture
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_PIN_3);
    //UART_write(uartA,&tildi,1);
    //the '~' is used for the pi to know that that is the end of the string
    char cellNum[3];
    DecimaltoAscii(cellNum,currentcell,1);
    cellNum[2]='=';
//    UART_write(uartA,cellNum,3);
    Semaphore_post(CameraSema);
    //UART_write(uart,&tildi,sizeof(tildi)); // meaning that you can type in a command
    //set low so it may not take a picture
//    char st;
//    UART_read(uartA,&st,1);

	//wait until the value being read is a tildi; otherwirse, loop forever
//	while(strncmp(st,tildi,1)){
//		UART_read(uartA,&st,1);
//	}
	//UART_write(uart,&st,sizeof(st)); // meaning that you can type in a command
    //GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_3,0);
	//const char CR = 10;
	//const char NL = 13;
	//UART_write(uart,&CR,sizeof(CR));
	//UART_write(uart,&NL,sizeof(NL));
}
void DecimaltoAscii(char *buf, int n, int len){
    if (n>=10){
        DecimaltoAscii(buf,n/10,len-1);
        n=n%10;
    }
    else if (len>0){
        DecimaltoAscii(buf,0,len-1);
    }
    *(buf+len)=n+'0';
}


