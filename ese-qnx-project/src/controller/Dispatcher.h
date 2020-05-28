#include "../servicetools/Channel.h"
#include "../servicetools/Message.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

#ifndef SRC_DISPATCHER_H_
#define SRC_DISPATCHER_H_

enum class EXECMODE
{
	MAIN,REPLAY
};

class Dispatcher
{
public:
	explicit Dispatcher(Channel& disp2gate,
						Channel& disp2motor,
						Channel& disp2lamp,
						Channel& disp2emrec,
						Channel& disp2sens,
						Channel& disp2hight,
						Channel& disp2Remote,
						Channel& disp2TimerControl,
						Channel& disp2cali,
						Channel& disp2serial,
						Channel& disp2main,
						Channel& toDisp,
						EXECMODE mode);
	void operator()();
	std::vector<Message> readfromfile();
	virtual ~Dispatcher();
	Channel& _2gate;
	Channel& _2motor;
	Channel& _2lamp;
	Channel& _2emrec;
	Channel& _2sens;
	Channel& _2hight;
	Channel& _2remoteControl;
	Channel& _2TimerControl;
	Channel& _2cali;
	Channel& _2serial;
	Channel& _2main;
	Channel& _2me;


	EXECMODE _currentmode;
	std::ifstream _readstream;

private:
	bool _calibration;
	//Signal convertStringToSignal(std::string ss);
	//Component convertStringToComponent(std::string ss);
};

#endif
