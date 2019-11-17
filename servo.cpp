#include <unistd.h>
#include "servo.h"

#define NS_PER_MS 1000000
#define NS_PER_US 1000

CServo::CServo(PWM_ControllerId pwmControllerId)
    : _pwmControllerId(pwmControllerId)
    , _fdPwm(-1)
    , _period_ms(20)
    , _pulse_min_us(1000)
    , _pulse_max_us(2000)
{
    // Default Constructor
}

CServo::~CServo(){
    if (_fdPwm >= 0){
        Close(); 
    }
}

int CServo::Open() {
    _fdPwm = PWM_Open(_pwmControllerId);
    return _fdPwm; 
}

void CServo::Close() { 
    close(_fdPwm); 
    _fdPwm = -1;
}

int CServo::SetServoParameters(uint32_t period_ms, uint32_t pulse_min_us, uint32_t pulse_max_us) {
    int ret = 0;

    if (period_ms > 1000){
        // period is too long!
        ret = -1;
    }
    else if (pulse_min_us >= period_ms * 1000 || pulse_min_us >= pulse_max_us){
        // min period is too big
        ret = -1; 
    }
    else if (pulse_max_us > period_ms * 1000){
        
        ret = -1; 
    }
    else {
        _period_ms = period_ms;
        _pulse_min_us = pulse_min_us;
        _pulse_max_us = pulse_max_us;
    }

    return ret;
}

int CServo::SetAngle(PWM_ChannelId channel, uint8_t angle){
    uint32_t pulse_width_us;
    if (angle == 0){
        pulse_width_us = _pulse_min_us; 
    }
    else if(angle >= 180){
        pulse_width_us = _pulse_max_us; 
    }
    else { // angle is between 1 and 179 inclusive. So it is safe to divide. 
        pulse_width_us = _pulse_min_us + (((_pulse_max_us - _pulse_min_us) * angle) / 180);
    }

	PwmState pwmState;
    pwmState.period_nsec = _period_ms * NS_PER_MS;
    pwmState.dutyCycle_nsec = pulse_width_us * NS_PER_US; 
    pwmState.polarity = PWM_Polarity_Normal;
    pwmState.enabled = true; 

    return PWM_Apply(_fdPwm, channel, &pwmState);
}

int CServo::SetPulseWidth(PWM_ChannelId channel, uint32_t pulse_width_us){
    if (pulse_width_us > _pulse_max_us){
		pulse_width_us = _pulse_max_us;
    }
    else if (pulse_width_us < _pulse_min_us){
		pulse_width_us = _pulse_min_us;
    }
    
	PwmState pwmState;
	pwmState.period_nsec = _period_ms * NS_PER_MS;
	pwmState.dutyCycle_nsec = pulse_width_us * NS_PER_US;
	pwmState.polarity = PWM_Polarity_Normal;
	pwmState.enabled = true;

    return PWM_Apply(_fdPwm, channel, &pwmState);
}