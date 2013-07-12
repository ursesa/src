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


/* Private macro / DEFINITIONS/DECLARATIONS*/
#define LEDGREEN		GPIOD, GPIO_Pin_12
#define LEDORANGE		GPIOD, GPIO_Pin_13
#define LEDRED			GPIOD, GPIO_Pin_14
#define LEDBLUE			GPIOD, GPIO_Pin_15



/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
static __IO uint32_t TimingDelay;
volatile uint32_t UrbiCoutnTest;


/* Unused private variables --------------------------------------------------*/
//uint16_t PrescalerValue = 0;
//__IO uint8_t DemoEnterCondition = 0x00;
//__IO uint8_t UserButtonPressed = 0x00;
//__IO int8_t X_Offset, Y_Offset, Z_Offset  = 0x00;
//uint8_t Buffer[6];

/* Private function prototypes */


/* Private functions ---------------------------------------------------------*/


/**
  * @}
  */

/**
  * @}
  */


#endif /* MAIN_H_ */
