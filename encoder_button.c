/*
 * encoder_button.c
 *
 *  Created on: Mar 6, 2015
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
#include "GlobalVar.h"

/* Example/Board Header files */
#include "Board.h"
#include "motor.h"
#include "uart.h"
#include "LED.h"
#include "encoder_button.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include <stdint.h>


int Encount_ready_flag=1; //"semaphore" that will let clear encoder wait until encoder ISR is read
int button_int_type;

int32_t prev=0;

void encoderInit(void){
       // GPIOIntRegister(GPIO_PORTA_BASE, encoder_ISR());
        //GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_2);
        //GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_INT_PIN_5|GPIO_INT_PIN_2,GPIO_BOTH_EDGES);
        GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2);
        GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_INT_PIN_2,GPIO_BOTH_EDGES);
        GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_2);
       // GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_0);
       // GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,~(GPIO_PIN_0));

        //HWI_startup
}

//need to set a timer at about 125 ms to debounce the switch
void ButtonInit(void){
		//Buton at pin A4, initialize high level interrupt
        GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4);
        GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_INT_PIN_4,GPIO_RISING_EDGE);
        //button_int_type = GPIO_HIGH_LEVEL;
        GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_4);
}

void BumpInit(void){
        //Buton at pin A4, initialize high level interrupt
        GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);
        GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_INT_PIN_7,GPIO_BOTH_EDGES);
        //button_int_type = GPIO_HIGH_LEVEL;
        GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_7);
}


void encoder_ISR(void){

	uint32_t HWI_mask = (GPIOIntStatus(GPIO_PORTA_BASE, 1));
	//Find which gpio pin interred
	GPIOIntClear(GPIO_PORTA_BASE,HWI_mask);
	if(HWI_mask & GPIO_PIN_4){
	    ready2go++;
	    if(ready2go==1){//setting the light green (READY STATE)
	        LEDGreen();

	    }
	    else if(ready2go==2){//STARTING THE ROBOT FOR MAPPING
	        Clock_start(clock0);
	        //Drive Task Starts here
	    }


	}
	 //  UART_write(uart,"interrupt occured\n",sizeof("interrupt occured\n"));
	//turn flag false to monopolize Encount
    //GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_2);
	if(HWI_mask & GPIO_PIN_2) {
		Encount_ready_flag = 0;
		prev++;
		if(prev==2){
			prev=0;
			Encount++;
		}
		//turn flag true to release Encount
		Encount_ready_flag = 1;
	}

    if(HWI_mask & GPIO_PIN_7) {
        GPIOIntDisable(GPIO_PORTA_BASE, GPIO_INT_PIN_7);
        Motors_stop();
        bumpFlag=1;
    }
}

void clearCount(void){
	//wait until Encount_ready_flag is true (Encount is not updated)
	while(Encount_ready_flag == 0);
    Encount=0;
}

void encoderCounter_N(int n){
    clearCount();
    while(Encount<n){

    }
    Motors_stop();
    clearCount();
}


int32_t getencoder(void) {
    UART_print_int(Encount,uart);
    return Encount;
}


