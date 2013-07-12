/*==============================================================================*/
/** @file       main.c
    @brief      E-LOAD main File
    @author     Marc Luethi

*/


/*==============================================================================*/
/*                          IMPORT                                              */
/*==============================================================================*/
#include "main.h"

/*==============================================================================*/
/*                          DEFINITIONS/DECLARATIONS                            */
/*==============================================================================*/


/*==============================================================================*/
/*                          STATIC FUNCTION PROTOTYPES                          */
/*==============================================================================*/


/*==============================================================================*/
/*                          EXTERN VARIABLES                                    */
/*==============================================================================*/


/*==============================================================================*/
/*                          STATIC  VARIABLES                                   */
/*==============================================================================*/


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


    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

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

    meGPIO_Init(DIGOUT_PIN1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DIGOUT_PIN2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DIGOUT_PIN3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DIGOUT_PIN4, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DIGOUT_PIN5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DIGOUT_PIN6, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DIGOUT_PIN7, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DIGOUT_PIN8, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);

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
    	TestVal =  LogA_PortBits_Compacter(0xA240, 0xEB60);

    	uint16_t TestWal = 0;
    	TestWal =  LogA_PortBits_Kompakter(0xA240, 0xEB60);


    	USART3_PutS("Hallo Urban! \r\n Und mein Akku war gerade leer!!! \r\n");
    	//GPIO_ToggleBits(LEDORANGE);

    	delayMS(200);
    	GPIO_Write(GPIOC, 0x0000);

    }
}

uint16_t LogA_PortBits_Compacter(uint16_t ExpandVal, uint16_t Bitmask)
{
	uint16_t CompactVal = 0;
	uint8_t i;
	uint8_t zeroes = 0;
	
	// 1 1 1 1  1 1 . .  . . . .  . . . .
	// 5 4 3 2  1 0 9 8  7 6 5 4  3 2 1 0
	// - - - -  - - - -  - - - -  - - - -
	// 1 1 1 0  1 0 1 1  0 1 1 0  0 0 0 0
	// E        B        6        0
	for (i=0; i<16; i++)
	{
		if (Bitmask & 0x0001)
		{
			CompactVal >>= 1;
			if (ExpandVal & 0x0001)
			{
				CompactVal |= 0x8000;
			}
		}
		else
		{
			zeroes++;
		}
		ExpandVal >>= 1;
		Bitmask >>= 1;
	}
	CompactVal >>= zeroes;
	return(CompactVal);
}

uint16_t LogA_PortBits_Kompakter(uint16_t ExpandVal, uint16_t Bitmask)
{
	uint16_t CompactVal = 0;
	uint8_t pos = 0;

	// 1 1 1 1  1 1 . .  . . . .  . . . .
	// 5 4 3 2  1 0 9 8  7 6 5 4  3 2 1 0
	// - - - -  - - - -  - - - -  - - - -
	// 1 1 1 0  1 0 1 1  0 1 1 0  0 0 0 0
	// E        B        6        0
	for (uint8_t i=0; i<16; i++)
	{
		if (Bitmask & 0x01<<i)
		{
			if (ExpandVal & 0x01<<i)
			{
				CompactVal |= 0x01<<pos;
			}
		pos++;
		}
	}
	return CompactVal;
}


void LogA_ToggleBlueLED(void)
{
	GPIO_ToggleBits(LEDBLUE);
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
