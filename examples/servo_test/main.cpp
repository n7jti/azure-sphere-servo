#include <errno.h>
#include <string.h>
#include <time.h>
#include <applibs/log.h>
#include "../../servo.h"

extern "C" void __cxa_pure_virtual()
{
	while (1)
		;
}

int main(void)
{
	bool quit = false;
	Log_Debug("Starting ServoTest\n");
	CServoController servo_controller(1);

	// The servo parameters might need to be tuned for your particular servo. These are the
	// settings I found for my TowerPro SG92R.
	//
	// Note:  You SHOULD NOT power your servo from the Azure Sphere.  It draws too much current.
	// use a separate (usually 5v) power supply for the servo and share a common ground with
	// the Azure Sphere and then take the data line from the Azure Sphere.
	//
	// servo.SetServoParameters(20, 500, 2250);

	int _servoFd = servo_controller.Open();
	if (_servoFd < 0)
	{
		Log_Debug("Failed to Open Servo: %d\n", _servoFd);
		quit = true;
	}

	CServoChannel servo(_servoFd, 0);
	servo.SetServoParameters(20, 500, 2500);
	const struct timespec sleep1s = {1, 0};

	while (!quit)
	{

		int ret;
		ret = servo.SetAngle(0);
		Log_Debug("servo.SetAngle(0): %d\n", ret);
		nanosleep(&sleep1s, nullptr);

		ret = servo.SetAngle(90);
		Log_Debug("servo.SetAngle(90): %d\n", ret);
		nanosleep(&sleep1s, nullptr);

		ret = servo.SetAngle(180);
		Log_Debug("servo.SetAngle(180): %d\n", ret);
		nanosleep(&sleep1s, nullptr);

		ret = servo.SetAngle(90);
		Log_Debug("servo.SetAngle(90): %d\n", ret);
		nanosleep(&sleep1s, nullptr);
	}

	return 0;
}