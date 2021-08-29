/**
 * @file drv8701_driver.h
 * @author Chris Kerluke (kip.kerluke@gmail.com)
 * @brief This module is created for interfacing to the DRV8701P H-Bridge Motor Driver
 *          Requirements:
 *              - When the motor command changes the maximum slew rate should be 1%/ms - ASSUMPTION: slew not brake to zero pwm
 *              - When both commands are 0, ie both pwm outputs are 0 the driver should be braking:slow decay
 *              - When the motor is in the off cycle driver should be braking:slow decay
 * @version 0.0.1
 * @date 2021-08-28
 * @copyright Copyright (c) 2021
 * 
 */

#include "hal_analog_module.h"

#define SLEW_RATE 100 //Corresponds to 1%/ms NOTE: Requires 1khz tick rate

//hal_analog_module.h
/*
#define HAL_PWM_MAX 10000 //Corresponds to 100.00% PWM

typedef enum
{
    HAL_nSLEEP,
    HAL_IN1,
    HAL_IN2
}HAL_eOUTPUTS;

void HAL_SetOutput(HAL_eOUTPUTS eOutput, uin16_t u16Value);
*/

//ENUM FOR MOTOR STATE - NOTE* Ignoring coasting
typedef enum{
    SLEEP,
    BRAKING,
    FORWARD,
    REVERSE
}motor_state;

typedef struct{
    uint16_t in1;   //local var: IN1 output to motor driver, inverted for slow side decay in low side of pwm
    uint16_t in2;   //local var: IN2 output to motor driver, inverted for slow side decay in low side of pwm
    bool nsleep;    //local var: nSLEEP 
}drv8701_var;


drv8701_var GetCommand();
void SetCommand(float motor_command, bool nsleep);
void tick();

