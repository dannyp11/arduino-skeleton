# arduino-skeleton
This is the first embarkment of my learning experience & side projects. Let's see how far I could go :)
Skeleton framework for Arduino with/without RTOS, uses avr gcc/ g++
Currently works with Unix system, compatible with Arduino framework (setup/loop) using sudar's repo

* Note: 
  1. This repo now supports Arduino IDE, only need to set IDE_SUPPORT to true from skeleton.mk
  2. Even though it supports C++ project, I will try to develop drivers/low level in C

# Directory Structure
        +---test/                               - all component testers go here, test code uses no os
        +---submodules/                         - remote dependency
        |       +---ChibiOS/                    -
        |       +---Arduino-Makefile/           - 
        +---c-project/                          - skeleton for C based projects
        |       +---with-os/                    -
        |       +---without-os/                 -
        +---cpp-project/                        - skeleton for C++ based projects
        |       +---with-os/                    -
        |       +---without-os/                 -
        |       +---with-ide/                   -
        +---doc                                 - documentations about arduino and modules
        +---common/                             - includes common mkfile (skeleton.mk) for all projects
        |       +---utils/                      - common protocols such as soft serial, i2c, debugging
        |       +---makefiles/                  - specific mk files for individual project options
	
# UnitTest framework (Software)
        This repo supports cxxtest framework, run make check will invoke testgen and run the test binary
        Put all test headers in test directory of project directory, see c-project/without-os/GPS-breakout-board for example

# Usage
        go to c*-project/with*-os/ directory and use its makefile
        go to test/ to checkout individual module test code, all code here must be in C except cxxtest headers
        note: always include common/skeleton.mk inside project makefile, then run make help to view variables

# Completed projects
        +--c-project/without-os/GPS-breakout-board      - gps tracker app that shows parsed distance/angle to lcd and serial port
        +--c-project/without-os/i2c-console             - i2c console app for arduino, this tool makes i2c debug easier 
        +--c-project/without-os/serial-calculator       - simple calculator via serial port, supports +-*/() and unsigned number

# Question & Bug fix?
        Please email author tdpham1105@yahoo.com
        Or file issue via github

# Release Note
* DONE v1.0.0 - all-C projects
  + I2C console via serial port: make I2C debugging easier
  + GPS NMEA data parser with LCD: track walked distance & position compared to initial one
  + Simple calculator via serial port, supports +-*/() and non negative numbers
* TODO v2.0.0 - matured C support & new mk files structure
  + Software Serial C support upto baud 9600 for RX, 115200 for TX
  + Support Arduino IDE framework (Arduino Makefile) - done
  + skeleton.mk is one-stop .mk for all projects, include it at the end of project's makefile - done
  + create libutils.a - done
    
