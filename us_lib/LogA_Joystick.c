//==================================================================================================
//! \brief     Joystick input.
//==================================================================================================
//! \verbatim
//! -----------------------------------------------------------------------------------------------
//! School   : HFU Hoehere Fachschule Uster
//! Subject  : Microcontroller
//! Topic    : LogA Logic Analyser
//! ------------------------------------------------------------------------------------------------
//!            Date         Name             Comment
//! Created  : 2008-10-01   Matthias Klaey   Created for HFU EduKit 2008
//! History  : 2010-09-20   Matthias Klaey   Combined positions added
//!          : 2013-07-14   Urban Seger      Adaptation for Log project
//! -----------------------------------------------------------------------------------------------
//! (c) Copyright 2013 Urban Seger / Matthias Klaey, HF Uster.
//! -----------------------------------------------------------------------------------------------
//! (c) Copyright 2008-2010 Matthias Klaey and HFU Hoehere Fachschule Uster. All Rights Reserved.
//! -----------------------------------------------------------------------------------------------
//! \endverbatim
//==================================================================================================
//! This module encapsulates joystick input, currently JS Series, 5208 model by E-Switch
//==================================================================================================


//==================================================================================================
// I N C L U D E S
//==================================================================================================

#include "LogA_Joystick.h"
#include "../me_lib/meGPIO_Init.h"


//==================================================================================================
// L O C A L   T Y P E S
//==================================================================================================

//! Joystick pins as bit struct.
//! This type is implemented locally within 'LogA_Joystick.c' because it is not relevant to the user
//! of the joystick module and because the pin setup is specific to the joystick of the project hardware
typedef struct
{
	int click	: 1;
	int right	: 1;
	int up		: 1;
	int left	: 1;
	int down	: 1;
}
LogA_Joystick_Pin_TS;

//! Joystick as union.
//! This type is implemented locally within 'LogA_Joystick.c' because it is not relevant to the user
//! of the joystick module and because the pin setup is specific to the joystick of the project hardware
typedef union
{
	LogA_Joystick_Position_TE position;
	LogA_Joystick_Pin_TS      pin;
}
LogA_Joystick_TU;


//==================================================================================================
// G L O B A L   F U N C T I O N   I M P L E M E N T A T I O N
//==================================================================================================

//--------------------------------------------------------------------------------------------------
// LogA_Joystick_Init
//--------------------------------------------------------------------------------------------------
void LogA_Joystick_Init(void)
{
	// Configure joystick input pins on GPIOD
    meGPIO_Init(JOYCLICK, GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(JOYRIGHT, GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(JOYUP, GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(JOYLEFT, GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(JOYDOWN, GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
}


//--------------------------------------------------------------------------------------------------
// LogA_Joystick_GetPosition
//--------------------------------------------------------------------------------------------------
LogA_Joystick_Position_TE LogA_Joystick_GetPosition(void)
{
	LogA_Joystick_TU joystick;
	joystick.position   = LogA_Joystick_Position_None;

	joystick.pin.click	= GPIO_ReadInputDataBit(JOYCLICK);
	joystick.pin.right	= GPIO_ReadInputDataBit(JOYRIGHT);
	joystick.pin.up		= GPIO_ReadInputDataBit(JOYUP);
	joystick.pin.left	= GPIO_ReadInputDataBit(JOYLEFT);
	joystick.pin.down	= GPIO_ReadInputDataBit(JOYDOWN);

	return (joystick.position);
}


//==================================================================================================
// End of file
//==================================================================================================
