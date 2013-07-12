/*==============================================================================*/
/** @file       LCD.c
    @brief      LCD Control
    @author     Thomas Braun
    @note
*/


/*==============================================================================*/
/*                          IMPORT                                              */
/*==============================================================================*/
#define USE_STM32LIB
#include <me_lib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "LCD.h"

/*==============================================================================*/
/*                          DEFINITIONS/DECLARATIONS                            */
/*==============================================================================*/
#define LCD_INSTRUCTION 0
#define LCD_DATA        1
#define LCD_READ        1
#define LCD_WRITE       0
#define LCD_ENABLE      1
#define LCD_DISABLE     0
#define LCD_wait() delayUS(10)

#define DATA_PORT  GPIOE
#define MASK  (GPIO_ReadInputData(DATA_PORT) & 0xFF00)
#define CONV_CHAR(A,B,C,D,E) (0x00 | ((A<<4) | (B<<3) | (C<<2) | (D<<1) | (E)))

#define LCD_DI GPIOC, GPIO_Pin_14
#define LCD_RW GPIOC, GPIO_Pin_15
#define LCD_EN GPIOC, GPIO_Pin_13
#define LCD_BL GPIOE, GPIO_Pin_14

/*==============================================================================*/
/*                          STATIC FUNCTION PROTOTYPES                          */
/*==============================================================================*/
static void LCD_read_busy(void);
static void LCD_putchar(unsigned char zeichen);
static void LCD_writeInstr(unsigned char Instr);
static void LCD_pos(unsigned char LCD_Lines , unsigned char shift);
static void ReplaceSpecialChar(char *Buffer);
static void AddSpecialChar(void);

/*==============================================================================*/
/*                          EXTERN VARIABLES                                    */
/*==============================================================================*/

/*==============================================================================*/
/*                          STATIC  VARIABLES                                   */
/*==============================================================================*/



