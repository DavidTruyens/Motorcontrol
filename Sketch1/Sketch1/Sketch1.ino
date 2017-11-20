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
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(3, 5);
//   avoid using pins with LEDs attached



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

void setup()
{
	Serial.begin(9600);
	Serial.println("Dual VNH5019 Motor Shield");
	md.init();
}

void loop()
{
	//encoder();
	md.setM1Speed(20);
	stopIfFault;
	for (int i = 0; i <= 400; i++)
	{
		encoder();
		md.setM1Speed(i);
		stopIfFault();

		delay(2);
	}

	for (int i = 400; i >= -400; i--)
	{
		md.setM1Speed(i);
		stopIfFault();
		encoder();
		delay(2);
	}

	for (int i = -400; i <= 0; i++)
	{
		md.setM1Speed(i);
		stopIfFault();
		encoder();
		delay(2);
	}


}

long oldPosition = -999;
long prevpos = 0;
long deltap = 0;
int speed = 0;


long lasttime;
long deltat;

void encoder() {
	long newPosition = myEnc.read();
	if (newPosition != oldPosition) {
		oldPosition = newPosition;
		//Serial.println(newPosition);
	}
	deltat = millis() - lasttime;

	if (deltat >= 10) {
		lasttime = millis();
		deltap = newPosition - prevpos;
		/*Serial.print(" delta p: ");
		Serial.print(deltap);
		Serial.print(" en t:");
		Serial.println(deltat);*/
		speed = deltap * 1000 / deltat;
		Serial.print("speed: ");
		Serial.println(speed);
		prevpos = newPosition;
	}


}