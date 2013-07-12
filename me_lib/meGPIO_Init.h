#ifndef meGPIO_Init_H_
#define meGPIO_Init_H_

#include "../stm_lib/stm32.h"
extern void meGPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, GPIOOType_TypeDef GPIO_OType, GPIOPuPd_TypeDef GPIO_PuPd, GPIOSpeed_TypeDef GPIO_Speed, unsigned char initState);

#endif
