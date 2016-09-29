/*
 * IRSensor.c
 *
 *  Created on: Feb 9, 2015
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
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "GlobalVar.h"

/* Example/Board Header files */
#include "Board.h"

// User created header files
#include "IRsensor.h"
#include "motor.h"
#include "cmd_int.h" //for function Brake



#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include <stdint.h>


void IRSensorInit(void){
		SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

		GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
		//Base address for module, Sameple Seq Number, Trigger Source, priority
		ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
        //base address for module, Sample Seq Number, Step to be configured, configuration (channel|Interupt enable|end select)
		ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
		//base address, step
		ADCSequenceEnable(ADC0_BASE, 3);
		//ADCIntEnable(ADC0_BASE, 3);

        GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
        //Base address for module, Sameple Seq Number, Trigger Source, priority
        ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
        //base address for module, Sample Seq Number, Step to be configured, configuration (channel|Interupt enable|end select)
        ADCSequenceStepConfigure(ADC0_BASE,0,0,ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);
        //base address, step
        ADCSequenceEnable(ADC0_BASE, 0);

        GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
        //Base address for module, Sameple Seq Number, Trigger Source, priority
        ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
        //base address for module, Sample Seq Number, Step to be configured, configuration (channel|Interupt enable|end select)
        ADCSequenceStepConfigure(ADC0_BASE,1,0,ADC_CTL_CH2|ADC_CTL_IE|ADC_CTL_END);
        //base address, step
        ADCSequenceEnable(ADC0_BASE, 1);

        GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);
        //Base address for module, Sameple Seq Number, Trigger Source, priority
        ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);
        //base address for module, Sample Seq Number, Step to be configured, configuration (channel|Interupt enable|end select)
        ADCSequenceStepConfigure(ADC0_BASE,2,0,ADC_CTL_CH3|ADC_CTL_IE|ADC_CTL_END);
        //base address, step
        ADCSequenceEnable(ADC0_BASE, 2);

}



void CheckIR(void){
	CheckIR_Front();
	CheckIR_Back();
	CheckIR_Left();
	CheckIR_Right();
}

void CheckIR_Front(void) {
	ADCProcessorTrigger(ADC0_BASE, 3);
	while(!ADCIntStatus(ADC0_BASE, 3, false)){
	}
	ADCIntClear(ADC0_BASE, 3);
	ADCSequenceDataGet(ADC0_BASE, 3, &readingF);
}

void CheckIR_Back(void) {
    ADCProcessorTrigger(ADC0_BASE, 0);
    while(!ADCIntStatus(ADC0_BASE, 0, false)){
    }
    ADCIntClear(ADC0_BASE, 0);
    ADCSequenceDataGet(ADC0_BASE, 0, &readingB);
}

void CheckIR_Left(void) {
    ADCProcessorTrigger(ADC0_BASE, 1);
    while(!ADCIntStatus(ADC0_BASE, 1, false)){
    }
    ADCIntClear(ADC0_BASE, 1);
    ADCSequenceDataGet(ADC0_BASE, 1, &readingL);
}

void CheckIR_Right(void) {
    ADCProcessorTrigger(ADC0_BASE, 2);
    while(!ADCIntStatus(ADC0_BASE, 2, false)){
    }
    ADCIntClear(ADC0_BASE, 2);
    ADCSequenceDataGet(ADC0_BASE, 2, &readingR);
}


void wallCheck(void){
    //CheckIR();
    if(readingR>2750){
        Brake();
        shiftLeft();
    }
    else if(readingL>3690){
        Brake();
        shiftRight();
    }

}



