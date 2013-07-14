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

//==================================================================================================
// I N C L U D E S
//==================================================================================================

#include "HFU_Util.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>


//==================================================================================================
// L O C A L   C O N S T A N T S
//==================================================================================================

static const long S_POW10[] =
{
	1L,            // 0
	10L,           // 1
	100L,          // 2
	1000L,         // 3
	10000L,        // 4
	100000L,       // 5
	1000000L,      // 6
	10000000L,     // 7
	100000000L,    // 8
	1000000000L    // 9
};


//==================================================================================================
// G L O B A L   F U N C T I O N   I M P L E M E N T A T I O N
//==================================================================================================


//--------------------------------------------------------------------------------------------------
// String alignment funtions
//--------------------------------------------------------------------------------------------------

void HFU_StringAlignLeft(char* str, size_t maxLen)
{
	size_t srcLen = strlen(str);
	size_t i;

	if (maxLen < srcLen)
		return;

	// Fill in spaces to the right of the string.
	for (i = srcLen; i < (maxLen - 1); i++)
		str[i] = ' ';

	str[maxLen - 1] = (char)'\0';
}

void HFU_StringAlignCenter(char* str, size_t maxLen)
{
	size_t srcLen = strlen(str);
	size_t offset = (maxLen - srcLen - 1) / 2;
	size_t i;

	if (maxLen < srcLen)
		return;

	// Center align string.
	for (i = srcLen; (i >= 0) && (i < SIZE_MAX); i--)
		str[i + offset] = str[i];

	// Fill in spaces to the left of the string.
	for (i = 0; i < offset; i++)
		str[i] = (char)' ';

	// Fill in spaces to the right of the string.
	for (i = (srcLen + offset); i < (maxLen - 1); i++)
		str[i] = ' ';
}

void HFU_StringAlignRight(char* str, size_t maxLen)
{
	size_t srcLen = strlen(str);
	size_t offset = maxLen - srcLen - 1;
	size_t i;

	if (maxLen < srcLen)
		return;

	// Right align string.
	for (i = srcLen; (i >= 0) && (i < SIZE_MAX); i--)
		str[i + offset] = str[i];

	// Fill in spaces to the left of the string.
	for (i = 0; i < offset; i++)
		str[i] = (char)' ';
}

void HFU_StringAlign(char* str, size_t maxLen, HFU_tStrAlign align)
{
	switch (align)
	{
		case HFU_STR_ALIGN_RIGHT:
			HFU_StringAlignRight(str, maxLen);
			break;

		case HFU_STR_ALIGN_CENTER:
			HFU_StringAlignCenter(str, maxLen);
			break;

		default: // Includes HFU_STR_ALIGN_LEFT:
			HFU_StringAlignLeft(str, maxLen);
			break;
	}
}


//--------------------------------------------------------------------------------------------------
// Safe string copy function, always adds a terminating '\0'
//--------------------------------------------------------------------------------------------------

char* HFU_strncpy(char* dst, const char* src, size_t maxLen)
{
	char* dstStart = dst;

	if (dst && maxLen)
	{
		if (src)
		{
			while (*src && --maxLen)
				*dst++ = *src++;
		}
		*dst = (char)'\0';             // Terminating '\0'.
	}

	return (dstStart);
}


//--------------------------------------------------------------------------------------------------
// LongToStr / ULongToStr (can be used instead of sprintf())
//--------------------------------------------------------------------------------------------------

size_t HFU_LongToStr(char* dst, long value, size_t maxLen)
{
	int i = 0;
	size_t j = 0;
	char digit;
	bool blanking = true;
	const char maxNegLong[] = "2147483648";

	if (maxLen == 0)
	{
		dst[0] = '\0';
		return (0);
	}

	// Process sign.
	if (value < 0L)
	{
		value = -value;
		dst[j++] = '-';
	}

	// Process up to 10 digits.
	for (i = 9; i >= 0; i--)
	{
		if (j == (maxLen - 1))
		{
			dst[0] = '\0';
			return (0);
		}

		// Special case -2147483648 = -2^31 = 0x8000'0000.
		if (value < 0L)
		{
			dst[j] = maxNegLong[j-1];
			j++;
		}
		else
		{
			digit = (char)((value / S_POW10[i]) % 10L);
			if (digit > '\0' || i == 0 || !blanking)
			{
				dst[j++] = (char)(digit + '0');
				blanking = false;
			}
		}
	}

	dst[j] = '\0';
	return (j);
}