/*==============================================================================*/
/** @brief      LCD initialization
 *  @details    Initialize LCD
*/
void LCD_Init(void)
{
    meGPIO_Init(DATA_PORT, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DATA_PORT, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DATA_PORT, GPIO_Pin_2, GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DATA_PORT, GPIO_Pin_3, GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DATA_PORT, GPIO_Pin_4, GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DATA_PORT, GPIO_Pin_5, GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DATA_PORT, GPIO_Pin_6, GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(DATA_PORT, GPIO_Pin_7, GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);

    meGPIO_Init(LCD_DI, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(LCD_RW, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(LCD_EN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);
    meGPIO_Init(LCD_BL, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_100MHz, 0);

    LCD_Config();

    LCD_Backlight(true);
}

/*==============================================================================*/
/** @brief      LCD initialization
 *  @details    Initialize LCD
*/
void LCD_Config(void)
{
    GPIO_Write(DATA_PORT, 0x38 | MASK);  // Function Set
    GPIO_WriteBit(LCD_DI, LCD_INSTRUCTION);    // Instr
    GPIO_WriteBit(LCD_RW, LCD_WRITE);          // Write
    GPIO_WriteBit(LCD_EN, LCD_ENABLE);         // LCD Enable
    LCD_wait();
    GPIO_WriteBit(LCD_EN, LCD_DISABLE);        // LCD Disable

    delayMS(50);

    LCD_writeInstr(0x38); // Function Set
    LCD_writeInstr(0x01); // Clear LCD
    LCD_writeInstr(0x06); /* Entry Mode */
    LCD_writeInstr(0x0C); /* LCD on */

    AddSpecialChar();
}

/*==============================================================================*/
/** @brief      LCD clear
 *  @details    Clears the LCD
*/
void LCD_clear(void) {
    LCD_printf(LCD_Line1, "                                        ");
    LCD_printf(LCD_Line2, "                                        ");
}

/*==============================================================================*/
/** @brief      LCD Busy
 *  @details    Wait till lcd is not bussy
*/
static void LCD_read_busy(void)
{
    unsigned char busy = 1;
    do
    {
        GPIO_WriteBit(LCD_DI, LCD_INSTRUCTION);
        GPIO_WriteBit(LCD_RW, LCD_READ);
        GPIO_WriteBit(LCD_EN, LCD_ENABLE);
        LCD_wait();
        busy = GPIO_ReadInputData(DATA_PORT) & 0xFF;
        busy = busy & 0x80;
        GPIO_WriteBit(LCD_EN, LCD_DISABLE);
    }while( busy != 0x00 );
}

/*==============================================================================*/
/** @brief      LCD putchar
 *  @details    Writes a Character to the LCD
*/
static void LCD_putchar(unsigned char zeichen)
{
    GPIO_Write(DATA_PORT, zeichen | MASK);
    GPIO_WriteBit(LCD_DI, LCD_DATA);
    GPIO_WriteBit(LCD_RW, LCD_WRITE);
    GPIO_WriteBit(LCD_EN, LCD_ENABLE);
    LCD_wait();
    GPIO_WriteBit(LCD_EN, LCD_DISABLE);
    GPIO_Write(DATA_PORT, 0xFF | MASK);
    LCD_read_busy();
}

/*==============================================================================*/
/** @brief      LCD putchar
 *  @details    Writes a Character to the LCD
*/
static void LCD_writeInstr(unsigned char Instr)
{
    GPIO_Write(DATA_PORT, Instr | MASK);
    GPIO_WriteBit(LCD_DI, LCD_INSTRUCTION);
    GPIO_WriteBit(LCD_RW, LCD_WRITE);
    GPIO_WriteBit(LCD_EN, LCD_ENABLE);
    LCD_wait();
    GPIO_WriteBit(LCD_EN, LCD_DISABLE);
    GPIO_Write(DATA_PORT, 0xFF | MASK);
    LCD_read_busy();
}

/*==============================================================================*/
/** @brief      LCD Printf
 *  @details    Printf for the LCD
*/
void LCD_printf(ELCDLines Line, const char * lprint, ...)
{
    /*
    Prints Message to LCD. This Funktion works like printf with a aditional argument
    of witch number it writes to the LCD.
    */
    char LCDStr[40+30];
    va_list Args;

    LCD_pos(Line, 0);

    va_start(Args, lprint);
    vsprintf(LCDStr, lprint, Args);
    va_end( Args );

    // Fill rest with whitespace
    for(int i=strlen(LCDStr); i<40; i++) LCDStr[i] = ' ';

    ReplaceSpecialChar(LCDStr);

    for(int i=0; i<40; i++) LCD_putchar(LCDStr[i]);
}

void LCD_FloatToStr(char* buf, unsigned int size, float value, unsigned int Dec)
{
    int FG, FK;
    float tmp = value;

    if(__isnanf(value))
    {
        sprintf(buf, "NA");
        return;
    }

    // Round float
    if(value >= 0) value += 0.3/pow(10, Dec);
    else value -= 0.3/pow(10, Dec);

    FG = tmp;
    tmp -= FG;
    FK = tmp*pow(10, Dec);
    if(FG < 0) FG = -FG;
    if(FK < 0) FK = -FK;

    char format[20];
    if(value >= 0) snprintf(format, size, "%%d.%%0%dd", Dec);
    else snprintf(format, size, "-%%d.%%0%dd", Dec);
    sprintf(buf, format, FG, FK);
}

/*==============================================================================*/
/** @brief      LCD pos
 *  @details    Set the Cursor position on the LCD
*/
static void LCD_pos(ELCDLines LCD_Lines, unsigned char shift)
{
    unsigned char InstrPos = 0;
    switch( LCD_Lines )
    {
    case LCD_Line1:
        InstrPos = 128 + shift;    //LCD zeile1
        break;
    case LCD_Line2:
        InstrPos = 192 + shift;    //LCD zeile2
        break;
    default :
        InstrPos = 128 + shift;    //LCD zeile1
        break;
    }
    LCD_writeInstr(InstrPos);
}

/*==============================================================================*/
/** @brief      Set Cursor
 *  @details    Sets the Cursor to a position
*/
void LCD_SetCursor(ELCDLines Line, unsigned char Pos)
{
    LCD_pos(Line, Pos);
    LCD_writeInstr(0x0F);
}

/*==============================================================================*/
/** @brief      Replace Char
 *  @details    Replace speziel character inthe string
*/
void LCD_CursorOFF(void)
{
    LCD_writeInstr(0x0C);
}

/*==============================================================================*/
/** @brief      Replace Char
 *  @details    Replace speziel character inthe string
*/
static void ReplaceSpecialChar(char *Buffer)
{
    int i;
    for(i=0; i<40; i++)
    {
        if(Buffer[i] == 'ä') Buffer[i] = 0xE1;
        if(Buffer[i] == 'ö') Buffer[i] = 0xEF;
        if(Buffer[i] == 'ü') Buffer[i] = 0xF5;
        if(Buffer[i] == '°') Buffer[i] = 0xDF;
    }
}

/*==============================================================================*/
/** @brief      Backlight enable
 *  @details    enables the backlight
*/
void LCD_Backlight(bool Enable)
{
    GPIO_WriteBit(LCD_BL, Enable);      // Instr
}

/*==============================================================================*/
/** @brief      Add Char
 *  @details    Write customized char to the LCD
*/
static void AddSpecialChar(void)
{
    const char data[] = {
        CONV_CHAR(0,0,1,0,0),
        CONV_CHAR(0,0,1,0,0),
        CONV_CHAR(0,1,1,1,0),
        CONV_CHAR(1,1,1,1,1),
        CONV_CHAR(0,1,1,1,0),
        CONV_CHAR(0,0,1,0,0),
        CONV_CHAR(0,0,1,0,0),
        CONV_CHAR(0,0,1,0,0),

        CONV_CHAR(0,0,0,0,0),
        CONV_CHAR(0,0,1,0,0),
        CONV_CHAR(0,1,1,1,0),
        CONV_CHAR(1,0,1,0,1),
        CONV_CHAR(1,0,1,0,1),
        CONV_CHAR(1,0,0,0,1),
        CONV_CHAR(0,1,1,1,0),
        CONV_CHAR(0,0,0,0,0),

        CONV_CHAR(1,1,1,1,1),
        CONV_CHAR(1,0,0,0,1),
        CONV_CHAR(0,1,1,1,0),
        CONV_CHAR(0,0,1,0,0),
        CONV_CHAR(0,1,0,1,0),
        CONV_CHAR(1,0,0,0,1),
        CONV_CHAR(1,1,1,1,1),
        CONV_CHAR(0,0,0,0,0),

        CONV_CHAR(0,0,0,0,1),
        CONV_CHAR(0,0,0,1,0),
        CONV_CHAR(0,0,0,1,0),
        CONV_CHAR(1,0,1,0,0),
        CONV_CHAR(1,0,1,0,0),
        CONV_CHAR(0,1,0,0,0),
        CONV_CHAR(0,1,0,0,0),
        CONV_CHAR(0,0,0,0,0),
        };


    // Add Data to LCD
    LCD_writeInstr(0x40);

    for(int i=0; i<sizeof(data); i++)
    {
        LCD_putchar(data[i]);
    }
}

