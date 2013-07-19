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
  *
  ******************************************************************************
  */


/*==============================================================================*/
/*                          IMPORT                                              */
/*==============================================================================*/


#include "globals.h"
#include <stm32.h>
#include "system_stm32f4xx.h"

//todo #include <stdio.h>

#include "me_lib/delay.h"
#include "me_lib/SysCLK.h"
#include "me_lib/meGPIO_Init.h"
#include "me_lib/USART3_Interface.h"

#include "us_lib/LogA_PortsLib.h"
#include "us_lib/LogA_TimingLib.h"
#include "us_lib/LogA_Joystick.h"
#include "us_lib/LogA_LCD.h"
#include "us_lib/LogA_LEDs.h"


/*==============================================================================*/
/*                             Module Constants                                 */
/*==============================================================================*/

const uint8_t aSampleData[12] = {0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0xFF, 0x00, 0xFF};
uint32_t __IO TimingDelay = 100;


MenuStatus_TE DeviceOperationMode = MenuStatus_NoPreselectionOrActivity;
LEDBlinking_TE LEDCurrentlyBlinking = LEDBlinking_None;


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


//    delayMS(200);

    LogA_Joystick_Init();
    LogA_DigOUT_Init();
    LogA_LEDs_Init();
    USART3_Init(9600);
	LCD_Initialize();
	delayMS(1);

	uint16_t NumberOfOutputXamples = 5;
	uint16_t TimeBetweenOutputXamplesMS = 300;
	LoopPatternOnDigOUT(TimeBetweenOutputXamplesMS, NumberOfOutputXamples);



    while(1)
    {
    	LogA_Joystick_Position_TE JoystickPosition = LogA_Joystick_GetPosition();
    	switch (JoystickPosition)
    	{
			case LogA_Joystick_Position_Click	:
				{
					switch (DeviceOperationMode)
					{
						case MenuStatus_NoPreselectionOrActivity :
							{
								LCD_WriteString(1, 0, "Please Select a ");
								LCD_WriteString(2, 1, "Menu Item First ");
								delayMS(1500);
							}
							break;
						case MenuStatus_Preselection_Right :
							{
								DeviceOperationMode = MenuStatus_Activity_Right;
								LogA_LEDs_ClearAllFour();
								LCD_WriteString(1, 0, "Now Outputting  ");
								LCD_WriteString(2, 1, "Data on DigOUT  ");
								LEDCurrentlyBlinking = LEDBlinking_Red_Quickly;
								//todo LoopPatternOnDigOUT(TimeBetweenOutputXamplesMS, NumberOfOutputXamples);
								delayMS(1000);
							}
							break;
						case MenuStatus_Preselection_Up :
							{
								DeviceOperationMode = MenuStatus_Activity_Up;
								LogA_LEDs_ClearAllFour();
								LCD_WriteString(1, 0, "Now Acquiring   ");
								LCD_WriteString(2, 1, "Data from DigIN ");
								LEDCurrentlyBlinking = LEDBlinking_Orange_Quickly;
								//todo StartSamplingOnDigIN(TimeBetweenInputSamplesUS, NumberOfInputSamples);
								delayMS(1000);
							}
							break;
						case MenuStatus_Preselection_Left :
							{
								DeviceOperationMode = MenuStatus_Activity_Left;
								LogA_LEDs_ClearAllFour();
								LCD_WriteString(1, 0, " 7 6 5 4 3 2 1 0");
								LCD_WriteString(2, 1, " x . . x . . . x");
								LEDCurrentlyBlinking = LEDBlinking_Green_Quickly;
								//todo ShowLiveStatusOnLCD(................);
								delayMS(1000);
							}
							break;
						case MenuStatus_Preselection_Down :
							{
								DeviceOperationMode = MenuStatus_Activity_Down;
								LogA_LEDs_ClearAllFour();
								LCD_WriteString(1, 0, "Now Sending OLS ");
								LCD_WriteString(2, 1, "Data to Terminal");
								LEDCurrentlyBlinking = LEDBlinking_Blue_Quickly;
								//todo SendOLSDataToTerminal(................);
								delayMS(1000);
							}
							break;
						// Removes warnings if not all cases are handled
						default:
							break; // Do nothing
					}

			    	LCD_Clear();

				}
    		    break;
			case LogA_Joystick_Position_Right	:
				{
					DeviceOperationMode = MenuStatus_Preselection_Right;
					LogA_LEDs_ClearAllFour();
					GPIO_SetBits(LEDRED);
					LEDCurrentlyBlinking = LEDBlinking_Red_Slowly;
					LCD_WriteString(1, 0, "Click to Confirm");
					LCD_WriteString(2, 1, "Generator       ");
				}
				break;
			case LogA_Joystick_Position_Up :
				{
					DeviceOperationMode = MenuStatus_Preselection_Up;
					LogA_LEDs_ClearAllFour();
					GPIO_SetBits(LEDORANGE);
					LEDCurrentlyBlinking = LEDBlinking_Orange_Slowly;
					LCD_WriteString(1, 0, "Click to Confirm");
					LCD_WriteString(2, 1, "Logic Logger    ");
				}
				break;
			case LogA_Joystick_Position_Left :
				{
					DeviceOperationMode = MenuStatus_Preselection_Left;
					LogA_LEDs_ClearAllFour();
					GPIO_SetBits(LEDGREEN);
					LEDCurrentlyBlinking = LEDBlinking_Green_Slowly;
					LCD_WriteString(1, 0, "Click to Confirm");
					LCD_WriteString(2, 1, "Live Tracker    ");
				}
				break;
			case LogA_Joystick_Position_Down :
				{
					DeviceOperationMode = MenuStatus_Preselection_Down;
					LogA_LEDs_ClearAllFour();
					GPIO_SetBits(LEDBLUE);
					LEDCurrentlyBlinking = LEDBlinking_Blue_Slowly;
					LCD_WriteString(1, 0, "Click to Confirm");
					LCD_WriteString(2, 1, "OLS File Dump   ");
				}
				break;

			// Covers Joystick_Position_None
			default:
				break; // Do nothing
    	}


    	GPIO_Write(GPIOC, PORTBITS_DIGOUT);
    	delayMS(10);
    	GPIO_Write(GPIOC, 0x0000);
    	delayMS(10);


    	USART3_PutS("Hallo Urban! \r\n Und mein Akku war gerade leer!!! \r\n");
    	//GPIO_ToggleBits(LEDORANGE);


//    	LCD_Clear();
//		delayMS(50);
//
//		LCD_WriteString(1, 3, "KILIAN");
//		delayMS(50);
//
//    	LCD_WriteString(2, 4, "SASKIA");
//		delayMS(50);


    }
    return (0);
}




void LoopPatternOnDigOUT(uint16_t TimeBetweenOutputXamplesMS, uint16_t NumberOfOutputXamples)
{
	uint16_t j = 0;

	for (uint16_t i=0; i<=NumberOfOutputXamples+1; i++)	// Real output of X-amples will be 2 greater than
														// NumberOfOutputXamples but it a trick to handle
														// the value 0
	{
		if (!NumberOfOutputXamples)
		{
			i=0; // force i to be reset at each iteration and provoke infinite loop
		}
		GPIO_Write(GPIOC, LogA_PortBits_Expander(aSampleData[i%12], PORTBITS_DIGOUT));
		delayMS(TimeBetweenOutputXamplesMS);
		j++;
	}
}


void StartSamplingOnDigIN(uint16_t TimeBetweenInputSamplesUS, uint16_t NumberOfInputSamples)
{
	uint16_t test = TimeBetweenInputSamplesUS;
	uint16_t tset = NumberOfInputSamples;
	uint16_t res = 1;
	res += test-tset;
}


