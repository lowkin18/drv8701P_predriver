
#include "unity.h"
#include "drv8701_PreDriver.h"
#include "hal_analog_module.h"
void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_brake_to_forward(void)
{   
    /*test setup for initial conditions */
    
    /* run test */
    SetCommand(100.0);
    uint16_t in2_expected =10000;
    for(;;)
    {
        tick();
        uint16_t in1 = HAL_GetOutput(HAL_IN1);
        uint16_t in2 = HAL_GetOutput(HAL_IN2);
        TEST_ASSERT(in1 == HAL_PWM_MAX && in2 == in2_expected-SLEW_RATE);  //check slew rate
        if(in2 == 0)
        {
            break;
        }
        in2_expected = in2_expected-SLEW_RATE;
    }
    uint16_t in1 = HAL_GetOutput(HAL_IN1);
    uint16_t in2 = HAL_GetOutput(HAL_IN2);
    TEST_ASSERT( in1 == HAL_PWM_MAX && in2 == 0);
}

void test_forward_to_brake(void)
{
    /*test setup for initial conditions */
    uint16_t in2_expected =0;
    SetCommand(100.0);
    uint16_t running = 100;
    while(running){
        running--;
        tick();
    }
    /* run test */
    SetCommand(0);
    for(;;)
    {
        tick();
        uint16_t in1 = HAL_GetOutput(HAL_IN1);
        uint16_t in2 = HAL_GetOutput(HAL_IN2);
        TEST_ASSERT( in1 == HAL_PWM_MAX && in2 == in2_expected+SLEW_RATE );  //check slew rate
        if(in2 == HAL_PWM_MAX)
        {
            break;
        }
        in2_expected = in2_expected+SLEW_RATE;
    }
    uint16_t in1 = HAL_GetOutput(HAL_IN1);
    uint16_t in2 = HAL_GetOutput(HAL_IN2);
    TEST_ASSERT( in1 == HAL_PWM_MAX && in2 == HAL_PWM_MAX);
}



void test_brake_to_reverse(void)
{
    /*test setup for initial conditions */

    /* run test */
    uint16_t in1_expected =10000;
    SetCommand(-100.0);
    for(;;)
    {
        tick();
        uint16_t in1 = HAL_GetOutput(HAL_IN1);
        uint16_t in2 = HAL_GetOutput(HAL_IN2);
        TEST_ASSERT(in1 == in1_expected-SLEW_RATE && in2 == HAL_PWM_MAX);  //check slew rate
        if(in1 == 0)
        {
            break;
        }
        in1_expected = in1_expected-SLEW_RATE;
    }
    uint16_t in1 = HAL_GetOutput(HAL_IN1);
    uint16_t in2 = HAL_GetOutput(HAL_IN2);
    TEST_ASSERT( in2 == HAL_PWM_MAX && in1 == 0);
}

void test_reverse_to_brake(void)
{
    /*test setup for initial conditions */
    uint16_t in1_expected =0;
    SetCommand(-100.0);
    uint16_t running = 100;
    while(running){
        running--;
        tick();
    }
    /* run test */
    SetCommand(0);
    for(;;)
    {
        tick();
        uint16_t in1 = HAL_GetOutput(HAL_IN1);
        uint16_t in2 = HAL_GetOutput(HAL_IN2);
        TEST_ASSERT(in1 == in1_expected+SLEW_RATE && in2 == HAL_PWM_MAX);  //check slew rate
        if(in1 == HAL_PWM_MAX)
        {
            break;
        }
        in1_expected = in1_expected+SLEW_RATE;
    }
    uint16_t in1 = HAL_GetOutput(HAL_IN1);
    uint16_t in2 = HAL_GetOutput(HAL_IN2);
    TEST_ASSERT( in1 == HAL_PWM_MAX && in2 == HAL_PWM_MAX);
}

void test_forward_50_step(void)
{
    /*test setup for initial conditions */
    uint16_t in1_expected =0;
    SetCommand(0);
    uint16_t running = 100;
    while(running){
        running--;
        tick();
    }
    SetCommand(0.5);
    tick();
    uint16_t in2 = HAL_GetOutput(HAL_IN2);
    TEST_ASSERT(in2 == 9950);
}

