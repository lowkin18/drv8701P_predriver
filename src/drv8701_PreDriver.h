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






//1 What is the purpose of slewing the command before outputting it to the predriver? Why might we want to do this?
/*  The purpose of slewing the command before outputting it to the predriver is to minimize large spikes or changes in current.
*   Large transients in current can harm and wear out the electronics faster, this will also cause larger accelerations which can hurt or wear out the mechanicals faster.
*   Large current transients also lead to worse EMI performance and can cause failures during EMC testing. 
*
*/


//2 How does the brake work? When the brake is applied, what happens to the h-bridge and why does that cause the motor to brake
/*The brake works by turning the motor into a generator, the mechanical energy is converted back to electrical energy. This works by having the h-bridge turn on the two bottom MOSFETS of the 
* h-bridge, which are shorted together and create a current flow that the motor needs to now drive.
*
*/

//3 What are the advantages/disadvantages of using analog control vs digital (eg SPI)
/*
*   Some of the advantages of PWM (Analog control) 
*       - That you require only 1 pin per direction
*       - Easier to implement from a firmware perspective
*       - This can be easier to debug from a hardware perspective
*       - Can be faster update rate with less EMI noise, SPI would require minimum 16 times clock speed, to send uint16_t      
*       -
*   
*   Some of the disadvantages of PWM(Analog Control)
*       -
*
*/