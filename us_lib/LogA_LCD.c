//==================================================================================================
//! \brief     LCD output.
//==================================================================================================
//! \verbatim
//! -----------------------------------------------------------------------------------------------
//! School   : HFU Hoehere Fachschule Uster
//! Subject  : Microcontroller
//! Topic    : LogA Logic Analyser
//! ------------------------------------------------------------------------------------------------
//!            Date         Name             Comment
//! Created  : 2008-10-01   Matthias Klaey   Created for HFU EduKit 2008
//! History  : 2012-05-06   Matthias Klaey   #define instead of const int
//!          : 201x-xx-xx   Thomas Braun     Alternative LCD code
//!          : 2013-07-14   Urban Seger      Omit any bus read function, adapt to match LogA IDE
//! -----------------------------------------------------------------------------------------------
//! (c) Copyright 2008-2013 Urban Seger / Thomas Braun / Matthias Klaey, HF Uster.
//! -----------------------------------------------------------------------------------------------
//! \endverbatim
//==================================================================================================
//! This module encapsulates LCD output to Newhaven NHD-0216KZW-AY5 using a xxxxx-compatible driver.
//==================================================================================================


//==================================================================================================
// I N C L U D E S
//==================================================================================================

#include "LogA_LCD.h"


//==================================================================================================
// L O C A L   D E F I N E S
//==================================================================================================

#define LCD_INSTRUCTION 0
#define LCD_DATA        1
#define LCD_READ        1
#define LCD_WRITE       0
#define LCD_ENABLE      1
#define LCD_DISABLE     0

// Data bus is connected to PE7 .. PE14
#define DATA_BUS_PORT	GPIOE
//#define DATA_BUS_BIT_OFFSET 7

#define LCD_RW GPIOE, GPIO_Pin_3
#define LCD_RS GPIOE, GPIO_Pin_4
#define LCD_EN GPIOE, GPIO_Pin_5
//#define LCD_BL GPIOc, GPIO_Pin_y

#define LCDDAT_CHAN0 GPIOE, GPIO_Pin_7
#define LCDDAT_CHAN1 GPIOE, GPIO_Pin_8
#define LCDDAT_CHAN2 GPIOE, GPIO_Pin_9
#define LCDDAT_CHAN3 GPIOE, GPIO_Pin_10
#define LCDDAT_CHAN4 GPIOE, GPIO_Pin_11
#define LCDDAT_CHAN5 GPIOE, GPIO_Pin_12
#define LCDDAT_CHAN6 GPIOE, GPIO_Pin_13
#define LCDDAT_CHAN7 GPIOE, GPIO_Pin_14


//==================================================================================================
// L O C A L   F U N C T I O N   D E C L A R A T I O N
//==================================================================================================

static void PrepareControlBusForInstructionWrite(void);
static void PrepareControlBusForDataWrite(void);
static void PrepareControlBusForBusyFlagRead(void); //todo
static void ExecuteDataBusOperation(void);
static void ResetControlBus(void);

static void SetDataBusAsOutput(void);
static void WriteDataBus(uint8_t byte);
static uint8_t ReadDataBus(void);

static void WriteInstruction(uint8_t instruction);
static void WriteData(uint8_t data);
//static uint8_t ReadData(void); //todo
static bool ReadBusyFlag(void);

static void SetFunction(void);
static void WaitForNotBusyFlag(void);

static void ReturnHome(void);
static void MoveCursorRight(void);
static void MoveCursor(int line, int pos);

//static void BacklightOn(void);
//static void BacklightOff(void);


//==================================================================================================
// G L O B A L   F U N C T I O N   I M P L E M E N T A T I O N
//==================================================================================================

