#ifndef SRC_CONTROLLER_SENSORCONTROL_H_
#define SRC_CONTROLLER_SENSORCONTROL_H_

#include "../servicetools/Channel.h"
static constexpr Component _thisComponent = Component::SENSOR;

class SensorControl
{
public:
	explicit SensorControl(Channel& sensor2disp, Channel& disp2sensor);
	virtual ~SensorControl();

	void operator()();

private:

	Channel& _2disp;
	Channel& _2me;

};

#endif
