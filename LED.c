/*
 * LED.c
 *
 *  Created on: Apr 13, 2015
 *      Author: annnguyen
 */

#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

#include "GlobalVar.h"
#include "line_sensor.h"
#include "LED.h"

int ready_count;//used to determine whether robot has been positioned correctly long enough
int status_flag=0;

void LEDInit(void) {
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5);
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5,0);
}

void LEDToggle(void) {
	if ( GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5) )
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5,0);
	else
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5,GPIO_PIN_5);
}

void LEDRed(void) {
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5,0);
}

void LEDGreen(void) {
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5,GPIO_PIN_5);
}
