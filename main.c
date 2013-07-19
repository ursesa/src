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
/*  I N C L U D E S                                                             */
/*==============================================================================*/


#include "globals.h"
#include <stm32.h>
#include "system_stm32f4xx.h"

#include <stdio.h>

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
/*                             Module Variables                                 */
/*==============================================================================*/

const uint8_t aSampleData[12] = {0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0xFF, 0x00, 0xFF};
__IO uint8_t aLoggedData[LOGGING_ARRAY_SIZE];
__IO uint16_t TimingDelay = 10;
__IO uint16_t DeadManDelay = DEADMAN_RELOAD;
__IO uint16_t LogArrayIndex = 0;

uint8_t TempVariable = 0;
uint8_t aTempArray[LOGGING_ARRAY_SIZE] = {};
char sTempString[] = " ";

LogA_Joystick_Position_TE JoystickPositionOLD = LogA_Joystick_Position_None;
MenuStatus_TE DeviceOperationMode = MenuStatus_NoPreselectionOrActivity;
LEDBlinking_TE LEDCurrentlyBlinking = LEDBlinking_None;
uint8_t LEDFlasherProRatio = 0;


/*==============================================================================*/
/*                            Module Functions                                  */
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

    LogA_DigOUT_Init();
    LogA_DigIN_Init();
    LogA_Joystick_Init();
    LogA_LEDs_Init();
    USART3_Init(9600);
	LCD_Initialize();
	delayMS(1);

	uint16_t NumberOfOutputXamples = 60;
	uint16_t TimeBetweenOutputXamplesMS = 200;
	LoopPatternOnDigOUT(TimeBetweenOutputXamplesMS, NumberOfOutputXamples);

	StartSamplingOnDigIN();		// for debugging and testing purposes start sampling and then activate the pattern
								// generator using the joystick

    while(1)
    {
    	LogA_Joystick_Position_TE JoystickPosition = LogA_Joystick_GetPosition();

    	if (JoystickPosition != JoystickPositionOLD)
    	{
    		DeadManDelay = DEADMAN_RELOAD;
    	}

    	JoystickPositionOLD = JoystickPosition;		// Save current joystick position for comparison at next iteration

    	if (DeadManDelay == 0)
    	{
    		//JoystickPosition = LogA_Joystick_Position_None;
    		DeviceOperationMode = MenuStatus_NoPreselectionOrActivity;
    	}

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
						LEDCurrentlyBlinking = LEDBlinking_ToggleAllFour;
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
						LoopPatternOnDigOUT(TimeBetweenOutputXamplesMS, NumberOfOutputXamples);
					}
					break;
					case MenuStatus_Preselection_Up :
					{
						DeviceOperationMode = MenuStatus_Activity_Up;
						LogA_LEDs_ClearAllFour();
						LCD_WriteString(1, 0, "Now Acquiring   ");
						LCD_WriteString(2, 1, "Data from DigIN ");
						LEDCurrentlyBlinking = LEDBlinking_Orange_Quickly;
						// StartSamplingOnDigIN();
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
						SendOLSDataToTerminal();
					}
					break;
					// Removes warnings if not all cases are handled
					default:
					break; // Do nothing
				}
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
			{
				if (DeviceOperationMode == MenuStatus_NoPreselectionOrActivity)
				{
					LEDCurrentlyBlinking = LEDBlinking_None;
					LEDFlasherProRatio++;
					if (LEDFlasherProRatio%16)
					{
						LogA_LEDs_ClearAllFour();
					}
					else
					{
						LogA_LEDs_LightAllFour();
					}
					LCD_WriteString(1, 0, "Move Joystick to");
					LCD_WriteString(2, 1, "Select an Action");
				}
			}
			break;

			for (uint8_t i = 0; i<LOGGING_ARRAY_SIZE; i++)
			{
				aTempArray[i] = aLoggedData[i];
			}
    	}
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



// NumberOfInputSamples is defined in the globals.h header:
// #define LOGGING_ARRAY_SIZE		100		// number of 8-bit samples to be stored in the offline logger array
// TimeBetweenInputSamples is also deinfined in globals.h:
// #define LOGGING_SAMPLING_GAP		3000
void StartSamplingOnDigIN(void)
{
    //TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

    /* Enable or disable TIM2 counters */
    TIM_Cmd(TIM2, ENABLE);
    //TIM_Cmd(TIM2, DISABLE);
}


void SendOLSDataToTerminal(void)
{
	USART3_PutS(";Rate: -1\r\n");
	USART3_PutS(";Channels: 8\r\n");
	USART3_PutS(";EnabledChannels: 255\r\n");

	for (uint8_t i = 0; i<LOGGING_ARRAY_SIZE; i++)
	{
		TempVariable = aLoggedData[i];				// this is a rather awkward way of doing the parsing
		sprintf(sTempString, "%X", TempVariable);	// but tinyprintf.c does not provide the same functionality
		USART3_PutS(sTempString);					// the 'normal' sprintf does
		USART3_PutS("@");
		sprintf(sTempString, "%i", i);
		USART3_PutS(sTempString);
		USART3_PutS("\r\n");
	}


//  sprintf(sTempString, "Hallo! \r\n Der Wert ist %X. \r\n", aLoggedData[LogArrayIndex]);
//  sprintf(sTempString, "Hallo! \r\n");

//  USART3_PutS("Halli");
}
