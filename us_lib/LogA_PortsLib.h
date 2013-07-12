/*
 * LogA_PortsLib.h
 *
 *  Created on: Jul 12, 2013
 *      Author: Urban Seger
 */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOGA_PORTSLIB_H
#define __LOGA_PORTSLIB_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
#include "../me_lib/meGPIO_Init.h"
#include "stm32f4xx.h"
   


/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Constants
  * @{
  */ 

/** @addtogroup STM32F4_DISCOVERY_LOW_LEVEL_OUT
  * @{
  */
#define DIGOUT_PIN1     GPIOC, GPIO_Pin_5
#define DIGOUT_PIN2     GPIOC, GPIO_Pin_6
#define DIGOUT_PIN3     GPIOC, GPIO_Pin_8
#define DIGOUT_PIN4     GPIOC, GPIO_Pin_9
#define DIGOUT_PIN5     GPIOC, GPIO_Pin_11
#define DIGOUT_PIN6     GPIOC, GPIO_Pin_13
#define DIGOUT_PIN7     GPIOC, GPIO_Pin_14
#define DIGOUT_PIN8		GPIOC, GPIO_Pin_15


/**
  * @}
  */ 



/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Functions
  * @{
  */
void LogA_DigOUT_Init(void);
uint16_t LogA_PortBits_Compactor(uint16_t ExpandVal, uint16_t Bitmask);


/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __LOGA_PORTSLIB_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

 

/*****END OF FILE****/
