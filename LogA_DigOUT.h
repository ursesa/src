/**
  ******************************************************************************
  * @file		LogA_DigOUT.h
  * @author		Urban Seger
  * @version	v0.1
  * @date		2013-07
  * @brief		This file contains definitions for STM32F4-Discovery Kit's
  * 			user-attributed digital output hardware resources.
  ******************************************************************************
  * @attention
  *
  * Adopted from stm32f4_discovery.h
  * V1.1.1
  * by MCD Application Team
  * &copy; COPYRIGHT 2012 STMicroelectronics
  *
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOGA_DIGOUT_H
#define __LOGA_DIGOUT_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
 #include "stm32f4xx.h"
   
/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32F4_DISCOVERY
  * @{
  */
      
/** @addtogroup STM32F4_DISCOVERY_LOW_LEVEL
  * @{
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Types
  * @{
  */
typedef enum 
{
  OUT1 = 0,
  OUT2 = 1,
  OUT3 = 2,
  OUT4 = 3,
  OUT5 = 4,
  OUT6 = 5,
  OUT7 = 6,
  OUT8 = 7,
} Out_TypeDef;



/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Constants
  * @{
  */ 

/** @addtogroup STM32F4_DISCOVERY_LOW_LEVEL_OUT
  * @{
  */
#define OUTn                             8	// There are 11 free connectors to GPIOC on the board
											// 3 are kept to be used as ADC inputs

#define OUT1_PIN                         GPIO_Pin_5
#define OUT1_GPIO_PORT                   GPIOC
#define OUT1_GPIO_CLK                    RCC_AHB1Periph_GPIOC
  
#define OUT2_PIN                         GPIO_Pin_6
#define OUT2_GPIO_PORT                   GPIOC
#define OUT2_GPIO_CLK                    RCC_AHB1Periph_GPIOC

#define OUT3_PIN                         GPIO_Pin_8
#define OUT3_GPIO_PORT                   GPIOC
#define OUT3_GPIO_CLK                    RCC_AHB1Periph_GPIOC

#define OUT4_PIN                         GPIO_Pin_9
#define OUT4_GPIO_PORT                   GPIOC
#define OUT4_GPIO_CLK                    RCC_AHB1Periph_GPIOC

#define OUT5_PIN                         GPIO_Pin_11
#define OUT5_GPIO_PORT                   GPIOC
#define OUT5_GPIO_CLK                    RCC_AHB1Periph_GPIOC

#define OUT6_PIN                         GPIO_Pin_13
#define OUT6_GPIO_PORT                   GPIOC
#define OUT6_GPIO_CLK                    RCC_AHB1Periph_GPIOC

#define OUT7_PIN                         GPIO_Pin_14
#define OUT7_GPIO_PORT                   GPIOC
#define OUT7_GPIO_CLK                    RCC_AHB1Periph_GPIOC

#define OUT8_PIN                         GPIO_Pin_15
#define OUT8_GPIO_PORT                   GPIOC
#define OUT8_GPIO_CLK                    RCC_AHB1Periph_GPIOC
/**
  * @}
  */ 


/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Macros
  * @{
  */  

/**
  * @}
  */ 


/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Functions
  * @{
  */
void LogA_DigOUT_Init(Out_TypeDef Out);
void LogA_DigOUT_On(Out_TypeDef Out);
void LogA_DigOUT_Off(Out_TypeDef Out);
void LogA_DigOUT_Toggle(Out_TypeDef Out);
/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __LOGA_DIGOUT_HH */
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
