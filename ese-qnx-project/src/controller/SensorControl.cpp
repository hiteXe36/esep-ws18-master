
#ifdef __DUMMY_TEST
#include "../hal/DummyHAL.h"
#else
#include "../hal/HAL.h"
#endif

#include <memory>

#include "SensorControl.h"
#include "../servicetools/Logger.h"
#include "../sensor/InterruptHandler.h"
#include "../sensor/GPIO.h"
#include "../servicetools/Message.h"
SensorControl::SensorControl(Channel& sensor2disp, Channel& disp2sensor)
		: 	_2disp(sensor2disp),
			_2me(disp2sensor)
{

}

void SensorControl::operator()()
{
	std::cout << "sensorcontrol started" << std::endl;
	GPIO gpio0(GPIO0_BASE);
	InterruptHandler interruptHandler(&gpio0, GPIO0_INTB);
	interruptHandler.enableInterrupt(PINS_FOR_IRQ_GENERATION);
	Signal signal;
	while (true)
	{
		signal = interruptHandler.awaitInterruptGPIO();
		if(signal == Signal::HIGHT_NOT_OK || signal == Signal::HIGHT_OK || signal == Signal::IS_METAL) {
			_2disp.enqueue(*(std::make_unique<Message>(signal, Component::HIGHT, Component::SENSOR)));
		}
		else if(signal == Signal::LB_HIGHT_ENTERED || signal == Signal::LB_HIGHT_LEFT) {
			_2disp.enqueue(*(std::make_unique<Message>(signal, Component::HIGHT, Component::SENSOR)));
			_2disp.enqueue(*(std::make_unique<Message>(signal, Component::MAIN, Component::SENSOR)));
		}
		else {
			_2disp.enqueue(*(std::make_unique<Message>(signal, Component::MAIN, Component::SENSOR)));
		}
	}
}

SensorControl::~SensorControl()
{

}

