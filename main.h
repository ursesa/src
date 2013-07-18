/*
 * main.h
 *
 *  Created on: Jul 8, 2013
 *      Author: Urban Seger
 */

#ifndef MAIN_H_
#define MAIN_H_


/* Includes / IMPORT */
#include <stm32.h>
#include "system_stm32f4xx.h"

//todo #include <stdio.h>

#include "me_lib/delay.h"
#include "me_lib/SysCLK.h"
#include "me_lib/meGPIO_Init.h"
#include "me_lib/USART3_Interface.h"

#include "us_lib/LogA_PortsLib.h"
#include "us_lib/LogA_TimingLib.h"
#include "us_lib/LogA_LCD.h"


/* Private macro / DEFINITIONS/DECLARATIONS*/

#define LEDRED		GPIOD, GPIO_Pin_14	// LED5
#define LEDORANGE	GPIOD, GPIO_Pin_13	// LED3
#define LEDGREEN	GPIOD, GPIO_Pin_12	// LED4
#define LEDBLUE		GPIOD, GPIO_Pin_15	// LED6

#define JOYRIGHT	GPIOD, GPIO_Pin_1
#define JOYUP		GPIOD, GPIO_Pin_2
#define JOYLEFT		GPIOD, GPIO_Pin_3
#define JOYDOWN		GPIOD, GPIO_Pin_6
#define JOYCLICK	GPIOD, GPIO_Pin_0

#define PORTBITS_DIGOUT 		0xEB60	// on GPIOC
#define PORTBITS_DIGIN			0x39B4	// on GPIOB
#define PORTBITS_DISPLAY_DATA	0x7F80  // on GPIOE

	// 1 1 1 1  1 1 . .  . . . .  . . . .
	// 5 4 3 2  1 0 9 8  7 6 5 4  3 2 1 0
	// = = = =  = = = =  = = = =  = = = =
	// 1 1 1 0  1 0 1 1  0 1 1 0  0 0 0 0
	// E        B        6        0       @ DigOUT
	// - - - -  - - - -  - - - -  - - - -
	// 0 0 1 1  1 0 0 1  1 0 1 1  0 1 0 0
	// 3        9        B        4       @ DigIN
	// - - - -  - - - -  - - - -  - - - -
	// 0 1 1 1  1 1 1 1  1 0 0 0  0 0 0 0
	// 7        F        8        0       @ Display Data Bus
	// - - - -  - - - -  - - - -  - - - -


/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
static __IO uint32_t TimingDelay;
//volatile uint32_t UrbiCountTest;


/* Unused private variables --------------------------------------------------*/
//uint16_t PrescalerValue = 0;
//__IO uint8_t DemoEnterCondition = 0x00;
//__IO uint8_t UserButtonPressed = 0x00;
//__IO int8_t X_Offset, Y_Offset, Z_Offset  = 0x00;
//uint8_t Buffer[6];

/* Private function prototypes */


/* Private functions ---------------------------------------------------------*/
void LoopPatternOnDigOUT(uint16_t TimeBetweenSamplesMS, uint16_t NumberOfSamples);


/**
  * @}
  */

/**
  * @}
  */


#endif /* MAIN_H_ */