//--------------------------------------------------------------------------------------------------
// LCD_Initialize
//--------------------------------------------------------------------------------------------------
void LCD_Initialize(void)
{
	meGPIO_Init(LCD_RW, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
	meGPIO_Init(LCD_RS, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
	meGPIO_Init(LCD_EN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
//	meGPIO_Init(LCD_BL, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);

	GPIO_WriteBit(LCD_RW, false); // R/W
	GPIO_WriteBit(LCD_RS, false); // RS
	GPIO_WriteBit(LCD_EN, false); // E

	SetDataBusAsOutput();
	delayMS(40); // ms

	SetFunction();
//	delayUS(500); // us

	LCD_Off();
//	delayUS(500); // us

	LCD_Clear();
//	delayUS(500); // us

	LCD_SetEntryMode(LCD_EntryMode_IncrementShiftRight);
//	delayUS(500); // us

	ReturnHome();
//	delayUS(500); // us

	LCD_On();
//	delayUS(500); // us
}


//--------------------------------------------------------------------------------------------------
// LCD_On/Off
//--------------------------------------------------------------------------------------------------
void LCD_On(void)
{
	WaitForNotBusyFlag();
	WriteInstruction(0x0C); // 0b0000'1DCB => 0b0000'1100 => 0x0C

	//BacklightOn();
}

void LCD_Off(void)
{
	//BacklightOff();

	WaitForNotBusyFlag();
	WriteInstruction(0x08); // 0b0000'1DCB => 0b0000'1000 => 0x08
}

//--------------------------------------------------------------------------------------------------
// LCD_SetEntryMode
//--------------------------------------------------------------------------------------------------
void LCD_SetEntryMode(LCD_EntryMode_T entryMode)
{
	WaitForNotBusyFlag();
	switch (entryMode)
	{
		case LCD_EntryMode_IncrementShiftRight:
		{
			WriteInstruction(0x06); // 0b0000'01IS => 0b0000'0110 => 0x06
			break;
		}
		case LCD_EntryMode_DecrementShiftLeft:
		{
			WriteInstruction(0x04); // 0b0000'01IS => 0b0000'0100 => 0x04
			break;
		}
		default:
		{
			// Do nothing
			break;
		}
	}
}

//--------------------------------------------------------------------------------------------------
// LCD_Clear
//--------------------------------------------------------------------------------------------------
void LCD_Clear(void)
{
	WaitForNotBusyFlag();
	WriteInstruction(0x01); // 0b0000'0001 => 0x01
}

//--------------------------------------------------------------------------------------------------
// LCD_WriteChar/String
//--------------------------------------------------------------------------------------------------
void LCD_WriteChar(int line, int pos, char c)
{
	MoveCursor(line, pos);

	WaitForNotBusyFlag();
	WriteData(c);
}

void LCD_WriteString(int line, int pos, const char* s)
{
	MoveCursor(line, pos);

	while (*s != '\0')
	{
		WaitForNotBusyFlag();
		WriteData(*s);
		s++;
	}
}


//==================================================================================================
// L O C A L   F U N C T I O N   I M P L E M E N T A T I O N
//==================================================================================================

//--------------------------------------------------------------------------------------------------
// Control Bus Access
//--------------------------------------------------------------------------------------------------
static void PrepareControlBusForInstructionWrite(void)
{
	GPIO_WriteBit(LCD_RW, LCD_WRITE); // R/W = Write
	GPIO_WriteBit(LCD_RS, LCD_INSTRUCTION); // RS  = Instruction
	GPIO_WriteBit(LCD_EN, LCD_ENABLE);  // E   = Prepare
}

static void PrepareControlBusForDataWrite(void)
{
	GPIO_WriteBit(LCD_RW, LCD_WRITE); // R/W = Write
	GPIO_WriteBit(LCD_RS, LCD_DATA);  // RS  = Data
	GPIO_WriteBit(LCD_EN, LCD_ENABLE);  // E   = Prepare
}


static void PrepareControlBusForBusyFlagRead(void)
{
	GPIO_WriteBit(LCD_RW, LCD_READ);  // R/W = Read
	GPIO_WriteBit(LCD_RS, LCD_INSTRUCTION); // RS  = Instruction
	GPIO_WriteBit(LCD_EN, LCD_ENABLE);  // E   = Prepare
}

static void ExecuteDataBusOperation(void)
{
	GPIO_WriteBit(LCD_EN, LCD_DISABLE); // E   = Execute

    // The 'enable pulse width' tPW must be at least 480 ns:
    delayUS(1); // us
}

static void ResetControlBus(void)
{
	GPIO_WriteBit(LCD_RW, LCD_WRITE);
	GPIO_WriteBit(LCD_RS, LCD_INSTRUCTION);
	GPIO_WriteBit(LCD_EN, LCD_DISABLE);
}

//--------------------------------------------------------------------------------------------------
// Data Bus Access
//--------------------------------------------------------------------------------------------------
static void SetDataBusAsOutput(void)
{
	//GPIO_WriteBit(LCDDATACHAN0, false);
	//GPIO_WriteBit(LCDDATACHAN1, false);
	//GPIO_WriteBit(LCDDATACHAN2, false);
	//GPIO_WriteBit(LCDDATACHAN3, false);
	//GPIO_WriteBit(LCDDATACHAN4, false);
	//GPIO_WriteBit(LCDDATACHAN5, false);
	//GPIO_WriteBit(LCDDATACHAN6, false);
	//GPIO_WriteBit(LCDDATACHAN7, false);
	meGPIO_Init(LCDDAT_CHAN0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
	meGPIO_Init(LCDDAT_CHAN1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
	meGPIO_Init(LCDDAT_CHAN2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
	meGPIO_Init(LCDDAT_CHAN3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
	meGPIO_Init(LCDDAT_CHAN4, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
	meGPIO_Init(LCDDAT_CHAN5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
	meGPIO_Init(LCDDAT_CHAN6, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
	meGPIO_Init(LCDDAT_CHAN7, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
}

static void WriteDataBus(uint8_t byte)
{
	uint16_t twobytes = 0x0000;
	twobytes = LogA_PortBits_Expander((uint16_t) byte, PORTBITS_DISPLAY_DATA);
	GPIO_Write(DATA_BUS_PORT, twobytes);
}

static uint8_t ReadDataBus(void)
{
	uint8_t byte = 0x00;
	uint16_t twobytes = 0x0000;
	twobytes = GPIO_ReadInputData(DATA_BUS_PORT);
	byte = (uint8_t) LogA_PortBits_Compactor(twobytes, PORTBITS_DISPLAY_DATA);
	return (byte);
}

//--------------------------------------------------------------------------------------------------
// Write/Read Instruction/Data
//--------------------------------------------------------------------------------------------------
static void WriteInstruction(uint8_t instruction)
{
	PrepareControlBusForInstructionWrite();
	WriteDataBus(instruction);
	ExecuteDataBusOperation();
	ResetControlBus();
}

static void WriteData(uint8_t data)
{
	PrepareControlBusForDataWrite();
	WriteDataBus(data);
	ExecuteDataBusOperation();
	ResetControlBus();
}

static bool ReadBusyFlag(void)
{
	uint8_t data = 0x00;

	PrepareControlBusForBusyFlagRead();
	ExecuteDataBusOperation();
	data = ReadDataBus();
	ResetControlBus();

	return (data >= 0x80);
}

//--------------------------------------------------------------------------------------------------
// Initialization
//--------------------------------------------------------------------------------------------------
static void SetFunction(void)
{
	//WaitForNotBusyFlag();
	WriteInstruction(0x3B); // 0b001D'10FF => 0b0011'1011 => 0x3B
							//FF: 00 Eng/Jap, 01 Eur1, 10 Eng/Russ, 11 Eur2
}

//--------------------------------------------------------------------------------------------------
// Busy flag
//--------------------------------------------------------------------------------------------------
static void WaitForNotBusyFlag(void)
{
	bool busyFlag = false;
	do
	{
		delayUS(80); // us
		busyFlag = ReadBusyFlag();
	}
	while (busyFlag);
}

//--------------------------------------------------------------------------------------------------
// Cursor control
//--------------------------------------------------------------------------------------------------
static void ReturnHome(void)
{
	WaitForNotBusyFlag();
	WriteInstruction(0x02); // 0b0000'001x => 0b0000'0010 => 0x02
}

static void MoveCursorRight(void)
{
	WaitForNotBusyFlag();
	WriteInstruction(0x14); // 0b0001'SRxx => 0b0001'0100 => 0x14
}

static void MoveCursor(int line, int pos)
{
	int i = 0;

	ReturnHome();

	pos += ((line - 1) * 0x40); // Offset of 0x40 per line (16 chars)
								// Line 1: Address 0x00 through 0x0F
								// Line 2: Address 0x40 through 0x4F
	for (i = 1; i < pos; i++)
		MoveCursorRight();
}

/*
//--------------------------------------------------------------------------------------------------
// Backlight control
//--------------------------------------------------------------------------------------------------
static void BacklightOn(void)
{
	GPIO_WriteBit(LCD_BL, true);
}

static void BacklightOff(void)
{
	GPIO_WriteBit(LCD_BL, false);
}
*/

//==================================================================================================
// End of file
//==================================================================================================
