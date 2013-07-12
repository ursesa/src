/*==============================================================================*/
/** @file       DoStateEventBlink.c
    @brief      This file contains a function, which can be used to blink something easily in a state event
    @author     Marc Luethi
*/


/*==============================================================================*/
/*                          IMPORT                                              */
/*==============================================================================*/
#include "DoStateEventBlink.h"

/*==============================================================================*/
/*                          DEFINITIONS/DECLARATIONS                            */
/*==============================================================================*/
/*==============================================================================*/
/*                          STATIC FUNCTION PROTOTYPES                          */
/*==============================================================================*/

/*==============================================================================*/
/*                          EXTERN VARIABLES                                    */
/*==============================================================================*/

/*==============================================================================*/
/*                          STATIC  VARIABLES                                   */
/*==============================================================================*/


/*==============================================================================*/
/** @brief      This function can be used in a state event to let something easily blink
 *  @param      Counter is a pointer to the working register of the function, should be set to 0 at start
 *  @param      TillOn defines after how many runs the output goes to 1
 *  @param      TillOff defines after how many runs after On the output goes to Off
*/
unsigned char DoStateEventBlink(unsigned int *Counter, unsigned int TillOn, unsigned int TillOff)
{
    if(*Counter < TillOn)
    {
        (*Counter)++;
        return 0;
    }
    if(*Counter >= (TillOn + TillOff -1))
    {
        *Counter = 0;
        return 1;
    }
    (*Counter)++;
    return 1;
}








