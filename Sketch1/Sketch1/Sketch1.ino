#include "DualVNH5019MotorShield.h"
DualVNH5019MotorShield md;

/* Encoder Library - Basic Example
* http://www.pjrc.com/teensy/td_libs_Encoder.html
*
* This example code is in the public domain.
*/

#include "Encoder.h"

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability --> Uno has pin 3 available as interrupt
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(3, 5);
//   avoid using pins with LEDs attached

#include <PID_v1.h>

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, 0.05, 0.1, 0.0001, DIRECT);


void setup()
{
	Serial.begin(9600);
	Serial.println("Dual VNH5019 Motor Shield");
	md.init();
	Setpoint = 1000;
	//turn the PID on
	myPID.SetOutputLimits(-2000, 2000);
	myPID.SetMode(AUTOMATIC);

}

int speed = 0;
float Pvalue = 1;
float gain = 0;
unsigned long prevtime = 0;
unsigned long interval = 1000;
double speedincr = 10;

void loop()
{
	encoder();
	Input = speed;
	myPID.Compute();
	md.setM1Speed(int(Output));
	stopIfFault;
	if (millis() - prevtime >= interval) {
		prevtime = millis();

		if (Setpoint >= 2000) {
			speedincr = -50;
		}
		else if (Setpoint <= -2000) {
			speedincr = 50;
		}

		Setpoint = Setpoint + speedincr;

		Serial.print("Setpoint= ");
		Serial.println(Setpoint);
	}

}

long oldPosition = -999;
long prevpos = 0;
long deltap = 0;

long lasttime;
long deltat;

void encoder() {
	long newPosition = myEnc.read();
	if (newPosition != oldPosition) {
		oldPosition = newPosition;
	}
	deltat = millis() - lasttime;

	if (deltat >= 10) {
		lasttime = millis();
		deltap = newPosition - prevpos;
		speed = deltap * 1000 / deltat;
		Serial.print("speed: ");
		Serial.println(speed);
		prevpos = newPosition;
	}
}

void stopIfFault()
{
	if (md.getM1Fault())
	{
		Serial.println("M1 fault");
		while (1);
	}
	if (md.getM2Fault())
	{
		Serial.println("M2 fault");
		while (1);
	}
}