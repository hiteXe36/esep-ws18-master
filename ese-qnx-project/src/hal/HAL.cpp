/**
 * @author Adem-Can Agdas, Paul Mathia
 */
#ifndef __DUMMY_TEST

#include "HAL.h"

#ifndef __EXT_QNX
#define __EXT_QNX
#endif
#include <hw/inout.h>
#include <sys/mman.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std;

HAL::HAL()
{
	gpio1 = mmap_device_io(GPIO_SIZE, GPIO1_BASE);
	if (gpio1 == MAP_DEVICE_FAILED)
	{
		perror("mmap_device_io failed: ");
	}

	gpio2 = mmap_device_io(GPIO_SIZE, GPIO2_BASE);
	if (gpio2 == MAP_DEVICE_FAILED)
	{
		perror("mmap_device_io failed: ");
	}
	std::cout << "[HAL] HAL-Objekt erzeugt (Singleton)" << std::endl;
}

HAL& HAL::getHAL()
{
	static HAL Instance;
	return Instance;
}

void HAL::greenLightOn()
{
	out32(gpio1 + SETDATAOUT, (1 << LIGHT_GREEN));
}

void HAL::greenLightOff()
{
	out32(gpio1 + CLEARDATAOUT, (1 << LIGHT_GREEN));
}

void HAL::yellowLightOn()
{
	out32(gpio1 + SETDATAOUT, (1 << LIGHT_YELLOW));
}

void HAL::yellowLightOff()
{
	out32(gpio1 + CLEARDATAOUT, (1 << LIGHT_YELLOW));
}
void HAL::redLightOn()
{
	out32(gpio1 + SETDATAOUT, (1 << LIGHT_RED));
}

void HAL::redLightOff()
{
	out32(gpio1 + CLEARDATAOUT, (1 << LIGHT_RED));
}

void HAL::gateOpen()
{
	out32(gpio1 + SETDATAOUT, (1 << GATE));
}

void HAL::gateClose()
{
	out32(gpio1 + CLEARDATAOUT, (1 << GATE));
}

void HAL::motorOff()
{
	out32(gpio1 + SETDATAOUT, (1 << MOTOR_STOP));
}

void HAL::motorOn()
{
	out32(gpio1 + CLEARDATAOUT, (1 << MOTOR_STOP));
}

void HAL::motorSlow()
{
	out32(gpio1 + SETDATAOUT, (1 << MOTOR_SLOW));
}

void HAL::motorFast()
{
	out32(gpio1 + CLEARDATAOUT, (1 << MOTOR_SLOW));
}

void HAL::motorLeft()
{
	simpleMotorRightOff();
	simpleMotorLeft();
}

void HAL::motorRight()
{
	simpleMotorLeftOff();
	simpleMotorRight();
}

void HAL::simpleMotorLeft()
{
	out32(gpio1 + SETDATAOUT, (1 << MOTOR_LEFT));
}

void HAL::simpleMotorRight()
{
	out32(gpio1 + SETDATAOUT, (1 << MOTOR_RIGHT));
}

void HAL::simpleMotorLeftOff()
{
	out32(gpio1 + CLEARDATAOUT, (1 << MOTOR_LEFT));
}

void HAL::simpleMotorRightOff()
{
	out32(gpio1 + CLEARDATAOUT, (1 << MOTOR_RIGHT));
}

void HAL::motorStop()
{
	simpleMotorLeftOff();
	simpleMotorRightOff();
}

HAL::~HAL()
{
	munmap((void*)gpio1, GPIO_SIZE);
	munmap((void*)gpio2, GPIO_SIZE);
}
#endif