void test_reverse_50_step(void)
{
    /*test setup for initial conditions */
    uint16_t in1_expected =0;
    SetCommand(0);
    uint16_t running = 100;
    while(running){
        running--;
        tick();
    }
    SetCommand(-0.5);
    tick();
    uint16_t in1 = HAL_GetOutput(HAL_IN1);
    TEST_ASSERT(in1 == 9950);
}


void test_forward_to_reverse(void)
{
    /*test setup for initial conditions */
    uint16_t in2_expected =0;
    SetCommand(100.0);
    uint16_t running = 102;
    while(running){
        running--;
        tick();
    }
    /* run test */
    SetCommand(-100.0);
    for(;;)
    {
        tick();
        uint16_t in1 = HAL_GetOutput(HAL_IN1);
        uint16_t in2 = HAL_GetOutput(HAL_IN2);
        TEST_ASSERT(in1 == HAL_PWM_MAX && in2 == in2_expected+SLEW_RATE);  //check slew rate
        if(in2 == HAL_PWM_MAX)
        {
            in2_expected = in2_expected+SLEW_RATE;
            break;
        }
        in2_expected = in2_expected+SLEW_RATE;
    }
    uint16_t in1_expected =10000;
    for(;;)
    {
        tick();
        uint16_t in1 = HAL_GetOutput(HAL_IN1);
        uint16_t in2 = HAL_GetOutput(HAL_IN2);
        TEST_ASSERT(in1 == in1_expected-SLEW_RATE && in2 ==HAL_PWM_MAX);  //check slew rate
        if(in1 == 0)
        {
            break;
        }
        in1_expected = in1_expected-SLEW_RATE;
    }

    uint16_t in1 = HAL_GetOutput(HAL_IN1);
    uint16_t in2 = HAL_GetOutput(HAL_IN2);
    TEST_ASSERT( in1 == 0 && in2 == HAL_PWM_MAX);
}

void test_reverse_to_forward(void)
{
    /*test setup for initial conditions */
    uint16_t in1_expected =0;
    SetCommand(-100.0);
    uint16_t running = 105;
    while(running){
        running--;
        tick();
    }
    /* run test */
    SetCommand(100.0);
    for(;;)
    {
        tick();
        uint16_t in1 = HAL_GetOutput(HAL_IN1);
        uint16_t in2 = HAL_GetOutput(HAL_IN2);
        TEST_ASSERT(in1 == in1_expected+SLEW_RATE && in2 == HAL_PWM_MAX);  //check slew rate
        if(in1 == HAL_PWM_MAX)
        {
            in1_expected = in1_expected+SLEW_RATE;
            break;
        }
        in1_expected = in1_expected+SLEW_RATE;
    }
    uint16_t in2_expected =10000;
    for(;;)
    {
        tick();
        uint16_t in1 = HAL_GetOutput(HAL_IN1);
        uint16_t in2 = HAL_GetOutput(HAL_IN2);
        TEST_ASSERT(in1==HAL_PWM_MAX && in2 == in2_expected-SLEW_RATE);  //check slew rate
        if(in2 == 0)
        {
            break;
        }
        in2_expected = in2_expected-SLEW_RATE;
    }

    uint16_t in1 = HAL_GetOutput(HAL_IN1);
    uint16_t in2 = HAL_GetOutput(HAL_IN2);
    TEST_ASSERT( in1 == HAL_PWM_MAX && in2 == 0);
}

void test_invalid_input_reverse(void)
{
    /*test setup for initial conditions */
    uint16_t in1_expected =0;
    SetCommand(0.0);
    uint16_t running = 105;
    while(running){
        running--;
        tick();
    }
    /* run test */
    SetCommand(-1000.0);
    running = 200;
    while(running){
        running--;
        if(running==150)
        {
            tick();
        }
        tick();
    }
    uint16_t in1 = HAL_GetOutput(HAL_IN1);
    uint16_t in2 = HAL_GetOutput(HAL_IN2);
    TEST_ASSERT( in1 == 0 && in2 == HAL_PWM_MAX);
}

void test_invalid_input_forward(void)
{
    /*test setup for initial conditions */
    SetCommand(0.0);
    uint16_t running = 105;
    while(running){
        running--;
        tick();
    }
    /* run test */
    SetCommand(1000.0);
    running = 105;
    while(running){
        running--;
        tick();
    }
    uint16_t in1 = HAL_GetOutput(HAL_IN1);
    uint16_t in2 = HAL_GetOutput(HAL_IN2);
    TEST_ASSERT( in1 == HAL_PWM_MAX && in2 == 0);
}