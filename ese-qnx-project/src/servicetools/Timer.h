/*
 * Timer.h
 *
 *  Created on: 21.11.2018
 *      Author: ace146
 */

#ifndef SRC_SERVICETOOLS_TIMER_H_
#define SRC_SERVICETOOLS_TIMER_H_

#include "Channel.h"
#include "timerinfo.h"

enum class TimerStatus{
	RUNNING,
	PAUSED,
	STOPPED,
};


class Timer {
public:
	/**
	 * @brief Constructor to create Timer-Object
	 * @param ch Channel to send messages to TimerControl
	 * @param info Timerinfo for TimerControl
	 */
	Timer(Channel& ch, Timerinfo info, int chid);
	/**
	 *
	 */
	void operator()();

	void pauseTimer();
	void continueTimer();
	TimerStatus getStatus();
	int getID();
	virtual ~Timer();
	/**
	 * @brief stops a timer.
	 */
	void stopTime();

private:
	Channel& _toTimerControl;
	struct timespec _timeLeft;
	TimerStatus _status;
	Timerinfo _info;
	timer_t timer_id;
	int chid;
};

#endif /* SRC_SERVICETOOLS_TIMER_H_ */
