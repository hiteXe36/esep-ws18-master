/*
 * PukFSM2.h
 *
 *  Created on: 11.01.2019
 *      Author: ace044
 */

#ifndef SRC_SERVICETOOLS_PUKFSM2_H_
#define SRC_SERVICETOOLS_PUKFSM2_H_

#include "../servicetools/enums.h"
#include "../servicetools/Channel.h"
#include "../servicetools/CalibrationValues.h"

class PukFSM2
{
public:
	explicit PukFSM2(PUKType pukType, int id, Channel& main2disp, Channel& disp2main, TimerValues timervalues);
	PukFSM2() = delete;
	virtual ~PukFSM2();

	Channel& _2disp;
	Channel& _2me;
	PUKType _pukType;
	int _id;
	State _state;
	static const int TIMERIDFORGATE = 0;
	static const int TIMERFORSLIDE2 = 2;
	TimerValues _timervalues;

	bool switchState(Signal s, int reihung);
	PUKType getPukType();
	State getState();
	int getPukID();
	void action();
};

#endif /* SRC_SERVICETOOLS_PUKFSM2_H_ */
