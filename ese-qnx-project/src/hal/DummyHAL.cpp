#ifdef __DUMMY_TEST

#include "DummyHAL.h"
#include "../servicetools/Logger.h"

#include <iostream>
#include <stdint.h>
#include <thread>
#include <chrono>

DummyHAL::DummyHAL()
{
	//Map GPIOs...
}

DummyHAL& DummyHAL::getHAL()
{
	static DummyHAL Instance;
	return Instance;
}

void DummyHAL::greenLightOn()
{
	LOG_WRITE("Green light activated");
}

void DummyHAL::greenLightOff()
{
	LOG_WRITE("Green light deactivated");
}
void DummyHAL::yellowLightOn()
{
	LOG_WRITE("Yellow light activated");
}

void DummyHAL::yellowLightOff()
{
	LOG_WRITE("Yellow light deactivated");
}
void DummyHAL::redLightOn()
{
	LOG_WRITE("Red light activated");
}

void DummyHAL::redLightOff()
{
	LOG_WRITE("Red light deactivated");
}

void DummyHAL::gateOpen()
{
	LOG_WRITE("Gate opened");
}

void DummyHAL::gateClose()
{
	LOG_WRITE("Gate closed");
}

void DummyHAL::motorOff()
{
	LOG_WRITE("MotorStop bit set");
}

void DummyHAL::motorOn()
{
	LOG_WRITE("MotorStop bit cleared");
}

void DummyHAL::motorSlow()
{
	LOG_WRITE("MotorSlow bit set");
}

void DummyHAL::motorFast()
{
	LOG_WRITE("MotorSlow bit cleared");
}

void DummyHAL::motorLeft()
{
	simpleMotorRightOff();
	simpleMotorLeft();
}

void DummyHAL::motorRight()
{
	simpleMotorLeftOff();
	simpleMotorRight();
}

void DummyHAL::simpleMotorLeft()
{
	LOG_WRITE("MotorLeft bit set");
}

void DummyHAL::simpleMotorRight()
{
	LOG_WRITE("MotorRight bit set");
}

void DummyHAL::simpleMotorLeftOff()
{
	LOG_WRITE("MotorLeft bit cleared");
}

void DummyHAL::simpleMotorRightOff()
{
	LOG_WRITE("MotorRight bit cleared");
}

void DummyHAL::motorStop()
{
	simpleMotorLeftOff();
	simpleMotorRightOff();
}

DummyHAL::~DummyHAL()
{
	//Unmap GPIOs...
}
#endif
