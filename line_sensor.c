/*
 * line_sensor.c
 *
 *  Created on: Feb 17, 2015
 *      Author: cmahn
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
#include "driverlib/adc.h"

/* Example/Board Header files */
#include "Board.h"
#include "motor.h"
#include "uart.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "IRSensor.h"
#include "GlobalVar.h"
#include "line_sensor.h"
#include <stdint.h>

extern uint32_t ui32LoadServo;

unsigned long dist;

char inputC2[]=" ";
char inputC3[]="\n";
//extern UART_Handle uart;
//extern UART_Params uartParams;

int blankCount;

void LineInit(void){
    analogLineSensorInit();
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,(GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5));
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5,(GPIO_PIN_4|GPIO_PIN_5));
    GPIOPinWrite(GPIO_PORTB_BASE,(GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5),(GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5));
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_4|GPIO_PIN_5,(GPIO_PIN_4|GPIO_PIN_5));
    InitialPinSet();

}

void analogLineSensorInit(void){
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

        GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2);
//Base address for module, Sameple Seq Number, Trigger Source, priority
        ADCSequenceConfigure(ADC1_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
        //base address for module, Sample Seq Number, Step to be configured, configuration (channel|Interupt enable|end select)
        ADCSequenceStepConfigure(ADC1_BASE,0,0,ADC_CTL_CH5|ADC_CTL_IE|ADC_CTL_END);
        //base address, step
        ADCSequenceEnable(ADC1_BASE, 0);

        GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);
        ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
        //base address for module, Sample Seq Number, Step to be configured, configuration (channel|Interupt enable|end select)
        ADCSequenceStepConfigure(ADC1_BASE,1,0,ADC_CTL_CH4|ADC_CTL_IE|ADC_CTL_END);
        //base address, step
        ADCSequenceEnable(ADC1_BASE, 1);
}


void checkAnalogLine(void){
    uint32_t ANGReadLine[1];
    uint32_t ANGReadLineR[1];


    //BASE module address, sequence number
    ADCProcessorTrigger(ADC1_BASE, 0);
    while(!ADCIntStatus(ADC1_BASE, 0, false)){
    }
    ADCIntClear(ADC1_BASE, 0);
    //base address, step, pointer to array
    ADCSequenceDataGet(ADC1_BASE, 0, ANGReadLine);
    readingLine=ANGReadLine[0];

    ADCProcessorTrigger(ADC1_BASE, 1);
    while(!ADCIntStatus(ADC1_BASE, 1, false)){
    }
    ADCIntClear(ADC1_BASE, 1);
    //base address, step, pointer to array
    ADCSequenceDataGet(ADC1_BASE, 1, ANGReadLineR);
    readingLineR=ANGReadLineR[0];
}

void printAnalogLine(void){
    checkAnalogLine();
    UART_write(uart,"Analog Line Left: \n",sizeof("Analog Line Left: \n"));
    UART_print_int(readingLine,uart);
    UART_write(uart,"\n",sizeof("\n"));
    UART_write(uart,"Analog Line Right: \n",sizeof("Analog Line Right: \n"));
    UART_print_int(readingLineR,uart);
    UART_write(uart,"\n",sizeof("\n"));
}

void LineClock(void){
    //Semaphore_post(SensorSemaAlpha);
}


void ReadSensorsW(void){
            //set the line sensor pins to output
            GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5);
            GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,(GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5));
            GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_4|GPIO_PIN_5);
            GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5,(GPIO_PIN_4|GPIO_PIN_5));
            GPIOPinWrite(GPIO_PORTB_BASE,(GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5),(GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5));
            GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_4|GPIO_PIN_5,(GPIO_PIN_4|GPIO_PIN_5));
            //set timer for 10 us for charging Line Sensors
            Timer_start(timer1);
            //wait for Semaphore Post for SensorSema
            //Clock_stop(clock1);
            Semaphore_pend(SensorSema,BIOS_WAIT_FOREVER);



}

void LineSensorInput_ISR(void){
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,(GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5));
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,GPIO_PIN_4|GPIO_PIN_5);
    Timer_start(timer2);


}

