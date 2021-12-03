/**
 * @file drv8701_driver.c
 * @author Chris Kerluke (kip.kerluke@gmail.com)
 * @brief this file is the source code for the drv8701_PreDriver
 * @version 0.0.1
 * @date 2021-12-02
 * 
 * @copyright Copyright (c) 2021
 */

#include "drv8701_PreDriver.h"

/* static variables */
static int16_t _motorCmdReq;  //requested motor command 
static int16_t _motorCmd;     //motor cmd

/**
 * @brief Set the Command of the motor
 * 
 * @param motor_command float value -100.0 to 100.0 with negative being reverse and positive forward
 */
void SetCommand(float motor_command)
{   
    /* bounding required */
    if(motor_command>100.0)motor_command=100.0;
    if(motor_command<-100.0)motor_command=-100.0;
    _motorCmdReq = (int16_t)((motor_command/100.0f)*HAL_PWM_MAX);
}

/**
 * @brief function that will be called to update the motor driver command at a set tick rate
 *        if tick rate is changed, SLEW_RATE should be modified or derived from tick speed
 */
void tick()
{
    uint16_t in1Cmd;    //output for in1 of DRV8701
    uint16_t in2Cmd;    //output for in2 of DRV8701

    /* slew motor command */
    int16_t delta = _motorCmdReq-_motorCmd;

    /* bound change to slew rate */
    delta = (delta>SLEW_RATE)?SLEW_RATE:(delta<-SLEW_RATE)?-SLEW_RATE:delta;
    _motorCmd = _motorCmd+delta;

    /* apply motor command uint16 bounds to positive */
    in1Cmd = HAL_PWM_MAX+_motorCmd;
    in2Cmd = HAL_PWM_MAX-_motorCmd;

    /* bound positive commands within range required based on algorithm*/
    if(in1Cmd>HAL_PWM_MAX)in1Cmd=HAL_PWM_MAX;
    if(in2Cmd>HAL_PWM_MAX)in2Cmd=HAL_PWM_MAX;

    /* set hardware pwm - 10000 == 100% duty */ 
    HAL_SetOutput(HAL_IN1, in1Cmd);
    HAL_SetOutput(HAL_IN2, in2Cmd);
}

