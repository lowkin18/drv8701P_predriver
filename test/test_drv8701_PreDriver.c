
#include "unity.h"
#include "drv8701_PreDriver.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_always_pass(void){
    int a = 1;
    TEST_ASSERT( a == 1 ); //this one will pass
}

void test_always_fail(void){
    int a = 1;
    TEST_ASSERT( a == 2 ); //this one will fail
}


void test_sleep_to_forward(void)
{
    drv8701_var check;
    uint16_t in2_last_cmd = 10000;   //starts at 10000
    SetCommand(100.0,1);
    tick(); //extra tick for transition
    for(;;)
    {
        tick();
        check = GetCommand();
        in2_last_cmd-=100;
        TEST_ASSERT(check.in2 == in2_last_cmd);  //check slew rate
        if(check.in1==10000 && check.in2 == 0)
        {
            break;
        }
    }
    TEST_ASSERT( check.in1 == 10000 && check.in2 == 0 && check.nsleep == true);
}

void test_forward_to_brake(void)
{
    drv8701_var check;
    uint16_t last_cmd = 0;   //starts at 10000
    SetCommand(0.0,1);
    for(;;)
    {
        tick();
        check = GetCommand();
        last_cmd+=100;
        TEST_ASSERT(check.in2 == last_cmd);  //check slew rate
        if(check.in2==10000 && check.in1 == 10000)
        {
            break;
        }
    }
    TEST_ASSERT( check.in1 == 10000 && check.in2 == 10000 && check.nsleep == true);
}

void test_brake_to_reverse(void)
{
    drv8701_var check;
    uint16_t last_cmd = 10000;   //starts at 10000
    SetCommand(-100.0,1);
    tick();
    for(;;)
    {
        tick();
        check = GetCommand();
        last_cmd-=100;
        TEST_ASSERT(check.in1 == last_cmd);  //check slew rate
        if(check.in1==0 && check.in2 == 10000)
        {
            break;
        }
    }
    TEST_ASSERT( check.in1 == 0 && check.in2 == 10000 && check.nsleep == true);
}

void test_reverse_to_brake(void)
{
    drv8701_var check;
    uint16_t last_cmd = 0;   //starts at 10000
    SetCommand(0.0,1);
    for(;;)
    {
        tick();
        check = GetCommand();
        last_cmd+=100;
        
        TEST_ASSERT(check.in1 == last_cmd);  //check slew rate
        if(check.in1==10000 && check.in2 == 10000)
        {
            break;
        }
    }
    TEST_ASSERT( check.in1 == 10000 && check.in2 == 10000 && check.nsleep == true);
}

void test_brake_to_forward(void)
{
    drv8701_var check;
    uint16_t last_cmd = 10000;   //starts at 10000
    SetCommand(100.0,1);
    tick(); //extra tick for transition
    for(;;)
    {
        tick();
        check = GetCommand();
        last_cmd-=100;
        
        TEST_ASSERT(check.in2 == last_cmd);  //check slew rate
        if(check.in1==10000 && check.in2 == 0)
        {
            break;
        }
    }
    TEST_ASSERT( check.in1 == 10000 && check.in2 == 0 && check.nsleep == true);
}

void test_forward_to_sleep(void)
{
    drv8701_var check;
    uint16_t last_cmd = 0;   //starts at 10000
    SetCommand(0.0,0);
    for(;;)
    {
        tick();
        check = GetCommand();
        last_cmd+=100;
        
        TEST_ASSERT(check.in2 == last_cmd);  //check slew rate
        if(check.in1==10000 && check.in2 == 10000)
        {
            break;
        }
    }
    tick(); //1 tick to transition to brake
    check = GetCommand();
    tick(); //1 tick to transition to sleep
    check = GetCommand();
    TEST_ASSERT( check.in1 == 10000 && check.in2 == 10000 && check.nsleep == false);
}
