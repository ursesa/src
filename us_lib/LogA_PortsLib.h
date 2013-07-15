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

/** @addtogroup STM32F4_DISCOVERY_LOW_LEVEL_DIGITAL_OUT
  * @{
  */
#define DIGOUT_CHAN0	GPIOC, GPIO_Pin_5
#define DIGOUT_CHAN1	GPIOC, GPIO_Pin_6
#define DIGOUT_CHAN2	GPIOC, GPIO_Pin_8
#define DIGOUT_CHAN3	GPIOC, GPIO_Pin_9
#define DIGOUT_CHAN4	GPIOC, GPIO_Pin_11
#define DIGOUT_CHAN5	GPIOC, GPIO_Pin_13
#define DIGOUT_CHAN6	GPIOC, GPIO_Pin_14
#define DIGOUT_CHAN7	GPIOC, GPIO_Pin_15


 /** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Constants
   * @{
   */

 /** @addtogroup STM32F4_DISCOVERY_LOW_LEVEL_DIGIAL_IN
   * @{
   */
#define DIGIN_CHAN0		GPIOB, GPIO_Pin_2
#define DIGIN_CHAN1		GPIOB, GPIO_Pin_4
#define DIGIN_CHAN2		GPIOB, GPIO_Pin_5
#define DIGIN_CHAN3		GPIOB, GPIO_Pin_7
#define DIGIN_CHAN4		GPIOB, GPIO_Pin_8
#define DIGIN_CHAN5		GPIOB, GPIO_Pin_11
#define DIGIN_CHAN6		GPIOB, GPIO_Pin_12
#define DIGIN_CHAN7		GPIOB, GPIO_Pin_13

/**
  * @}
  */ 



/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Functions
  * @{
  */
void LogA_DigOUT_Init(void);
uint16_t LogA_PortBits_Compactor(uint16_t ExpandVal, uint16_t Bitmask);
uint16_t LogA_PortBits_Expander(uint16_t CompactVal, uint16_t Bitmask);


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
