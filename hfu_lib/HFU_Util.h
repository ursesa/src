//==================================================================================================
//! \brief     HFU utilities optimized for embedded systems.
//==================================================================================================
//! \verbatim
//! -----------------------------------------------------------------------------------------------
//! School   : HFU Hoehere Fachschule Uster
//! Subject  : Microcontroller
//! Topic    : HFU Utilities
//! ------------------------------------------------------------------------------------------------
//!            Date         Name             Comment
//! Created  : 2008-10-01   Matthias Klaey   Created for HFU EduKit 2008, based on CB-TSU-Util
//! History  : -
//! -----------------------------------------------------------------------------------------------
//! (c) Copyright 2008-2012 Matthias Klaey and HFU Hoehere Fachschule Uster. All Rights Reserved.
//! -----------------------------------------------------------------------------------------------
//! \endverbatim
//==================================================================================================
//! HFU utilities optimized for embedded systems.
//==================================================================================================

#ifndef HFU_UTIL_H
#define HFU_UTIL_H


//==================================================================================================
// I N C L U D E S
//==================================================================================================

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


//==================================================================================================
// G L O B A L   T Y P E S
//==================================================================================================

// String alignment.
typedef	enum
{
	HFU_STR_ALIGN_LEFT,
	HFU_STR_ALIGN_RIGHT,
	HFU_STR_ALIGN_CENTER
}
HFU_tStrAlign;


//==================================================================================================
// G L O B A L   F U N C T I O N   D E C L A R A T I O N
//==================================================================================================

//--------------------------------------------------------------------------------------------------
// String alignment funtions
//--------------------------------------------------------------------------------------------------
// Example usage:
//
// #define STR_LEN 11
// char str[STR_LEN] = "Example";
//
//             0123456789
// str before "Example"
// str after  "Example   "
// HFU_StringAlignLeft(str, STR_LEN);
//
//             0123456789
// str before "Example"
// str after  " Example  "
// HFU_StringAlignCenter(str, STR_LEN);
//
//             0123456789
// str before "Example"
// str after  "   Example"
// HFU_StringAlignRight(str, STR_LEN);
//--------------------------------------------------------------------------------------------------

void HFU_StringAlignLeft(char* str, size_t maxLen);
void HFU_StringAlignCenter(char* str, size_t maxLen);
void HFU_StringAlignRight(char* str, size_t maxLen);
void HFU_StringAlign(char* str, size_t len, HFU_tStrAlign align);


//--------------------------------------------------------------------------------------------------
// Safe string copy function, always adds a terminating '\0'
//--------------------------------------------------------------------------------------------------

char* HFU_strncpy(char* dst, const char* src, size_t maxLen);


//--------------------------------------------------------------------------------------------------
// LongToStr / ULongToStr (can be used instead of sprintf())
//--------------------------------------------------------------------------------------------------

size_t HFU_LongToStr(char* dst, long value, size_t maxLen);
size_t HFU_ULongToStr(char* dst, unsigned long value, size_t maxLen);


//--------------------------------------------------------------------------------------------------
// DoubleToStr / DoubleToEStr (can be used instead of sprintf())
//--------------------------------------------------------------------------------------------------

size_t HFU_DoubleToStr(char* dst, double value, int round, size_t maxLen);
size_t HFU_DoubleToEStr(char* dst, double value, int round, size_t maxLen);


//--------------------------------------------------------------------------------------------------
// StrToLong / StrToULong (can be used instead of sscanf())
//--------------------------------------------------------------------------------------------------

bool HFU_StrToLong(const char* src, long* pValue);
bool HFU_StrToULong(const char* src, unsigned long* pValue);


//--------------------------------------------------------------------------------------------------
// StrToDouble (can be used instead of sscanf())
//--------------------------------------------------------------------------------------------------

bool HFU_StrToDouble(const char* src, double* pValue);


#endif // HFU_UTIL_H

//==================================================================================================
// End of file
//==================================================================================================
