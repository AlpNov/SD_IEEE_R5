/*
 * Motor.C
 *
 *      Author: Christopher Ahn
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
/*Uart monitor files*/
#include <ti/sysbios/knl/Clock.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include <ti/drivers/UART.h>


/* Example/Board Header files */
#include "Board.h"

//#include <stdbool.h>
#include <stdint.h>
#include <ti/drivers/GPIO.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_pwm.h"
#include "driverlib/pwm.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "motor.h"
#include "movement.h"
#include "GlobalVar.h"
#include "uart.h"

#include "line_sensor.h"



#define LATOL 300
#define RATOL 3990

//Servo test range and variables
#define MINSERVOTEST 180
#define MAXSERVOTEST 200
#define INCSERVOTEST 2


unsigned long ulPeriod;
unsigned long distance;

extern int encodeCt;
extern UART_Handle uart;
extern UART_Params uartParams;

uint32_t PWM_FREQUENCY;
uint32_t ui32Load;
uint32_t ui32PWMClock;
volatile uint16_t ui8Adjust;
uint32_t Duty;
uint32_t DutyServo;

uint32_t SysClock;
int servoCt;
int servoTestDuty = MINSERVOTEST;



void Motors_Init(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,~(GPIO_PIN_0));//R1
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,0);//R2
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5,GPIO_PIN_5);
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,GPIO_PIN_7);//L1
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_PIN_4);//L2


    GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_6|GPIO_PIN_7,GPIO_DIR_MODE_OUT);
    GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_7,GPIO_DIR_MODE_OUT);
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_2|GPIO_PIN_3,GPIO_DIR_MODE_OUT);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2|GPIO_PIN_3);

    // PWM settings
    //50
    PWM_FREQUENCY = 1000; //to get a period of 2[ms]
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);//setting system clock at 80Mhz
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);// PWM clock at the modules is 80[Mhz]/64=1.25[Mhz]
    ui32PWMClock = SysCtlClockGet() / 64;// set up PWM clock frequency = 1.25[Mhz]
    ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1; // Calculate Load counter (this determines PWM period)

    ui32LoadServo=(ui32PWMClock /(4*50)) - 1;
    //Another way to set up Load is: Load=PWMperiod/Sytemclockperiod;

    /*
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PD1_M1PWM1);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Load);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, (0*ui32Load)/100);
    PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);
    PWMGenEnable(PWM1_BASE, PWM_GEN_0);
    */




    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);  // enable PWM module 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
    GPIOPinConfigure(GPIO_PB6_M0PWM0);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
    GPIOPinConfigure(GPIO_PB7_M0PWM1);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PF1_M1PWM5);

    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);// Set up Generator 3 as Count down mode
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);

    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ui32Load);//Set up period with calculated Load value
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32LoadServo);


    PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT|PWM_OUT_1_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);

    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (SERVOF*ui32LoadServo)/1000);
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    Motors_shift(BKE,BKE,BKE,BKE);
    clearCount();



}

void Motors_shift_Front(uint32_t l1, uint32_t r1){
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,(((l1*ui32Load)/100)));//PA6
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,(((r1*ui32Load)/100)));//PF2
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

}

void Motors_shift_Back(uint32_t l2, uint32_t r2){
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,(((l2*ui32Load)/100)));//PA7
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,(((r2*ui32Load)/100)));//PF7
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

}


void Motors_shift(uint32_t l1, uint32_t l2, uint32_t r1, uint32_t r2){
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,(((l1*ui32Load)/100)));//PA6
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,(((l2*ui32Load)/100)));//PA7
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,(((r1*ui32Load)/100)));//PF2
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,(((r2*ui32Load)/100)));//PF7
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

}


void Motors_stop(){
    //uint32_t BkVal=(99*ui32Load)/100;
    uint32_t BkVal=ui32Load-1;
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,BkVal);//PA6
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,BkVal);//PA7
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,BkVal);//PF2
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,BkVal);//PF7

    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);
}

void InitialPinSet(void){
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,~(GPIO_PIN_0));//R1
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,0);//R2
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,GPIO_PIN_7);//L1
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_PIN_4);//L2

}

void ReversePinSet(void){
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,(GPIO_PIN_0));//R1
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,GPIO_PIN_6);//R2
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,~(GPIO_PIN_7));//L1
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,~(GPIO_PIN_4));//L2
}

