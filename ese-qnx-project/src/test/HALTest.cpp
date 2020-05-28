/*
 * @author Ömer Kirdas, Adem-Can Agdas
 * @version 0.2
 */

#ifdef __DUMMY_TEST
#include "../hal/DummyHAL.h"
#else
#include "../hal/HAL.h"
#endif

#include <iostream>
#include <chrono>
#include <thread>

class HALTest {

public:
	HALTest() {
		HALINSTANCE.motorRight();
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));

		HALINSTANCE.motorSlow();
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));

		HALINSTANCE.motorStop();
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));

		HALINSTANCE.motorLeft();
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));

		HALINSTANCE.motorFast();
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));

		HALINSTANCE.motorStop();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		HALINSTANCE.gateOpen();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));

		HALINSTANCE.gateClose();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		HALINSTANCE.redLightOn();
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

		HALINSTANCE.redLightOff();
		HALINSTANCE.yellowLightOn();
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

		HALINSTANCE.yellowLightOff();
		HALINSTANCE.greenLightOn();
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

		HALINSTANCE.greenLightOff();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));

		HALINSTANCE.greenLightOn();
		HALINSTANCE.redLightOn();
		HALINSTANCE.yellowLightOn();
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

		HALINSTANCE.greenLightOff();
		HALINSTANCE.redLightOff();
		HALINSTANCE.yellowLightOff();
	}
};

