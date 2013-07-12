/*==============================================================================*/
/** @file       TickDelay.c
    @brief      Polling delay function based on SysTick ctr. Time resolution in [us]
    @details    It is possible to realize delays till 204s.
                The sysTick counter has a round rotary time of 798ms.
                It is important to call the IsElapsed function at least
                every 797ms (shorter than the sysTick round rotary time).
    @author     Marc Luethi
*/
/*==============================================================================*/
/*                          IMPORT                                              */
/*==============================================================================*/
#include "TickDelay.h"
#include <stm32.h>

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
/** @brief      This function sets the time delay in [us]
*/
void TickDelay_Set(struct sTickDelay *Sys, unsigned int DelayUs)
{
    Sys->TargetTicks = DelayUs * 21; //One Tick is 47.619ns
    Sys->CtrOld = SysTick->VAL;
}
/*==============================================================================*/
/** @brief      This function must be called after the Set function
 *              to check if the time is already elapsed.
 *              It returns true if the time is elapsed.
*/
unsigned char TickDelay_IsElapsed(struct sTickDelay *Sys)
{
    unsigned int ActVal = 0, Delta = 0;

    ActVal = SysTick->VAL;
    if(ActVal > Sys->CtrOld)
    {
        //Counter was reloaded
        Delta =  Sys->CtrOld;
        Delta += SysTick->LOAD - ActVal;
    }
    else
    {
        Delta = Sys->CtrOld - ActVal;
    }
    Sys->CtrOld = ActVal;

    if(Delta < Sys->TargetTicks)
    {
        Sys->TargetTicks -= Delta;
        return 0;
    }
    else return 1;
}
