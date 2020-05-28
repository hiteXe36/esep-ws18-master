#pragma once

#include "../servicetools/Channel.h"

class GateControl
{
public:
	explicit GateControl(Channel& disp2gate);
	virtual ~GateControl();

	void operator()();

	void openGate();
	void closeGate();

private:

	Channel& _2me;

	static constexpr Component _thisComponent = Component::MOTOR;
};
