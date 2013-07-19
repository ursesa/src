/*
 * LogA_Joystick.h
 *
 *  Created on: Jul 19, 2013
 *      Author: Urban Seger
 */

#ifndef LOGA_JOYSTICK_H_
#define LOGA_JOYSTICK_H_


#define JOYRIGHT	GPIOD, GPIO_Pin_1
#define JOYUP		GPIOD, GPIO_Pin_2
#define JOYLEFT		GPIOD, GPIO_Pin_3
#define JOYDOWN		GPIOD, GPIO_Pin_6
#define JOYCLICK	GPIOD, GPIO_Pin_0


//==================================================================================================
// G L O B A L   T Y P E S
//==================================================================================================

//! Joystick positions, value reflects individual switches.

typedef enum
{
	LogA_Joystick_Position_None          = 0x00,

	LogA_Joystick_Position_Click         = 0x01,

	LogA_Joystick_Position_Right         = 0x02,
	LogA_Joystick_Position_Up            = 0x04,
	LogA_Joystick_Position_Left          = 0x08,
	LogA_Joystick_Position_Down          = 0x10,


	// The current switch does neither support diagonal positions,
	// nor position plus Click combinations
//	LogA_Joystick_Position_Up_Left      = 0x0C,
//	LogA_Joystick_Position_Up_Right     = 0x06,
//	LogA_Joystick_Position_Down_Left    = 0x14,
//	LogA_Joystick_Position_Down_Right   = 0x12,
//
//	LogA_Joystick_Position_Click_Right	= 0x03,
//	LogA_Joystick_Position_Click_Up		= 0x05,
//	LogA_Joystick_Position_Click_Left	= 0x09,
//	LogA_Joystick_Position_Click_Down	= 0x11,

	// The following positions are mechanically not possible:
	// Click_Top_Right / Click_Top_Left / Click_Bottom_Right / Click_Bottom_Left
}
LogA_Joystick_Position_TE;


//==================================================================================================
// G L O B A L   F U N C T I O N   D E C L A R A T I O N
//==================================================================================================


//--------------------------------------------------------------------------------------------------
// LogA_Joystick_Initialize
//--------------------------------------------------------------------------------------------------
//! \brief     Initialises the joystick module.
//--------------------------------------------------------------------------------------------------
void LogA_Joystick_Init(void);


//--------------------------------------------------------------------------------------------------
// LogA_Joystick_GetPosition
//--------------------------------------------------------------------------------------------------
//! \brief     Returns current position of the joystick.
//!
//! \return    Current position of the joystick.
//--------------------------------------------------------------------------------------------------
LogA_Joystick_Position_TE LogA_Joystick_GetPosition(void);


#endif /* LOGA_JOYSTICK_H_ */
