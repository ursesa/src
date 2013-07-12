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
#include "me_lib/delay.h"
#include "me_lib/SysCLK.h"
#include "me_lib/meGPIO_Init.h"
#include "me_lib/USART3_Interface.h"

//todo #include "stm32f4xx.h"
//todo #include "stm32f4_discovery.h"
//todo #include "stm32f4xx_it.h"

#include "us_lib/LogA_DigOUT.h"
#include "us_lib/LogA_TimingLib.h"
//todo #include <stdio.h>


/* Private macro / DEFINITIONS/DECLARATIONS*/
#define LEDGREEN		GPIOD, GPIO_Pin_12
#define LEDORANGE		GPIOD, GPIO_Pin_13
#define LEDRED			GPIOD, GPIO_Pin_14
#define LEDBLUE			GPIOD, GPIO_Pin_15

#define DIGOUT_PIN1     GPIOC, GPIO_Pin_5
#define DIGOUT_PIN2     GPIOC, GPIO_Pin_6
#define DIGOUT_PIN3     GPIOC, GPIO_Pin_8
#define DIGOUT_PIN4     GPIOC, GPIO_Pin_9
#define DIGOUT_PIN5     GPIOC, GPIO_Pin_11
#define DIGOUT_PIN6     GPIOC, GPIO_Pin_13
#define DIGOUT_PIN7     GPIOC, GPIO_Pin_14
#define DIGOUT_PIN8		GPIOC, GPIO_Pin_15


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
void LogA_ToggleBlueLED(void);
uint16_t LogA_PortBits_Compacter(uint16_t ExpandVal, uint16_t Bitmask);
uint16_t LogA_PortBits_Kompakter(uint16_t ExpandVal, uint16_t Bitmask);

/* Private functions ---------------------------------------------------------*/


/**
  * @}
  */

/**
  * @}
  */


#endif /* MAIN_H_ */
