/*
 * Timer.cpp
 *
 *  Created on: 21.11.2018
 *      Author: ace146
 */

#include "Timer.h"
#include <memory>
#include <stdio.h>
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <sched.h>
#include "Message.h"
#include "enums.h"

#define TIMEOUT 1

Timer::Timer(Channel& ch, Timerinfo info, int chid) :
		_toTimerControl(ch), _status(TimerStatus::RUNNING), _info(info), timer_id(
				-1), chid(chid) {
	struct sigevent event;
	struct itimerspec itime;

	event.sigev_notify = SIGEV_PULSE;
	event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, chid, _NTO_SIDE_CHANNEL,
			0);
	event.sigev_priority = SIGEV_PULSE_PRIO_INHERIT;
	event.sigev_code = TIMEOUT;
	event.sigev_value.sival_int = _info._id;
	timer_create(CLOCK_REALTIME, &event, &timer_id);

	int seconds = _info._duration / 1000;
	int nano = (_info._duration % 1000) * 1000000;

	itime.it_value.tv_sec = seconds;
	itime.it_value.tv_nsec = nano;
	itime.it_interval.tv_sec = 0;
	itime.it_interval.tv_nsec = 0;
	timer_settime(timer_id, 0, &itime, NULL);
}

void Timer::pauseTimer() {

	struct itimerspec itime;
	timer_gettime(timer_id, &itime);
	timer_delete(timer_id);
	_status = TimerStatus::PAUSED;
	_timeLeft = itime.it_value;

}
void Timer::continueTimer() {
	struct sigevent event;
	struct itimerspec itime;

	event.sigev_notify = SIGEV_PULSE;
	event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, chid, _NTO_SIDE_CHANNEL,
			0);
	event.sigev_priority = SIGEV_PULSE_PRIO_INHERIT;
	event.sigev_code = TIMEOUT;
	timer_create(CLOCK_REALTIME, &event, &timer_id);

	itime.it_value = _timeLeft;
	timer_settime(timer_id, 0, &itime, NULL);
	_status = TimerStatus::RUNNING;
}

void Timer::stopTime() {
	timer_delete(timer_id);
	timer_id = -1;
	_status = TimerStatus::STOPPED;
}

TimerStatus Timer::getStatus() {
	return _status;
}

int Timer::getID() {
	return _info._id;
}

Timer::~Timer() {

}
