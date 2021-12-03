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
#ifndef H_DRV8701_PREDRIVER
#define H_DRV8701_PREDRIVER
#include "hal_analog_module.h"

/* Defines */
#define SLEW_RATE 100 //Corresponds to 1%/ms NOTE: Requires 1khz tick rate

/* function declarations */
void SetCommand(float motor_command);
void tick();

#endif