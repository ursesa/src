/**
  ********************************************************************************************
  * @file		LogA_LEDs.c
  * @author		Urban Seger
  * @version	v0.1
  * @date		2013-07
  * @brief		This module encapsulates LED control on the STM32F4 Discovery Evaluation Board
  ********************************************************************************************
  * @attention
//==================================================================================================
//! \verbatim
//! -----------------------------------------------------------------------------------------------
//! School   : HFU Hoehere Fachschule Uster
//! Subject  : Microcontroller
//! Topic    : LogA Logic Analyser
//! ------------------------------------------------------------------------------------------------
//!            Date         Name             Comment
//! Created  : 2013-07-19   Urban Seger      Created to match LogA Hardware STM32F4 Discovery
//! -----------------------------------------------------------------------------------------------
//! (c) Copyright 2013 Urban Seger, Konolfingen
//! -----------------------------------------------------------------------------------------------
//! \endverbatim
//================================================================================================*/



//==================================================================================================
// I N C L U D E S
//==================================================================================================

#include "LogA_LEDs.h"




//==================================================================================================
// L O C A L   D E F I N E S
//==================================================================================================



//==================================================================================================
// L O C A L   F U N C T I O N   D E C L A R A T I O N
//==================================================================================================

//static void PrepareControlBusForInstructionWrite(void);


//==================================================================================================
// G L O B A L   F U N C T I O N   I M P L E M E N T A T I O N
//==================================================================================================

//--------------------------------------------------------------------------------------------------
// LEDs Initialisation
//--------------------------------------------------------------------------------------------------
void LogA_LEDs_Init(void)
{
    meGPIO_Init(LEDRED, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(LEDORANGE, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(LEDGREEN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(LEDBLUE, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
}


//--------------------------------------------------------------------------------------------------
// LEDs Static On/Off
//--------------------------------------------------------------------------------------------------
void LogA_LEDs_LightAllFour(void)
{
	GPIO_SetBits(LEDRED);
	GPIO_SetBits(LEDORANGE);
	GPIO_SetBits(LEDGREEN);
	GPIO_SetBits(LEDBLUE);
}

void LogA_LEDs_ClearAllFour(void)
{
	GPIO_ResetBits(LEDRED);
	GPIO_ResetBits(LEDORANGE);
	GPIO_ResetBits(LEDGREEN);
	GPIO_ResetBits(LEDBLUE);
}


//--------------------------------------------------------------------------------------------------
// LEDs Blinking Controls
//--------------------------------------------------------------------------------------------------

void LogA_ToggleActiveLED(void)
{
	switch (LEDCurrentlyBlinking)
	{
		case LEDBlinking_Red_Slowly :
		case LEDBlinking_Red_Quickly :
			{
				GPIO_ToggleBits(LEDRED);
				GPIO_ResetBits(LEDORANGE);
				GPIO_ResetBits(LEDGREEN);
				GPIO_ResetBits(LEDBLUE);
			}
			break;
		case LEDBlinking_Orange_Slowly :
		case LEDBlinking_Orange_Quickly :
			{
				GPIO_ResetBits(LEDRED);
				GPIO_ToggleBits(LEDORANGE);
				GPIO_ResetBits(LEDGREEN);
				GPIO_ResetBits(LEDBLUE);
			}
			break;
		case LEDBlinking_Green_Slowly :
		case LEDBlinking_Green_Quickly :
			{
				GPIO_ResetBits(LEDRED);
				GPIO_ResetBits(LEDORANGE);
				GPIO_ToggleBits(LEDGREEN);
				GPIO_ResetBits(LEDBLUE);
			}
			break;
		case LEDBlinking_Blue_Slowly :
		case LEDBlinking_Blue_Quickly :
			{
				GPIO_ResetBits(LEDRED);
				GPIO_ResetBits(LEDORANGE);
				GPIO_ResetBits(LEDGREEN);
				GPIO_ToggleBits(LEDBLUE);
			}
			break;
		case LEDBlinking_ToggleAllFour :
		{
			GPIO_ToggleBits(LEDRED);
			GPIO_ToggleBits(LEDORANGE);
			GPIO_ToggleBits(LEDGREEN);
			GPIO_ToggleBits(LEDBLUE);
		}
		break;
		// Covers LEDBlinking_None
		default:
			{
				GPIO_ResetBits(LEDRED);
				GPIO_ResetBits(LEDORANGE);
				GPIO_ResetBits(LEDGREEN);
				GPIO_ResetBits(LEDBLUE);
			}
			break;
	}
}


//==================================================================================================
// L O C A L   F U N C T I O N   I M P L E M E N T A T I O N
//==================================================================================================

//--------------------------------------------------------------------------------------------------
// Control Bus Access
//--------------------------------------------------------------------------------------------------

//static void PrepareControlBusForInstructionWrite(void)
//{
//	GPIO_WriteBit(LCD_RW, LCD_WRITE); // R/W = Write
//	GPIO_WriteBit(LCD_RS, LCD_INSTRUCTION); // RS  = Instruction
//	GPIO_WriteBit(LCD_EN, LCD_ENABLE);  // E   = Prepare
//}






//==================================================================================================
// End of file
//==================================================================================================
