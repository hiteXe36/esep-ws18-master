#include "MotorControl.h"

#ifdef __DUMMY_TEST
#include "../hal/DummyHAL.h"
#else
#include "../hal/HAL.h"
#endif
#include "../servicetools/Logger.h"

MotorControl::MotorControl(Channel& disp2motor)
		:  _2me(disp2motor)
{

}

void MotorControl::operator()()
{
	while (true)
	{
		Message rcvMsg = _2me.dequeue();
		LOG_WRITE("MotorControllerThread hat eine Message bekommen:" + Enums::signalToString(rcvMsg.signal));
		switch (rcvMsg.signal)
		{
			case Signal::MOTOR_ON:
				motorOn();
				break;
			case Signal::MOTOR_OFF:
				motorOff();
				break;
			case Signal::MOTOR_SLOW:
				motorSlow();
				break;
			case Signal::MOTOR_FAST:
				motorFast();
				break;
			case Signal::MOTOR_RIGHT:
				motorRight();
				break;
			case Signal::MOTOR_LEFT:
				motorLeft();
				break;
			case Signal::MOTOR_STOP:
				motorStop();
				break;
			default:
				LOG_WRITE("Cryptic Signal in Layer 8 detected");
				break;
		}
	}
}

void MotorControl::motorOff()
{
	HALINSTANCE.motorOff();
}

void MotorControl::motorOn()
{
	HALINSTANCE.motorOn();
}

void MotorControl::motorSlow()
{
	HALINSTANCE.motorSlow();
}

void MotorControl::motorFast()
{
	HALINSTANCE.motorFast();
}

void MotorControl::motorLeft()
{
	HALINSTANCE.motorLeft();
}
void MotorControl::motorRight()
{
	HALINSTANCE.motorRight();
}

void MotorControl::motorStop()
{
	HALINSTANCE.motorStop();
}

MotorControl::~MotorControl()
{

}