size_t HFU_ULongToStr(char* dst, unsigned long value, size_t maxLen)
{
	int i = 0;
	size_t j = 0;
	char digit;
	bool blanking = true;

	if (maxLen == 0)
	{
		dst[0] = '\0';
		return (0);
	}

	// Process up to 10 digits.
	for (i = 9; i >= 0; i--)
	{
		if (j == (maxLen - 1))
		{
			dst[0] = '\0';
			return (0);
		}

		digit = (char)((value / (unsigned long)S_POW10[i]) % 10UL);
		if (digit > '\0' || i == 0 || !blanking)
		{
			dst[j++] = (char)(digit + '0');
			blanking = false;
		}
	}

	dst[j] = '\0';
	return (j);
}


//--------------------------------------------------------------------------------------------------
// DoubleToStr / DoubleToEStr (can be used instead of sprintf())
//--------------------------------------------------------------------------------------------------

size_t HFU_DoubleToStr(char* dst, double value, int round, size_t maxLen)
{
	double roundingValue = 0.5;
	int roundingPos = round;
	int digit = 0;
	int i = 0;
	size_t j = 0;
	int k = 0;
	int count = 0;
	bool blanking = false;

	if (maxLen == 0)
	{
		dst[0] = '\0';
		return (0);
	}

	// Process sign.
	if (value < 0.0)
	{
		value = -value;
		dst[j++] = '-';
	}

	// Round value.
	if (round < 0)
		roundingPos = 0;

	for (i = roundingPos; i > 0; i--)
		roundingValue *= 0.1;

	value += roundingValue;

	// Limit value to 9.999...1.000.
	while (value >= 10.0)
	{
		value = value * 0.1;
		k++;
	}

	// Set blanking of leading zeroes.
	blanking = (bool)(k > 0);

	// Digit count.
	if (round >= 0)
		count = round + k + 1;
	else
		count = k + 1;

	// Process digits.
	for (i = (count - 1); i >= 0; i--)
	{
		if (j >= (maxLen - 1))
		{
			dst[0] = '\0';
			return (0);
		}

		// Write digit.
		digit = (int)value % 10;
		if (digit > 0 || i <= round || !blanking) {
			dst[j++] = (char)(digit + '0');
			blanking = false;
		}

		if (j == maxLen)
		{
			dst[0] = '\0';
			return (0);
		}

		// Write decimal point.
		if (i == round)
			dst[j++] = '.';

		value = (value - (double)digit) * 10.0;
	}

	dst[j] = '\0';
	return (j);
}

size_t HFU_DoubleToEStr(char* dst, double value, int round, size_t maxLen)
{
	double roundingValue = 0.5;
	int roundingPos = round;
	double temp = value;
	int digit = 0;
	int i = 0;
	size_t j = 0;
	int mantissaCount = 0;
	int exponent = 0;
	size_t exponentLen = 0;

	if (maxLen < 4)
	{
		dst[0] = '\0';
		return (0);
	}

	// Process sign.
	if (value < 0.0)
	{
		value = -value;
		dst[j++] = '-';
	}

	// Round value and get exponent.
	if (value != 0.0)
	{
		// Get exponent of unrounded value for rounding.
		temp = value;
		exponent = 0;
		while (temp < 1.0)
		{
			temp = temp * 10.0;
			exponent--;
		}
		while (temp >= 10.0)
		{
			temp = temp * 0.1;
			exponent++;
		}

		// Round value.
		if (round < 0)
			roundingPos = 0;

		for (i = (roundingPos - exponent); i > 0; i--)
			roundingValue *= 0.1;

		value += roundingValue;

		// Get exponent of rounded value and limit value to 9.999...1.000.
		exponent = 0;
		while (value < 1.0)
		{
			value = value * 10.0;
			exponent--;
		}
		while (value >= 10.0)
		{
			value = value * 0.1;
			exponent++;
		}
	}

	// Adjust exponent string length.
	if (abs(exponent) < 10)
		exponentLen = 2;               // E0..E9     needs 2 chars.
	else if (abs(exponent) < 100)
		exponentLen = 3;               // E10..E99   needs 3 chars.
	else
		exponentLen = 4;               // E100..E999 needs 4 chars.
	if (exponent < 0)
		exponentLen++;                 // Add length for minus sign.

	// Check string length, we need at least 1 mantissa digit + "E" + exponentLen + trailing zero.
	if (exponentLen + 2 >= maxLen)
	{
		dst[0] = '\0';
		return (0);
	}

	// Digit count of mantissa.
	if (round >= 0)
		mantissaCount = round + 1;
	else
		mantissaCount = 1;

	// Write mantissa.
	for (i = (mantissaCount - 1); i >= 0; i--)
	{
		if (j >= (int)(maxLen - exponentLen))
		{
			dst[0] = '\0';
			return (0);
		}

		// Write digit.
		digit = (int)value % 10;
		dst[j++] = (char)(digit + '0');

		if (j >= (int)(maxLen - exponentLen))
		{
			dst[0] = '\0';
			return (0);
		}

		// Write decimal point.
		if (i == round)
			dst[j++] = '.';

		value = (value - (double)digit) * 10.0;
	}

	// Write exponent.
	dst[j++] = 'E';
	HFU_LongToStr(&dst[j], (long)exponent, (size_t)exponentLen);

	return (strlen(dst));
}


