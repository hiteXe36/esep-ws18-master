#pragma once

#include "../servicetools/Channel.h"
#include "../servicetools/Message.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>

//enum class EXECMODE
//{
//	MAIN,REPLAY
//};

class Dispatcher
{
public:
	explicit Dispatcher(Channel& disp2gate,
						Channel& disp2motor,
//						Channel& disp2lamp,
//						Channel& disp2emrec,
//						Channel& disp2sens,
//						Channel& disp2hight,
						Channel& disp2main,
						Channel& disp2serial,
						Channel& disp2puktc,
						Channel& disp2timer,
						Channel& toDisp//,
//						Channel& disp2Remote,
/*						EXECMODE mode*/);
	void operator()();
//	std::vector<Message> readfromfile();
	virtual ~Dispatcher();
	Channel& _2gate;
	Channel& _2motor;
//	Channel& _2lamp;
//	Channel& _2emrec;
//	Channel& _2sens;
//	Channel& _2hight;
	Channel& _2main;
	Channel& _2serial;
	Channel& _2puktc;
	Channel& _2timer;
	Channel& _2me;
//	Channel& _2remoteControl;

//	EXECMODE _currentmode;
//	std::ifstream _readstream;

//private:
//	Signal convertStringToSignal(std::string ss);
//	Component convertStringToComponent(std::string ss);

};
