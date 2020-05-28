 /*
 * PukFSM.h
 *
 *  Created on: 07.01.2019
 *      Author: ace044
 */

#ifndef SERVICETOOLS_PUKFSM_H_
#define SERVICETOOLS_PUKFSM_H_
#include "../servicetools/CalibrationValues.h"

#include "../servicetools/enums.h"
#include "../servicetools/Channel.h"

class PukFSM1 {
public:
	explicit PukFSM1(int id,Channel& main2disp, Channel& disp2main, TimerValues timervalues);
	PukFSM1() = delete;
	virtual ~PukFSM1();

	int _id;
	Channel& _2disp;
	Channel& _2me;
	State _state;
	PUKType _pukType;
	static const int TIMERIDFORGATE = 0;
	static const int TIMERFORSLIDE1 = 1;
	TimerValues _timervalues;

	bool switchState(Signal s, bool machine2busy,bool slide1full, bool slide2full, int reihung);
	PUKType getPukType();
	State getState();
	void action();
	void setType(PUKType puktype);
	int getID();


private:
	void sendMessage(Signal s, Component c);
};

#endif /* SERVICETOOLS_PUKFSM_H_ */
