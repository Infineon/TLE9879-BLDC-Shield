
// Include the Shield library to your Arduino project
#include "TLE9879_Group.h"

// Declare Shield group object
TLE9879_Group *shields;

void setup()
{
	// Initialize the Shield group object with the
	// number of Shields in the stack
	shields = new TLE9879_Group(1);
	
	// Set the desired mode (FOC, HALL, BEMF)
	shields->setMode(FOC);
	
	// Set the desired motor speed (RPM)
	shields->setMotorSpeed(1000);
	
	/*
	You might have to adjust the motor parameters,
	in case the motor does not run properly.
	*/
	
	// Start the motor and let it run for 5 seconds,
	// then stop the motor
	shields->setMotorMode(START_MOTOR);
	delay(5000);
	shields->setMotorMode(STOP_MOTOR);
}

void loop()
{
}
