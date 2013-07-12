
#include <me_lib.h>

typedef enum
{
    LCD_Line1,
    LCD_Line2,
}ELCDLines;


extern void LCD_Init(void);
extern void LCD_Config(void);
extern void LCD_printf(ELCDLines Line, const char * lprint, ...);
extern void LCD_clear(void);
extern void LCD_SetCursor(ELCDLines Line, unsigned char Pos);
extern void LCD_CursorOFF(void);
extern void LCD_Backlight(bool Enable);
extern void LCD_FloatToStr(char* buf, unsigned int size, float value, unsigned int Dec);
