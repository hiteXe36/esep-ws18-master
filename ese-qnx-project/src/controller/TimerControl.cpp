#include <vector>
#include <algorithm>
#include <utility>
#include <memory>
#include <sys/neutrino.h>

#include "TimerControl.h"
#include "../servicetools/Logger.h"
#include "../servicetools/Timer.h"
#include "../servicetools/timerinfo.h"
#include "../servicetools/Message.h"

#define TIMEOUT 1

TimerControl::TimerControl(Channel& timer2disp, Channel& disp2timer) :
		_2me(disp2timer), _2disp(timer2disp) {
	_chid = ChannelCreate(0);
	_waitForPulsesThread = std::thread(&TimerControl::waitForPulses, this);

}

void TimerControl::operator()() {
	while (true) {
		Message rcvmsg = _2me.dequeue();
		LOG_WRITE(
				"TimerControl received Message: "
						+ Enums::signalToString(rcvmsg.signal) + " from src: "
						+ Enums::componentToString(rcvmsg.src));

		switch (rcvmsg.signal) {
		case Signal::STARTTIMER: {
			Timer *timer = new Timer(_2me, rcvmsg._timerinfo, _chid);
			LOG_WRITE(
					"Inserting id: " + std::to_string(rcvmsg._timerinfo._id)
							+ "to timerMap");
			timerList.push_back(timer);
			break;
		}
		case Signal::STOPTIMER: {
			Timer *timer = getTimerWithID(rcvmsg._timerinfo._id);
			if (timer == nullptr) {
				LOG_WRITE("TIMER NOT FOUND");
				break;
			}

			timer->stopTime();

			std::vector<Timer*>::iterator it = std::find(timerList.begin(),
					timerList.end(), timer);
			delete *it;
			timerList.erase(it);
			break;
		}
		case Signal::PAUSEALLTIMERS: {
			for (const auto& t : timerList) {
				t->pauseTimer();
			}
			break;
		}
		case Signal::CONTINUEALLTIMERS: {
			for (const auto& t : timerList) {
				t->continueTimer();
			}
			break;
		}
		default:
			std::cout << "Timer Controller got wrong signal"
					<< Enums::signalToString(rcvmsg.signal) << std::endl;
		}
	}
}

void TimerControl::waitForPulses() {
	struct _pulse pulse;
	while (true) {
		if ((MsgReceivePulse(_chid, &pulse, sizeof(pulse), NULL) == 0)
				&& (pulse.code == TIMEOUT)) {
			Timer *timer = getTimerWithID(pulse.value.sival_int);
			if (timer == nullptr) {
				LOG_WRITE("TIMER NOT FOUND");
				continue;
			}

			if (timer->getStatus() != TimerStatus::STOPPED) {
				Timerinfo info(0, pulse.value.sival_int, pulse.value.sival_int);
				LOG_WRITE(
						"Timer id: " + std::to_string(info._id)
								+ " TIMED OUT. Notifying TimerController");
				_2disp.enqueue(
						Message(Signal::TIMED_OUT, Component::MAIN,
								Component::TIMER_CONTROL, info));
				std::vector<Timer*>::iterator it = std::find(timerList.begin(),
						timerList.end(), timer);
				delete *it;
				timerList.erase(it);
			}
		}
	}
}

Timer * TimerControl::getTimerWithID(int id) {
	Timer *result = nullptr;
	for (const auto& t : timerList) {
		if (id == t->getID()) {
			result = t;
			break;
		}
	}

	return result;
}

TimerControl::~TimerControl() {
	_waitForPulsesThread.join();
}

