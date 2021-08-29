/**
 * @file drv8701_driver.c
 * @author Chris Kerluke (kip.kerluke@gmail.com)
 * @brief this file is the source code for the drv8701_PreDriver
 * @version 0.0.1
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "drv8701_PreDriver.h"

//local state of drv8701_vars used to facilitate slew
static drv8701_var _requested_cmd =     //requested command from application code
{
.in1 = HAL_PWM_MAX, //default requested in1 to max pwm
.in2 = HAL_PWM_MAX, //default requested in2 to max pwm
.nsleep = false     //default device asleep, false turns off drv8701, true turns on drv8701
};                                      


static drv8701_var _current_cmd =       //current command of the MCU outputs to drv8701
{
.in1 = HAL_PWM_MAX, //default requested in1 to max pwm
.in2 = HAL_PWM_MAX, //default requested in2 to max pwm
.nsleep = false     //default device asleep, false turns off drv8701, true turns on drv8701
};                                      

//local vars
static motor_state _motor_state = SLEEP;

//local functions
static uint16_t SlewCommand(uint16_t current_pwm, uint16_t new_pwm);


/**
 * @brief void SetCommand(); this function allows the application to set a new PWM command
 *        function needs to be supplied with motor commands from -100.0(reverse) to 100.0(forward) and nsleep = true
 * 
 * @param motor_command: float from -100.0% to 100.0%, minus for reverse and positive for forward
 * @param nsleep: bool: to set driver into sleep mode - 0 for sleep and 1 for on. if nsleep 0 motor commands ignored.
 * 
 */
void SetCommand(float motor_command, bool nsleep)
{
    //check if requested nsleep, override any commands. Application code should set motor to 0 on sleep
    if(nsleep == false)
    {
        _requested_cmd.nsleep = nsleep;
        _requested_cmd.in1 = HAL_PWM_MAX;
        _requested_cmd.in2 = HAL_PWM_MAX;
    }
    //check brake condition, if command is zero we want motor to brake: slow decay
    else if(motor_command == 0.0)
    {
        //set motor to brake, both _in1 and _in2 high
        _requested_cmd.nsleep = nsleep;
        _requested_cmd.in1 = HAL_PWM_MAX;
        _requested_cmd.in2 = HAL_PWM_MAX;
    }
    
    else if(motor_command > 0.0) //forward direction
    {
        _requested_cmd.nsleep = nsleep;
        _requested_cmd.in1 = HAL_PWM_MAX;
        _requested_cmd.in2 = HAL_PWM_MAX-(uint16_t)(motor_command*100);
    }
    else if(motor_command < 0.0) //reverse direction
    {
        _requested_cmd.nsleep = nsleep;
        _requested_cmd.in1 = HAL_PWM_MAX+(uint16_t)(motor_command*100);
        _requested_cmd.in2 = HAL_PWM_MAX;
    }
    else
    {
        /* error condition */
        _requested_cmd.nsleep = false;
        _requested_cmd.in1 = HAL_PWM_MAX;
        _requested_cmd.in2 = HAL_PWM_MAX;
    }
}


/**
 * @brief void tick(); This function will be called by the application at 1khz and implements the function of the driver
 * 
 * 
 */