void RotateLeft(void){
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,~(GPIO_PIN_0));//R1
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,0);//R2
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,~(GPIO_PIN_7));//L1
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,~(GPIO_PIN_4));//L2
    Motors_shift(DV,DV,DV,DV);
    encoderCounter_N(100);
   // flagCheck();
}

void RotateRight(void){
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,(GPIO_PIN_0));//R1
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,GPIO_PIN_6);//R2
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,(GPIO_PIN_7));//L1
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,(GPIO_PIN_4));//L2
    Motors_shift(DV,DV,DV,DV);
    encoderCounter_N(50);
   // flagCheck();
}

void shiftLeft(void){
    int shiftLFlag=1;
    int shiftLCt=0;
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,~(GPIO_PIN_0));//R1
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,(GPIO_PIN_6));//R2
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,~(GPIO_PIN_7));//L1
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,(GPIO_PIN_4));//L2
    while(shiftLFlag){
        ReadSensorsW();
        if((vision!=000)&&(Bvision!=000)){
            shiftLCt++;
            if(shiftLCt>2){
                Brake();
                shiftLFlag=0;
            }
        }
        else{
            Motors_shift(DV,DV,DV,DV);
        }

    }
}

void shiftRight(void){
    int shiftRFlag=1;
    int shiftRCt=0;
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,(GPIO_PIN_0));//R1
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,~(GPIO_PIN_6));//R2
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,(GPIO_PIN_7));//L1
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,~(GPIO_PIN_4));//L2
    while(shiftRFlag){
        ReadSensorsW();
        if((vision!=000)&&(Bvision!=000)){
            shiftRCt++;
            if(shiftRCt>2){
                Brake();
                shiftRFlag=0;
            }
        }
        else{
            Motors_shift(DV,DV,DV,DV);
        }

    }
}

//GO OVER STRAFING DIRECTIONS FOR DIAGONAL MOVEMENT
//
//
// l1, l2,  r1,  r2
void veerRight(int vision){

    if(vision==001){
        diagonalRight(OVEERA);
    }
    else{
        diagonalRight(OVEERB);
    }
}

void veerLeft(int vision){

    if(vision==100){
        diagonalLeft(OVEERA);
    }
    else{
        diagonalLeft(OVEERB);
    }
}

void rotateRight_Sense(void){
    int flagRR=1;
    RotationFlag=0;
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,(GPIO_PIN_0));//R1
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,GPIO_PIN_6);//R2
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,(GPIO_PIN_7));//L1
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,(GPIO_PIN_4));//L2
    Motors_shift(ROTATE,ROTATE,ROTATE,ROTATE);
    encoderCounter_N(SHIFTN);
    while(flagRR){
        ReadSensorsW();
        if(vision != 000){
                Motors_stop();
                flagRR=0;
        }
        else{
         Motors_shift(ROTATE,ROTATE,ROTATE,ROTATE);
        }
    }
    Motors_stop();
}

void rotateLeft_Sense(void){
    int flagLL=1;
    RotationFlag=1;
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,0);//R1
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,0);//R2
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,0);//L1
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,0);//L2
    Motors_shift(ROTATE,ROTATE,ROTATE,ROTATE);
    encoderCounter_N(SHIFTN);
    while(flagLL){
        ReadSensorsW();
        if(vision != 000){
                Motors_stop();
                flagLL=0;
        }
        else{
            Motors_shift(ROTATE,ROTATE,ROTATE,ROTATE);
        }
    }
    Motors_stop();
}

void rotate180_Sense(void){
    int flag180=1;
    RotationFlag=1;
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,0);//R1
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,0);//R2
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,0);//L1
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,0);//L2
    Motors_shift(ROTATE,ROTATE,ROTATE,ROTATE);
    encoderCounter_N(2053);
    while(flag180){
        ReadSensorsW();
        if(vision != 000){
            Motors_stop();
            flag180=0;
        }
        else{
            Motors_shift(ROTATE,ROTATE,ROTATE,ROTATE);
        }
    }
    Motors_stop();
    flagCheck();
}

//uint32_t l1, uint32_t l2, uint32_t r1, uint32_t r2
void diagonalLeft(int n){
    InitialPinSet();
    Motors_shift(BKE,n,n,BKE);
   // flagCheck();
}

void diagonalRight(int n){
    InitialPinSet();
    Motors_shift(n,BKE,BKE,n);
   // flagCheck();
}


