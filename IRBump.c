/*
 * IRbump.c
 *
 *  Created on: Apr 16, 2015
 *      Author: annnguyen
 */

#include "IRBump.h"

#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include "inc/hw_timer.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "GlobalVar.h"
#include "Board.h"

// User created header files
#include "IRsensor.h"
#include "motor.h"
#include "cmd_int.h" //for function Brake



#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include <stdint.h>


void IRBumpInit(void){
	//Port D0, Pin J3.03, Channel 7, ADC1 sequence 2
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
	//Base address for module, Sameple Seq Number, Trigger Source, priority
	ADCSequenceConfigure(ADC1_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);

	//base address for module, Sample Seq Number, Step to be configured, configuration (channel|Interupt enable|end select)
	ADCSequenceStepConfigure(ADC1_BASE,2,0,ADC_CTL_CH7|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC1_BASE, 2);

}

void CheckIR_Bump(void) {
	ADCProcessorTrigger(ADC1_BASE, 2);
	while(!ADCIntStatus(ADC1_BASE, 2, false)){
	}
	ADCIntClear(ADC1_BASE, 2);
	ADCSequenceDataGet(ADC1_BASE, 2, &readingSpeedBump);
}