void LineSensorRead_ISR(void){

    // using 6-line sensor
    /*Begin reading sensors, then setting sensor value to binary 1 or 0*/
		prevision=vision;
        checkAnalogLine();
        L=GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_3);
        L=(L==8) ? 1 : 0 ;
        R=GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_5);
        R=(R==32) ? 1 : 0 ;
        C=GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_4);
        C=(C==16) ? 1 : 0 ;

        sideLine=GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_4);
        sideLine=(sideLine==16) ? 1 : 0 ;

        //sideLineDetect();
        vision=calcVision(L,C,R);

        Bprevision=Bvision;
        BL=GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_5);
        BL=(BL==32) ? 1 : 0 ;
        BR=GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_4);
        BR=(BR==16) ? 1 : 0 ;
        BC=GPIOPinRead(GPIO_PORTE_BASE,GPIO_PIN_5);
        BC=(BC==32) ? 1 : 0 ;


        Bvision=calcBVision(BL,BC,BR);
        /*End Reading line sensors*/
        Semaphore_post(SensorSema);

}

void FollowSequenceForward(int vision, int prevision){
    if(vision==101){
        vision=111;
    }
    else if(vision==010){
        if(prevision==0){
            vision=0;
        }
        else{
            vision==111;
        }
    }
    switch (vision){
        case 000:
        	if(prevision==0){
        		if(lost==1){
        			frontLeft();
        		}
        		else if(lost==0){
        			frontRight();
        		}
        	}
            if(prevision<12){
                frontRight();
            }
            else if(prevision>99){
                frontLeft();
            }
        break;
        case 111:
           frontForward();
        break;
        case 100:
            frontLeft();
            lost=1;
        break;
        case 110:
            frontLeft();
            lost=1;
        break;
        case 001:
            frontRight();
            lost=0;
        break;
        case 011:
            frontRight();
            lost=0;
        break;
        default:
            frontForward();
        break;
    }
}

void FollowSequenceBack(int Bvision, int Bprevision){
    if(Bvision==101){
        Bvision=111;
    }
    else if(Bvision==010){
        if(Bprevision==0){
            Bvision==0;
        }
        else{
            Bvision=111;
        }
    }
    switch (Bvision){
    case 000:
        if(Bprevision<12){
            backRight();
        }
        else if(Bprevision>99){
            backLeft();
        }
        else if(Bprevision==0){
            if(Blost==1){
                backLeft();
            }
            else if(Blost==0){
                backRight();
            }
        }
    break;
        case 111:
           backForward();
        break;
        case 100:
            backLeft();
            Blost=1;
        break;
        case 110:
            backLeft();
            Blost=1;
        break;
        case 001:
            backRight();
            Blost=0;
        break;
        case 011:
            backRight();
            Blost=0;
        break;
        default:
            backForward();
        break;
    }
}

void checkVision(void){
    ReadSensorsW();
    printVision();
    printVision_side();
    printAnalogLine();
    Semaphore_post(RXSema);
}

void printVision(void){
    UART_write(uart,"\r\n",sizeof("\r\n"));
    UART_write(uart,"FRONT-LEFT CENTER RIGHT\r\n",sizeof("FRONT-LEFT CENTER RIGHT\r\n"));
    UART_print_int(L,uart);
    UART_print_int(C,uart);
    UART_print_int(R,uart);
    UART_write(uart,"\r\n",sizeof("\r\n"));
    UART_write(uart,"BACK-LEFT CENTER RIGHT\r\n",sizeof("BACK-LEFT CENTER RIGHT\r\n"));
    UART_print_int(BL,uart);
    UART_print_int(BC,uart);
    UART_print_int(BR,uart);
    UART_write(uart,"\r\n",sizeof("\r\n"));
}

void printVision_side(void){
    UART_write(uart,"\r\n",sizeof("\r\n"));
    UART_write(uart,"SIDE: \n",sizeof("SIDE: \n"));
    UART_print_int(sideLine,uart);
    UART_write(uart,"\r\n",sizeof("\r\n"));
}


int calcVision(int L, int C, int R){
    vision=L*100;
    vision=vision+(C*10);
    vision=vision+R;
    return vision;
}

int calcBVision(int L, int C, int R){
    Bvision=L*100;
    Bvision=Bvision+(C*10);
    Bvision=Bvision+R;
    return Bvision;
}

