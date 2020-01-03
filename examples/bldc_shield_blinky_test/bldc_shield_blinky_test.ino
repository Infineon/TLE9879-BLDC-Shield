
// Include the shield library in your Arduino project
#include "TLE9879_Group.h"

// Create shield group object
TLE9879_Group *shields; 

void setup()
{
  // Initialize the shield group object with the number
  // shields in the stack
  shields = new TLE9879_Group(1);
}

void loop()
{
  // Call the testBlinky() method in the loop of the 
  // Arduino prohect -> the LED should change its
  // color every second
  shields->testBlinky();
}
