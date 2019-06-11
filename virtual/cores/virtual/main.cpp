/*
  main.cpp - Main loop for Arduino sketches
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>
#include "virtual_io.h"
#include <iostream>

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

void setupUSB() __attribute__((weak));
void setupUSB() { }

void parseCommandLine(int argc, char* argv[]) __attribute__((weak));
void parseCommandLine(int argc, char* argv[]) { }

// This function can be overridden in other libraries
// e.g. to run a testing framework.
//
__attribute__((weak))
extern void executeTestFunction();

void init(void) {
  // Arduino core does some device-related setup here.
  // We don't need to do anything.
}

int main(int argc, char* argv[]) {
   
   // Enable consumer code to read command line args.
   //
   parseCommandLine(argc, argv);
   
  if (!initVirtualInput(argc, argv)) return 1;
  
  if(testFunctionExecutionRequested()) {
     if(executeTestFunction) {
        executeTestFunction();
     }
     return 0;
  }

  init();
  initVariant();

  setup();

  while (true) {
    std::cout << "Starting cycle " << currentCycle() << std::endl;
    loop();
    if (serialEventRun) serialEventRun();
    nextCycle();
  }

  return 0;
}