//--------------------------------------------------------------------------------------------------
// StrToLong / StrToULong (can be used instead of sscanf())
//--------------------------------------------------------------------------------------------------

bool HFU_StrToLong(const char* src, long* pValue)
{
	bool isValue = false;
	bool isNegative = false;
	bool isParsing = true;

	if ((src == NULL) || (strlen(src) == 0))
		return (false);

	// Skip leading spaces.
	while (*src == ' ')
		src++;

	// Parse string.
	*pValue = 0L;
	while (isParsing)
	{
		switch (*src)
		{
			case '-' :
				isNegative = true;
				break;

			case '+' :
				break;

			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9' :
				*pValue = *pValue * 10L + (long)(*src - '0');
				isValue = true;
				break;

			default:
				isParsing = false;
				break;
		}
		src++;
	}
	src--;

	// Adjust sign.
	if (isNegative)
		*pValue = -*pValue;

	return (isValue);
}

bool HFU_StrToULong(const char* src, unsigned long* pValue)
{
	bool isValue = false;
	bool isParsing = true;

	if ((src == NULL) || (strlen(src) == 0))
		return (false);

	// Skip leading spaces.
	while (*src == ' ')
		src++;

	// Parse string.
	*pValue = 0UL;
	while (isParsing)
	{
		switch (*src)
		{
			case '+' :
				break;

			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9' :
				*pValue = *pValue * 10UL + (unsigned long)(*src - '0');
				isValue = true;
				break;

			default:
				isParsing = false;
				break;
		} // switch (*str).
		src++;
	}
	src--;

	return (isValue);
}

//--------------------------------------------------------------------------------------------------
// StrToDouble (can be used instead of sscanf())
//--------------------------------------------------------------------------------------------------

bool HFU_StrToDouble(const char* src, double* pValue)
{
	bool isValue = false;
	bool isNegative = false;
	int exponent = 0;
	int exponentCorrection = 0;
	bool exponentIsNegativ = false;
	bool exponentInputParsed = false;
	bool dpParsed = false;
	bool isParsing = true;

	if ((src == NULL) || (strlen(src) == 0U))
		return (false);

	// Skip leading spaces.
	while (*src == ' ')
		src++;

	// Parse string.
	*pValue = 0.0;
	while (isParsing)
	{
		switch (*src)
		{
			case '-' :
				if (!exponentInputParsed)
					isNegative = true;
				else
					exponentIsNegativ = true;
				break;

			case '+' :
				break;

			case '.' :
				if (!dpParsed)
					dpParsed = true;
				else
					isParsing = false;
				break;

			case 'e' :
			case 'E' :
				if (!exponentInputParsed)
					exponentInputParsed = true;
				else
					isParsing = false;
				break;

			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9' :
				if (!exponentInputParsed)
				{
					*pValue = *pValue * 10.0 + (double)(*src - '0');
					isValue = 1;
					if (dpParsed)
						exponentCorrection--;
				}
				else
				{
					exponent = exponent * 10 + (*src - '0');
				}
				break;

			default:
				isParsing = false;
				break;
		}
		src++;
	}
	src--;

	// Calculate exponent correction.
	if (exponentIsNegativ)
		exponent = -exponent;
	exponent = exponent + exponentCorrection;

	// Adjust value according to positive exponent.
	while (exponent > 0)
	{
		*pValue = *pValue * 10.0;
		exponent--;
	}

	// Adjust value according to negative exponent.
	while (exponent < 0)
	{
		*pValue = *pValue * 0.1;
		exponent++;
	}

	// Adjust sign.
	if (isNegative)
		*pValue = -*pValue;

	return (isValue);
}


//==================================================================================================
// End of file
//==================================================================================================
