/**
  ******************************************************************************
  * @file		LogA_DigOUT.c
  * @author		Urban Seger
  * @version	v0.1
  * @date		2013-07
  * @brief		This file provides set of firmware functions to manage output pins
  * 			available on the STM32F4-Discovery Kit from STMicroelectronics.
  ******************************************************************************
  * @attention
  *
  * Adopted from stm32f4_discovery.c
  * V1.1.1
  * by MCD Application Team
  * &copy; COPYRIGHT 2012 STMicroelectronics
  *
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "LogA_DigOUT.h"

/** @addtogroup Utilities
  * @{
  */ 

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */   
    
/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL 
  * @brief This file provides set of firmware functions to manage output pins
  *        available on the STM32F4-Discovery Kit from STMicroelectronics.
  * @{
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_Variables
  * @{
  */ 
GPIO_TypeDef* DIGOUT_PORT[OUTn] = {OUT1_GPIO_PORT, OUT2_GPIO_PORT, OUT3_GPIO_PORT, OUT4_GPIO_PORT,
								OUT5_GPIO_PORT, OUT6_GPIO_PORT, OUT7_GPIO_PORT, OUT8_GPIO_PORT};
const uint16_t DIGOUT_PIN[OUTn] = {OUT1_PIN, OUT2_PIN, OUT3_PIN, OUT4_PIN,
								OUT5_PIN, OUT6_PIN, OUT7_PIN, OUT8_PIN};
const uint32_t DIGOUT_CLK[OUTn] = {OUT1_GPIO_CLK, OUT2_GPIO_CLK, OUT3_GPIO_CLK, OUT4_GPIO_CLK,
								OUT5_GPIO_CLK, OUT6_GPIO_CLK, OUT7_GPIO_CLK, OUT8_GPIO_CLK};
NVIC_InitTypeDef   NVIC_InitStructure;

/**
  * @}
  */ 


/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_FunctionPrototypes
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_Functions
  * @{
  */ 

/**
  * @brief  Configures OUT GPIO.
  * @param  Out: Specifies the output to be configured.
  *   This parameter can be one of following parameters:
  *     @arg OUT1
  *     @arg OUT2
  *     @arg OUT3
  *     @arg OUT4
  *     @arg OUT5
  *     @arg OUT6
  *     @arg OUT7
  *     @arg OUT8
  * @retval None
  */
void LogA_DigOUT_Init(Out_TypeDef Out)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_OUT Clock */
  RCC_AHB1PeriphClockCmd(DIGOUT_CLK[Out], ENABLE);

  /* Configure the GPIO_OUT pin */
  GPIO_InitStructure.GPIO_Pin = DIGOUT_PIN[Out];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DIGOUT_PORT[Out], &GPIO_InitStructure);
}

/**
  * @brief  Turns selected OUT On.
  * @param  Out: Specifies the output to be set on.
  *   This parameter can be one of following parameters:
  *     @arg OUT1
  *     @arg OUT2
  *     @arg OUT3
  *     @arg OUT4
  *     @arg OUT5
  *     @arg OUT6
  *     @arg OUT7
  *     @arg OUT8
  * @retval None
  */
void LogA_DigOUT_On(Out_TypeDef Out)
{
  DIGOUT_PORT[Out]->BSRRL = DIGOUT_PIN[Out];
}

/**
  * @brief  Turns selected OUT Off.
  * @param  Out: Specifies the output to be set off.
  *   This parameter can be one of following parameters:
  *     @arg OUT1
  *     @arg OUT2
  *     @arg OUT3
  *     @arg OUT4
  *     @arg OUT5
  *     @arg OUT6
  *     @arg OUT7
  *     @arg OUT8
  * @retval None
  */
void LogA_DigOUT_Off(Out_TypeDef Out)
{
  DIGOUT_PORT[Out]->BSRRH = DIGOUT_PIN[Out];
}

/**
  * @brief  Toggles the selected OUT.
  * @param  Out: Specifies the output to be toggled.
  *   This parameter can be one of following parameters:
  *     @arg OUT1
  *     @arg OUT2
  *     @arg OUT3
  *     @arg OUT4
  *     @arg OUT5
  *     @arg OUT6
  *     @arg OUT7
  *     @arg OUT8
  * @retval None
  */
void LogA_DigOUT_Toggle(Out_TypeDef Out)
{
  DIGOUT_PORT[Out]->ODR ^= DIGOUT_PIN[Out];
}

/**
  * @}
  */ 

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
