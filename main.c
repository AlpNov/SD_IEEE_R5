
 /*
 * main.c
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
/* Example/Board Header files */
#include "Board.h"
#include "motor.h"
#include "cmd_int.h"
#include "movement.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "IRSensor.h"
#include "uart.h"
#include "line_sensor.h"
#include "CGraph.h"
#include "encoder_button.h"
#include <stdint.h>
#include "GlobalVar.h"
#include "inc/hw_types.h"
#include <stdint.h>
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
#include "piComm.h"
#include "IRBump.h"


int32_t Left,Center,Right;



int main(void){
    /* Call init functions */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0xCF;
    Board_initGeneral();
    Board_initGPIO();
    Board_initUART();
    BumpInit();
    LEDInit();
    ButtonInit();
    Motors_Init();
    LineInit();
    cmd_Init();
    piInit();
    IRSensorInit();
    UartInit();
    encoderInit();
    IRBumpInit();

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    UART_write(uart,"HELLO WORLD\r\n",sizeof("HELLO WORLD\r\n"));
    /* Start BIOS */
    BIOS_start();
    return (0);
}

