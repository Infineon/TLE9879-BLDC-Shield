
// Include the Shield library to your Arduino project
#include "TLE9879_Group.h"

// Declare Shield group object
TLE9879_Group *shields;

void setup()
{
	shields = new TLE9879_Group(2);
	
	// Most of the methods in the TLE9879_Group class 
	// have an optional 'boardnr' parameter, to specify
	// a single Shield to send the command to.
	shields->setMode(HALL, BOARD1);
	shields->setMode(FOC, BOARD1);
	
	shields->setMotorSpeed(500, BOARD1);
	shields->setMotorSpeed(2000, BOARD2);
	
	/*
	You might have to adjust the motor parameters,
	in case the motor does not run properly.
	*/
}

void loop()
{
	shields->setMotorMode(START_MOTOR, BOARD1);
	delay(5000);
	shields->setMotorMode(START_MOTOR, BOARD2);
	delay(5000);
	shields->setMotorMode(STOP_MOTOR, BOARD1);
	delay(5000);
	shields->setMotorMode(STOP_MOTOR, BOARD2);
	delay(5000);
}