void frontForward(void){
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,~(GPIO_PIN_0));//R1
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,GPIO_PIN_7);//L1
    Motors_shift_Front(VEERA,VEERA);
   // flagCheck();
}


void frontLeft(void){
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,~(GPIO_PIN_0));//R1
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,~(GPIO_PIN_7));//L1
    int veerSpeed;
    veerSpeed=VEERA;
    ReadSensorsW();
    while((C==0)){
        Motors_shift_Front(veerSpeed,veerSpeed);
        ReadSensorsW();
    }
    if(checkFlag==0){
        Motors_stop();
    }
    else{
        frontForward();
    }
    flagCheck();
}


void frontRight(void){
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,(GPIO_PIN_0));//R1
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,(GPIO_PIN_7));//L1
    int veerSpeed;
    veerSpeed=VEERA;
    ReadSensorsW();
    while((C==0)){
        Motors_shift_Front(veerSpeed,veerSpeed);
        ReadSensorsW();
    }
    if(checkFlag==0){
        Motors_stop();
    }
    else{
        frontForward();
    }
    flagCheck();
}

void backForward(void){
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_PIN_4);//L2
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,0);//R2
    Motors_shift_Back(VEERA,VEERA);
  //  flagCheck();
}


void backLeft(void){
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,(GPIO_PIN_6));//R2
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,(GPIO_PIN_4));//L2
    int veerSpeed;
    veerSpeed=VEERA;
    ReadSensorsW();
    while((BC==0)){
        Motors_shift_Back(veerSpeed,veerSpeed);
        ReadSensorsW();
    }
    if(checkFlag==0){
        Motors_stop();
    }
    else{
        backForward();
    }
    flagCheck();
}


void backRight(void){
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,~(GPIO_PIN_6));//R2
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,~(GPIO_PIN_4));//L2
    int veerSpeed;
    veerSpeed=VEERA;
    ReadSensorsW();
    while((BC==0)){
        Motors_shift_Back(veerSpeed,veerSpeed);
        ReadSensorsW();
    }
    if(checkFlag==0){
        Motors_stop();
    }
    else{
        backForward();
    }
    flagCheck();
}

void ServoControl(void){

    switch(servoCt){
    case 0:
        	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (SERVOL*ui32LoadServo)/1000);
        	//PWMGenEnable(PWM1_BASE, PWM_GEN_2);
        	UART_write(uart,"LEFT\n",sizeof("LEFT\n"));
        	servoTestDuty=SERVOL-10;
        	break;
    case 1:
        	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (SERVOF*ui32LoadServo)/1000);
        	//PWMGenEnable(PWM1_BASE, PWM_GEN_2);
        	UART_write(uart,"FRONT\n",sizeof("FRONT\n"));
        	servoTestDuty=SERVOF-10;
        	break;

    case 2:
        	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (SERVOR*ui32LoadServo)/1000);
        	//PWMGenEnable(PWM1_BASE, PWM_GEN_2);
        	UART_write(uart,"RIGHT\n",sizeof("RIGHT\n"));
        	servoTestDuty=SERVOR-10;
        	break;
    case 3:
        	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (SERVOB*ui32LoadServo)/1000);
        	//PWMGenEnable(PWM1_BASE, PWM_GEN_2);
        	UART_write(uart,"BACK\n",sizeof("BACK\n"));
        	servoTestDuty=SERVOB-10;
        	servoCt=-1;
        	break;
    default:
    	break;
    }
	++servoCt;


}


void ServoTune(void) {
	int Servomaxtest;
	int Servomintest;
	switch(servoCt){
	case 1:
		Servomaxtest = SERVOL+10;
		Servomintest = SERVOL-10;
		break;
	case 2:
		Servomaxtest = SERVOF+10;
		Servomintest = SERVOF-10;
		break;
	case 3:
		Servomaxtest = SERVOR+10;
		Servomintest = SERVOR-10;
		break;
	case 0:
		Servomaxtest = SERVOB+10;
		Servomintest = SERVOB-10;
		break;
	default:
		break;
	}
	if ( (servoTestDuty>Servomaxtest) || (servoTestDuty<Servomintest) ){
		servoTestDuty=Servomintest;
	}
	UART_print_int(servoTestDuty,uart);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (servoTestDuty*ui32LoadServo)/1000);
	//PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	servoTestDuty += INCSERVOTEST;
}
