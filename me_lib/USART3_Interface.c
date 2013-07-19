/*==============================================================================*/
/** @file       USART3_Interface.c
    @brief      Initialization of USART3 Interface for the HMI communication
    @author     Marc Luethi

    TX data is written to a TX FIFO buffer. Data is sent by the TX interrupt.
*/


/*==============================================================================*/
/*                          IMPORT                                              */
/*==============================================================================*/
#include <stm32.h>
#include "USART3_Interface.h"

/*==============================================================================*/
/*                          DEFINITIONS/DECLARATIONS                            */
/*==============================================================================*/
#define BUF_FIFO_SIZE   256  //Must be a power of 2 (16,32,64,128,256,512,...)
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
/** @brief      Initialisation of USART3 Interface for the HMI communication
 *  @details    57600 Baud, RX Interrupt enabled
*/
void USART3_Init(unsigned int BaudRate)
{

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;	//Rx | Tx
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //GPIO_Speed_25MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);


    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);

    //Interrupt Configuration
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}
/*==============================================================================*/
/** @brief      Writes data to the TX FIFO buffer
*/
void USART3_PutC(char c)
{
	USART_SendData(USART3, c);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); //: Transmission Complete flag
}

/*==================================================================================*/
void USART3_PutS(char *c)
{
    while(*c != 0)
    {
        USART3_PutC(*c);
        c++;
    }
}
/*==============================================================================*/
/** @brief      Interrupt Function: Called if a byte has been received or sent
 *  @details    Sends data from the TX FIFO buffer
*/
void USART3_IRQHandler(void)
{
    //char c;
    if((USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) ||
       (USART_GetITStatus(USART3, USART_IT_ORE_RX) != RESET))
    {
         //c = USART_ReceiveData(USART3);
         //todo MeComL1_ReceiveFrame(0, c);
    }
}
