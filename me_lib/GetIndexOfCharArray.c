/*==============================================================================*/
/** @file       GetIndexOfCharArray.c
    @brief      This functions return the index of the first position in
                array from the given value
    @author     Marc Luethi
    @note

    @par Log
    @verbatim
    11.01.11 ML :   First implementation
    @endverbatim
*/
 

/*==============================================================================*/
/*                          IMPORT                                              */
/*==============================================================================*/
#include "GetIndexOfCharArray.h"

/*==============================================================================*/
/** @brief      Returns the index of the first cs in *c
*/
int GetIndexOfCharArray(char *c, char cs, int iLength)
{
    int i;
    for(i=0; i<iLength; i++) if(c[i] == cs) return i;
    return 0;
}
/*==============================================================================*/
/** @brief      Returns the index of the first val in *array
*/
int GetIndexOfSiArray(int *array, int val, int iLength)
{
    int i;
    for(i=0; i<iLength; i++) if(array[i] == val) return i;
    for(i=0; i<iLength; i++) if(array[i] >  val) return i;
    return -1;
}
