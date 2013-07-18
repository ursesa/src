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


// Module Constants
const uint8_t aSampleData[12] = {0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0xFF, 0x00, 0xFF};


// Module Functions

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
    //TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM2, DISABLE);

    USART3_Init(9600);

    delayMS(200);

    meGPIO_Init(LEDRED, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(LEDORANGE, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(LEDGREEN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(LEDBLUE, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);

    meGPIO_Init(JOYRIGHT, GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(JOYUP, GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(JOYLEFT, GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(JOYDOWN, GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(JOYCLICK, GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);

    LogA_DigOUT_Init();
	LCD_Initialize();
	delayMS(1);

	uint16_t NumberOfSamples = 5;
	uint16_t TimeBetweenSamplesMS = 300;
	LoopPatternOnDigOUT(TimeBetweenSamplesMS, NumberOfSamples);



    while(1)
    {

    	if (GPIO_ReadInputDataBit(JOYRIGHT))
    	{
    		GPIO_SetBits(LEDRED);
    	}
    	else
    	{
    		GPIO_ResetBits(LEDRED);
    	}

    	if (GPIO_ReadInputDataBit(JOYUP))
    	{
    		GPIO_SetBits(LEDORANGE);
    	}
    	else
    	{
    		GPIO_ResetBits(LEDORANGE);
    	}

    	if (GPIO_ReadInputDataBit(JOYLEFT))
    	{
    		GPIO_SetBits(LEDGREEN);
    	}
    	else
    	{
    		GPIO_ResetBits(LEDGREEN);
    	}

    	if (GPIO_ReadInputDataBit(JOYDOWN))
    	{
    		GPIO_SetBits(LEDBLUE);
    	}
    	else
    	{
    		GPIO_ResetBits(LEDBLUE);
    	}

    	if (GPIO_ReadInputDataBit(JOYCLICK))
    	{
    		GPIO_SetBits(LEDRED);
    		GPIO_SetBits(LEDORANGE);
    		GPIO_SetBits(LEDGREEN);
    		GPIO_SetBits(LEDBLUE);
    	}


//    	delayMS(200);


    	GPIO_Write(GPIOC, PORTBITS_DIGOUT);
    	delayMS(10);
    	GPIO_Write(GPIOC, 0x0000);
    	delayMS(10);


    	USART3_PutS("Hallo Urban! \r\n Und mein Akku war gerade leer!!! \r\n");
    	//GPIO_ToggleBits(LEDORANGE);


    	LCD_Clear();
		delayMS(50);

		LCD_WriteString(1, 3, "KILIAN");
		delayMS(50);

    	LCD_WriteString(2, 4, "SASKIA");
		delayMS(50);


    }
}




void LoopPatternOnDigOUT(uint16_t TimeBetweenSamplesMS, uint16_t NumberOfSamples)
{
	uint16_t j = 0;

	for (uint16_t i=0; i<=NumberOfSamples+1; i++)	// overflow of NumberOfSamples-1 in the case of
													// NumberOfSamples = 0 deliberate
	{
		if (!NumberOfSamples)
		{
			i=0; // force i to be reset at each iteration and provoke infinite loop
		}
		GPIO_Write(GPIOC, LogA_PortBits_Expander(aSampleData[i%12], PORTBITS_DIGOUT));
		delayMS(TimeBetweenSamplesMS);
		j++;
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
