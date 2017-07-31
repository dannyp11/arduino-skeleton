# arduino-skeleton
[![Build Status](https://travis-ci.org/dannyp11/arduino-skeleton.svg?branch=master)](https://travis-ci.org/dannyp11/arduino-skeleton)
<a href="https://scan.coverity.com/projects/dannyp11-arduino-skeleton">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/13324/badge.svg"/>
</a>
[![Release](https://img.shields.io/badge/Release-2.1.0-green.svg)](https://img.shields.io/badge/Release-2.1.0-green.svg)
[![License](https://img.shields.io/badge/License-BSD%202--Clause-orange.svg)](https://opensource.org/licenses/BSD-2-Clause)

This is the first embarkment of my learning experience & side projects. Let's see how far I could go :)
Skeleton framework for Arduino with/without RTOS, uses avr gcc/ g++
Currently works with Unix system, compatible with Arduino framework (setup/loop) using sudar's repo

* Note: 
  1. This repo now supports Arduino IDE, only need to set IDE_SUPPORT to true from skeleton.mk
  2. Even though it supports C++ project, I will try to develop drivers/low level in C
  3. When in doubt, run make help in project directory for variable config

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
        Put all test headers in test directory of project directory
        Name test headers as *Test.h 
        See c-project/without-os/GPS-breakout-board for example
        
        This repo also supports googletest framework. Similarly run make check
        Put all test headers in test directory of project directory
       	Name test files as *gtest.cpp
       	See test/gtest for example

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
