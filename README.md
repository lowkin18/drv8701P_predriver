# drv8701P_predriver
This is an example of a predriver for DRV8701P using Slow Decay

This driver provides the SetCommand and GetCommand

SetCommand(float motor_command, bool nsleep)
  
  Params: motor_command: -100.0 to 100.0 with negative being reverse and positive being forward
          nsleep: Boolean variable that will set the DRV8701P chip to sleep

GetCommand();
  return: Struct containing the peripherals outputs
  
        uint16_t in1
        
        uint16_t in2
        
        bool  nsleep


NOTE: Coasting mode is not implemented in this driver
NOTE: Slew rate of 1%/ms is implemented on assumption tick rate is 1khz
NOTE: This driver slews on increasing forward/reverse pwm and decreasing forward/reverse pwm





Test Report:

Test 'test_drv8701_driver.c'
----------------------------
Generating runner for test_drv8701_driver.c...
Compiling test_drv8701_driver_runner.c...
Compiling test_drv8701_driver.c...
Compiling unity.c...
Compiling drv8701_driver.c...
Compiling cmock.c...
Linking test_drv8701_driver.out...
Running test_drv8701_driver.out...

-------------------
FAILED TEST SUMMARY
-------------------
[test_drv8701_driver.c]
  Test: test_always_fail
  At line (20): "Expression Evaluated To FALSE"

--------------------
OVERALL TEST SUMMARY
--------------------
TESTED:  8
PASSED:  7
FAILED:  1
IGNORED: 0


stderr:
---------------------
BUILD FAILURE SUMMARY
---------------------
Unit test failures.
