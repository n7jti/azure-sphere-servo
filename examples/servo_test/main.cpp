#include <errno.h>
#include <string.h>
#include <time.h>
#include <applibs/log.h>
#include "../../servo.h"

extern "C" void __cxa_pure_virtual() { while (1); }

int main(void)
{
	bool quit = false; 
	int ret; 
	Log_Debug("Starting ServoTest\n");
	CServo servo(0);

	// The servo parameters might need to be tuned for your particular servo. These are the 
	// settings I found for my TowerPro SG92R.
	//
	// Note:  You SHOULD NOT power your servo from the Azure Sphere.  It draws too much current. 
	// use a separate (usually 5v) power supply for the servo and share a common ground with 
	// the Azure Sphere and then take the data line from the Azure Sphere. 
	//
	// servo.SetServoParameters(20, 500, 2250);

	int _servoFd = servo.Open();
	if (_servoFd < 0) {
		Log_Debug("Failed to Open Servo: %d\n", _servoFd);
		quit = true; 
	}
	
	const struct timespec sleep1s = {1, 0};

	while (!quit) {
		
		int ret; 
		ret = servo.SetAngle(0, 0);
		Log_Debug("servo.SetAngle(0,0): %d\n", ret);
		nanosleep(&sleep1s, nullptr);

		ret = servo.SetAngle(0, 90);
		Log_Debug("servo.SetAngle(0,0): %d\n", ret);
		nanosleep(&sleep1s, nullptr);

		ret = servo.SetAngle(0, 180);
		Log_Debug("servo.SetAngle(0,180): %d\n", ret);
		nanosleep(&sleep1s, nullptr);

		ret = servo.SetAngle(0, 90);
		Log_Debug("servo.SetAngle(0,0): %d\n", ret);
		nanosleep(&sleep1s, nullptr);
	}

    return 0;
}