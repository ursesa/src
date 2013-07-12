/**
  ******************************************************************************
  * @file		LogA_TimingLib.c
  * @author		Urban Seger
  * @version	v0.1
  * @date		2013-07
  * @brief		This file provides set of functions to manage timing on the
  * 			STM32F4-Discovery Kit from STMicroelectronics.
  ******************************************************************************
  * @attention
  *
  * Adopted from SysTick/main.c
  * V1.0.0
  * by MCD Application Team
  * &copy; COPYRIGHT 2011 STMicroelectronics
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "LogA_TimingLib.h"


/* Functions ---------------------------------------------------------*/

  /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f4xx.c file
     */

  /* Setup SysTick Timer for 1 msec interrupts.
     ------------------------------------------
    1. The SysTick_Config() function is a CMSIS function which configure:
       - The SysTick Reload register with value passed as function parameter.
       - Configure the SysTick IRQ priority to the lowest value (0x0F).
       - Reset the SysTick Counter register.
       - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
       - Enable the SysTick Interrupt.
       - Start the SysTick Counter.

    2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
       SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
       SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
       inside the misc.c file.

    3. You can change the SysTick IRQ priority by calling the
       NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function
       call. The NVIC_SetPriority() is defined inside the core_cm4.h file.

    4. To adjust the SysTick time base, use the following formula:

         Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)

       - Reload Value is the parameter to be passed for SysTick_Config() function
       - Reload Value should not exceed 0xFFFFFF
   */

//***************************************************************************************
// NOTE: In stm32f4xx_it.h, the SysTick_Handler interrupt handling routine must be active
//***************************************************************************************

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, based on SysTick settings.
  * @retval None
  */
void DelayBasedOnSysTick(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

/***********
  THE FOLLOWING TIMERS ARE CONFIGURED USING THE me_lib LIBRARY
  ***********/

/**
  * @brief  Configure timer 2.
  * @param  None
  * @retval None
  */
void Timer2Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    TIM_TimeBaseStructure.TIM_Period = 0x4AF;
    TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/1680) - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef  TIM_OCInitStructure;

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 40961;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    /* Clear TIM2 update pending flags */
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}


/**
  * @brief  Configure timer 2 interrupt handling.
  * @param  None
  * @retval None
  */
void Timer2InterruptConfig(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
