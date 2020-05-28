#pragma once

#include "../servicetools/Channel.h"

class MotorControl
{
public:
	explicit MotorControl(Channel& disp2motor);
	void operator()();
//	void motorOff();
//	void motorOn();
//	void motorSlow();
//	void motorFast();
//
//	void motorLeft();
	void motorRight();
	void motorStop();

	virtual ~MotorControl();
private:

	Channel& _2me;
	static constexpr Component _thisComponent = Component::MOTOR;
};