void tick()
{

    //set outputs based on cmd request and current state
    switch(_motor_state)
    {
        //STATE MOTOR IS IN SLEEP MODE BOTH IN1 & IN2 at HAL_PWM_MAX
        case SLEEP:
            _current_cmd.nsleep = false;
            _current_cmd.in1 = HAL_PWM_MAX;
            _current_cmd.in2 = HAL_PWM_MAX;
            break;
        //STATE MOTOR IS IN BRAKING MODE (SLOW DECAY), PWM SHOULD BE MAX AND nsleep TRUE
        case BRAKING:
            _current_cmd.nsleep = true;
            _current_cmd.in1 = HAL_PWM_MAX;
            _current_cmd.in2 = HAL_PWM_MAX;
            break;
        //STATE MOTOR IS IN FORWARD MODE WITH (SLOW DECAY), IN1 SHOULD BE MAX, WITH IN2 AT DUTY CYCLE %, nsleep TRUE
        case FORWARD:
            _current_cmd.nsleep = true;
            _current_cmd.in2 = SlewCommand(_current_cmd.in2, _requested_cmd.in2);
            _current_cmd.in1 = HAL_PWM_MAX;
            break;
        //STATE MOTOR IS IN REVERSE MODE WITH (SLOW DECAY), IN2 SHOULD BE MAX, WITH IN1 AT DUTY CYCLE %, nsleep TRUE
        case REVERSE:
            _current_cmd.nsleep = true;
            _current_cmd.in1 = SlewCommand(_current_cmd.in1, _requested_cmd.in1);
            _current_cmd.in2 = HAL_PWM_MAX;
            break;
        //ERROR DEFAULT
        default:
            _current_cmd.nsleep = false;
            _current_cmd.in1 = HAL_PWM_MAX;
            _current_cmd.in2 = HAL_PWM_MAX;
            break;
    }


    
    //set state based on cmd request and current state
    switch(_motor_state)
    {
        case SLEEP:
            //exit sleep state if nsleep set to active
            if(_requested_cmd.nsleep == true)
            {
                _current_cmd.nsleep = true; //turn on DRV8701P
                
                if(_requested_cmd.in2 != HAL_PWM_MAX) //requested forward
                {
                    _motor_state = FORWARD;
                }
                else if(_requested_cmd.in1 != HAL_PWM_MAX) // requested reverse
                {
                    _motor_state = REVERSE;
                }
                else    //did not request forward or reverse, so assume braking for now
                {
                    _motor_state = BRAKING;
                }
            }
            break;
        case BRAKING: 
            //exit braking if nsleep false
            if(_requested_cmd.nsleep == false)
            {
                _motor_state = SLEEP;
            }
            else if(_requested_cmd.in2 != HAL_PWM_MAX) //requested forward
            {
                _motor_state = FORWARD;
            }
            else if(_requested_cmd.in1 != HAL_PWM_MAX) //requested reverse
            {
                _motor_state = REVERSE;
            }
            break;
            //FORWARD state transitions when IN2 becomes HAL_PWM_MAX
        case FORWARD:
            if(_current_cmd.in2 == HAL_PWM_MAX)
            {
                _motor_state = BRAKING; 
            }
            break;
            //REVERSE STATE transitions whne IN1 becomes HAL_PWM_MAX
        case REVERSE:
            if(_current_cmd.in1 == HAL_PWM_MAX)
            {
                _motor_state = BRAKING; 
            }
            break;
        default:
            //ERROR STATE, TURN OF MOTOR WITH nsleep AND SET PWM MAX
            _motor_state = SLEEP;
            break;
    }

    //OUTPUT TO PERIPHERAL DRIVER
    HAL_SetOutput(HAL_nSLEEP,_current_cmd.nsleep);
    HAL_SetOutput(HAL_IN1,_current_cmd.in1);
    HAL_SetOutput(HAL_IN2,_current_cmd.in2);
}

/**
 * @brief uint16_t static SlewCommand(uint16_t current_pwm, uint16_t requested_pwm)
 *      This function is used to slew the PWM commands given to the motor driver.
 *      The max SLEW RATE is a define and is set to 1% under the assumption this updates at 1khz
 * 
 * @param current_pwm 
 * @param requested_pwm 
 * @return uint16_t 
 */
uint16_t static SlewCommand(uint16_t current_pwm, uint16_t requested_pwm)
{
    //slew limit 1%/ms --> 1% per tick call
    uint16_t slewRate = SLEW_RATE;  //assumption on tick rate of 1khz
    uint16_t pwm_with_slew;         //return adjusted PWM accounting for slew of 1%/ms
    int16_t cmdDelta = requested_pwm-current_pwm;

    //if requested cmd == current command no slew necessary just return cmd
    if(cmdDelta == 0)
    {
        pwm_with_slew = requested_pwm;
    }
    //if requested is less than 
    else if(cmdDelta>slewRate)
    {
        pwm_with_slew = current_pwm+SLEW_RATE;
    }
    else if(cmdDelta<-slewRate)
    {
        pwm_with_slew = current_pwm-SLEW_RATE;
    }
    else
    {
        pwm_with_slew = requested_pwm;
    }

    return pwm_with_slew;
}

/**
 * @brief Get the Command object
 * 
 * @return drv8701_var 
 */
drv8701_var GetCommand()
{
    return _current_cmd;
}


