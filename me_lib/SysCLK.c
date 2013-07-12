/*==============================================================================*/
/** @file       SysCLK.c
    @brief      Initialisation of the SysClock
    @author     Thomas Braun

    @par Log
    @verbatim
    01.02.11 Initial Release
    10.11.11 Updated to STM32F4
    @endverbatim
*/


/*==============================================================================*/
/*                          IMPORT                                              */
/*==============================================================================*/
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
/** @brief      System clock initialization
 *  @details    Sets the processor to fullspeed. 168MHz. External 8MHz Crystal
*/
void setSysClock(void)
{
    FLASH_OB_BORConfig(OB_BOR_LEVEL3);
    FLASH_InstructionCacheCmd(ENABLE);
    FLASH_DataCacheCmd(ENABLE);
    FLASH_PrefetchBufferCmd(ENABLE);
    FLASH_SetLatency(FLASH_Latency_5);
    RCC_HCLKConfig(RCC_SYSCLK_Div1); // HCLK  = SYSCLK = 168MHz
    RCC_PCLK1Config(RCC_HCLK_Div4); //  PCLK1 = HCLK/4 = 42MHz (APB1 bus 42MHz, timers on this bus  84MHz)
    RCC_PCLK2Config(RCC_HCLK_Div2); //  PCLK2 = HCLK/2 = 84MHz (APB2 bus 84MHz, timers on this bus 168MHz)
    RCC_HSEConfig(RCC_HSE_ON);
    RCC_WaitForHSEStartUp();

    // PLLM=8, PLLN=336 PLLP=2 PLLQ=7
    // PLLVCO = HSE / PLLM * PLLN / PLLP = 8MHz / 8 * 336 / 2 = 168MHz
    // PLLCLK = PLLVCO / PLLP = 336MHz / 2 = 168MHz
    // PLLVCO = PLLVCO / PLLQ = 336MHz / 7 = 48MHz
    RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 7);
    RCC_PLLCmd(ENABLE); // Enable PLL
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); // Wait till PLL is ready
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // Select PLL as system clock source
    while(RCC_GetSYSCLKSource() != 0x08); // Wait till PLL is used as system clock source

    //Configure the SysTick (Cortex System timer)
    SysTick->LOAD = (unsigned int)0x00FFFFFF; //Maximal reload Value. One round is 798.92ms
    SysTick->VAL  = (unsigned int)0;
    SysTick->CTRL = (unsigned int)0x00000001; //Enable and HCLK / 8 = 21MHz

}

