/*
 * main.h
 *
 *  Created on: Jul 8, 2013
 *      Author: Urban Seger
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_


//==================================================================================================
// I N C L U D E S
//==================================================================================================

#include <stm32.h>


//==================================================================================================
// G L O B A L   D E F I N E S
//==================================================================================================

// 1 1 1 1  1 1 . .  . . . .  . . . .
// 5 4 3 2  1 0 9 8  7 6 5 4  3 2 1 0
// = = = =  = = = =  = = = =  = = = =
// 1 1 1 0  1 0 1 1  0 1 1 0  0 0 0 0
// E        B        6        0       @ DigOUT
// - - - -  - - - -  - - - -  - - - -
// 0 0 1 1  1 0 0 1  1 0 1 1  0 1 0 0
// 3        9        B        4       @ DigIN
// - - - -  - - - -  - - - -  - - - -
// 0 1 1 1  1 1 1 1  1 0 0 0  0 0 0 0
// 7        F        8        0       @ Display Data Bus
// - - - -  - - - -  - - - -  - - - -

#define PORTBITS_DIGOUT 		0xEB60	// on GPIOC
#define PORTBITS_DIGIN			0x39B4	// on GPIOB
#define PORTBITS_DISPLAY_DATA	0x7F80  // on GPIOE


//==================================================================================================
// G L O B A L   T Y P E S
//==================================================================================================

//! Menu selection and execution status
typedef enum
{
	MenuStatus_NoPreselectionOrActivity = 0x00,

	MenuStatus_Preselection_Right		= 0x01,
	MenuStatus_Preselection_Up			= 0x02,
	MenuStatus_Preselection_Left		= 0x04,
	MenuStatus_Preselection_Down		= 0x08,

	MenuStatus_Activity_Right			= 0x10,
	MenuStatus_Activity_Up				= 0x20,
	MenuStatus_Activity_Left			= 0x40,
	MenuStatus_Activity_Down			= 0x80,
}
MenuStatus_TE;


//! LED selection and blinking speed
typedef enum
{
	LEDBlinking_None 					= 0x00,

	LEDBlinking_Red_Slowly				= 0x01,
	LEDBlinking_Orange_Slowly			= 0x02,
	LEDBlinking_Green_Slowly			= 0x04,
	LEDBlinking_Blue_Slowly				= 0x08,

	LEDBlinking_Red_Quickly				= 0x10,
	LEDBlinking_Orange_Quickly			= 0x20,
	LEDBlinking_Green_Quickly			= 0x40,
	LEDBlinking_Blue_Quickly			= 0x80,
}
LEDBlinking_TE;


//==================================================================================================
// G L O B A L   V A R I A B L E S
//==================================================================================================

extern __IO uint32_t TimingDelay;
extern MenuStatus_TE DeviceOperationMode;
extern LEDBlinking_TE LEDCurrentlyBlinking;




/* Unused private variables --------------------------------------------------*/
//volatile uint32_t UrbiCountTest;
//GPIO_InitTypeDef GPIO_InitStructure;
//uint16_t PrescalerValue = 0;
//__IO uint8_t DemoEnterCondition = 0x00;
//__IO uint8_t UserButtonPressed = 0x00;
//__IO int8_t X_Offset, Y_Offset, Z_Offset  = 0x00;
//uint8_t Buffer[6];

/* Private function prototypes */


/* Private functions ---------------------------------------------------------*/
extern void LoopPatternOnDigOUT(uint16_t TimeBetweenOutputXamplesMS, uint16_t NumberOfOutputXamples);
extern void StartSamplingOnDigIN(uint16_t TimeBetweenInputSamplesUS, uint16_t NumberOfInputSamples);


#endif /* GLOBALS_H_ */
