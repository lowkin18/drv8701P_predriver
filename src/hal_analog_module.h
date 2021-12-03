#ifndef _H_HAL_ANALOG_MODULE
#define _H_HAL_ANALOG_MODULE

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define HAL_PWM_MAX 10000 //Corresponds to 100.00% PWM
typedef enum
{
    HAL_nSLEEP, 
    HAL_IN1,
    HAL_IN2,
    END_OF_OUTPUTS
}HAL_eOUTPUTS;


void HAL_SetOutput(HAL_eOUTPUTS eOutput, uint16_t u16Value);


#ifdef TEST
uint16_t HAL_GetOutput(HAL_eOUTPUTS eOutput);
#endif


#endif
