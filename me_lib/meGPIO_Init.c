/*==============================================================================*/
/** @file       meGPIO_Init.c
    @brief      Simple one line GPIO init function
    @author     Marc Luethi
*/
/*==============================================================================*/
/*                          IMPORT                                              */
/*==============================================================================*/
#include "meGPIO_Init.h"
#include "../stm_lib/stm32.h"

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
/** @brief      This function initializes one GPIO Pin
*/
void meGPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOOType_TypeDef GPIO_OType, GPIOPuPd_TypeDef GPIO_PuPd, GPIOSpeed_TypeDef GPIO_Speed, unsigned char initState)
{

    if(GPIOx == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   /* Enable the GPIO_OUT Clock */
    if(GPIOx == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    if(GPIOx == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    if(GPIOx == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    if(GPIOx == GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    if(GPIOx == GPIOF) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    if(GPIOx == GPIOG) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    if(GPIOx == GPIOH) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
    if(GPIOx == GPIOI) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);

    GPIO_WriteBit(GPIOx, GPIO_Pin, initState);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
    GPIO_InitStructure.GPIO_OType = GPIO_OType;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
    GPIO_Init(GPIOx, &GPIO_InitStructure);

}

