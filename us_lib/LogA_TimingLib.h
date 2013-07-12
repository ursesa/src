/*
 * LogA_TimingLib.h
 *
 *  Created on: Jul 7, 2013
 *      Author: Urban Seger
 */

#ifndef LOGA_TIMINGLIB_H_
#define LOGA_TIMINGLIB_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "main.h"

/* Variables -----------------------------------------------------------------*/

/** @addtogroup LogA_Modules
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void DelayBasedOnSysTick(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void Timer2Config(void);
void Timer2InterruptConfig(void);

#endif /* LOGA_TIMINGLIB_H_ */

/*****END OF FILE****/
