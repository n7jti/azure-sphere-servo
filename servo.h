// Azure Sphere Servo Library by Alan Ludwig (https://github.com/n7jti/azure-sphere-servo)

// MIT License
//
// Copyright (c) 2019 Alan Ludwig
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <applibs/pwm.h>

class CServo {
public: 
    CServo(PWM_ControllerId pwmControllerId);
	~CServo(); 

    // Opens the PWM Controller
    // Preconditions: 
    //   The pwmController is closed (has no open file handles)
    //   The pwmController is correctly added to the app_manifest_json
    // Postconditions: 
    //   if the return value < 0 an error occured. 
    //   if the return value is >= 0 then the pwmController is open and the file descriptor is returned.
    int Open(); 

    // Preconditions: None
    // Postconditions: 
    //  If Open were previously called, then the file descriptor is closed. 
    void Close();

    // Sets the servo paremeters used in the calcuations for subsequent calls to SetAngle or SetPulseTime.
    // This call does not immediatly update the hardware. 
    // Preconditions: 
    //   period_ms is between 1 and 1000
    //   pulse_min_us is strictly less than the period and pulse_max_ms
    //   pulse_max_us is less than or equal to the period and greater than pluse_min_us
    // Postconditions: Internal variables are set, or a non-zero value is returned. 
    int SetServoParameters(uint32_t period_ms = 20, uint32_t pulse_min_us = 1000, uint32_t pulse_max_us = 2000);

    // Set the servo SetAngle
    // Preconditions:
    //   channel is valid for the hardware in use. 
    //   angle is 0 - 180 inclusive
    int SetAngle(PWM_ChannelId channel, uint8_t angle);

    // Sets the pluse width of the pluse sent to the servo.  
    // Preconditions: 
    //   channel is valid for the hardware in use
    //   pulse_width_us is within pulse_min_us and pulse_max_us (inclusive)
    int SetPulseWidth(PWM_ChannelId channel, uint32_t pulse_width_us);

private:
    PWM_ControllerId _pwmControllerId; 
    int _fdPwm; 
    uint32_t _period_ms;
    uint32_t _pulse_min_us; 
    uint32_t _pulse_max_us;
};