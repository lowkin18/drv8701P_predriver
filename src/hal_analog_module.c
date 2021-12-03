/**
 * @file hal_analog_module.c
 * @author Chris Kerluke (kip.kerluke@gmail.com)
 * @brief  Simply written for testing
 * @version 0.1
 * @date 2021-12-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "hal_analog_module.h"

static uint16_t PWM_OUT[END_OF_OUTPUTS];

/**
 * @brief 
 * 
 * @param eOutput 
 * @param u16Value 
 */
void HAL_SetOutput(HAL_eOUTPUTS eOutput, uint16_t u16Value)
{
    PWM_OUT[eOutput] = u16Value;
}

/**
 * @brief function for getting the current motor pwm outputs, created for testing
 * 
 * @param eOutput enum value that is the index of the PWM_OUT Vars
 * @return uint16_t value of the variable requested by index
 */
uint16_t HAL_GetOutput(HAL_eOUTPUTS eOutput){
    return PWM_OUT[eOutput];
}