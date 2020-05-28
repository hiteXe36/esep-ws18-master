#include "GateControl.h"
#include <iostream>
//#ifdef __DUMMY_TEST
//#include "../hal/DummyHAL.h"
//#else
//#include "../hal/HAL.h"
//#endif
//#include "../servicetools/Logger.h"

GateControl::GateControl(Channel& disp2gate)
		: _2me(disp2gate)
{

}

void GateControl::operator()()
{
	while (true)
	{
		Message rcvMsg = _2me.dequeue();
//		LOG_WRITE("MotorControllerThread hat eine Message bekommen");
		switch (rcvMsg.signal)
		{
			case Signal::GATE_OPEN:
				openGate();
				break;
			case Signal::GATE_CLOSE:
				closeGate();
				break;
			default:
//				LOG_WRITE("Cryptic Signal in Layer 8 detected");
				break;
		}
	}
}

void GateControl::openGate()
{
	std::cout << "Gate opened" << std::endl;
}

void GateControl::closeGate()
{
    std::cout << "Gate closed" << std::endl;
}

GateControl::~GateControl()
{

}

