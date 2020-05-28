#pragma once

#include "../servicetools/Channel.h"
#include "../servicetools/Timer.h"
#include <map>
class TimerControl
{
public:
	explicit TimerControl(Channel& timer2disp, Channel& disp2timer);
	virtual ~TimerControl();

	void operator()();

private:

	Channel& _2me;
	Channel& _2disp;

	static constexpr Component _thisComponent = Component::TIMER_CONTROL;
	std::vector<Timer*> timerList;
	int _chid;
	std::thread	_waitForPulsesThread;

	void waitForPulses();
	Timer* getTimerWithID(int id);
};
