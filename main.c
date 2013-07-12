/**
  ******************************************************************************
  * @file		main.c
  * @author		Urban Seger
  * @version	v0.1
  * @date		2013-07
  * @brief		Main file for the HFU MCU Logic Analyser student project.
  ******************************************************************************
  * @attention
  *
  * Adopted from E-LOAD/main.c
  * by Thomas Braun and Marc Lüthi
  * &copy; COPYRIGHT 2011 STMicroelectronics
  *
  ******************************************************************************
  */


/*==============================================================================*/
/*                          IMPORT                                              */
/*==============================================================================*/
#include "main.h"


/*==============================================================================*/
/** @brief      main
*/
int main(void)
{
	/**
	 *  IMPORTANT NOTE!
	 *  The symbol VECT_TAB_SRAM needs to be defined when building the project
	 *  if code has been located to RAM and interrupts are used.
	 *  Otherwise the interrupt table located in flash will be used.
	 *  See also the <system_*.c> file and how the SystemInit() function updates
	 *  SCB->VTOR register.
	 *  E.g.  SCB->VTOR = 0x20000000;
	 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0);

    setSysClock();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    SysTick_Config(SystemCoreClock / 100);


    Timer2Config();
    Timer2InterruptConfig();

    //TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

    /* TIM2 enable counters */
    TIM_Cmd(TIM2, ENABLE);

    USART3_Init(9600);

    delayMS(200);

    meGPIO_Init(LEDGREEN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(LEDORANGE, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(LEDRED, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(LEDBLUE, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);

    LogA_DigOUT_Init();

    while(1)
    {
    	delayMS(200);

    	// 1 1 1 1  1 1 . .  . . . .  . . . .
    	// 5 4 3 2  1 0 9 8  7 6 5 4  3 2 1 0
    	// - - - -  - - - -  - - - -  - - - -
    	// 1 1 1 0  1 0 1 1  0 1 1 0  0 0 0 0
    	// E        B        6        0
    	GPIO_Write(GPIOC, 0xEB60);

    	uint16_t TestVal = 0;
    	TestVal =  LogA_PortBits_Compactor(0xA240, 0xEB60);

    	USART3_PutS("Hallo Urban! \r\n Und mein Akku war gerade leer!!! \r\n");
    	//GPIO_ToggleBits(LEDORANGE);

    	delayMS(200);
    	GPIO_Write(GPIOC, 0x0000);


    }
}




void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
   {
     TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
     GPIO_ToggleBits(LEDORANGE);

     /* LED1 toggling with frequency = 73.24 Hz */
     /*STM_EVAL_LEDToggle(LED1);
         capture = TIM_GetCapture1(TIM2);
     TIM_SetCompare1(TIM2, capture + CCR1_Val);*/
   }
   /*else if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
   {
     TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

     // LED2 toggling with frequency = 109.8 Hz
     STM_EVAL_LEDToggle(LED2);
     capture = TIM_GetCapture2(TIM3);
     TIM_SetCompare2(TIM3, capture + CCR2_Val);
   }
   else if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
   {
     TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);

     // LED3 toggling with frequency = 219.7 Hz
     STM_EVAL_LEDToggle(LED3);
     capture = TIM_GetCapture3(TIM3);
     TIM_SetCompare3(TIM3, capture + CCR3_Val);
   }
   else
   {
     TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);

     // LED4 toggling with frequency = 439.4 Hz
     STM_EVAL_LEDToggle(LED4);
     capture = TIM_GetCapture4(TIM3);
     TIM_SetCompare4(TIM3, capture + CCR4_Val);
   }*/
 }
