#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define HAL_PWM_MAX 10000 //Corresponds to 100.00% PWM

typedef enum
{
    HAL_nSLEEP,
    HAL_IN1,
    HAL_IN2
}HAL_eOUTPUTS;

void static inline HAL_SetOutput(HAL_eOUTPUTS eOutput, uint16_t u16Value)
{
    switch(eOutput)
    {
        case HAL_nSLEEP:
            //CALL_PERIPHERAL_DRIVER FOR NSLEEP GPIO
            //printf("HAL_nSLEEP:%d\t",u16Value);
            break;
        case HAL_IN1:
            //CALL_PERIPHERAL_DRIVER FOR HAL_IN1
            //printf("HAL_IN1:%d\t",u16Value);
            break;
        case HAL_IN2:
            //CALL_PERIPHERAL_DRIVER FOR HAL_IN2
            //printf("HAL_IN2:%d\t\n",u16Value);
            break;
        default:
            break;
    }
}

