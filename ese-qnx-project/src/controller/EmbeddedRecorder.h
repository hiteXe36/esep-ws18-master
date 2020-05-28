#ifndef SRC_CONTROLLER_EMBEDDEDRECORDER_H_
#define SRC_CONTROLLER_EMBEDDEDRECORDER_H_

#include "../servicetools/Channel.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

class EmbeddedRecorder
{
public:
	explicit EmbeddedRecorder(Channel& emrec2disp, Channel& disp2emrec);
	virtual ~EmbeddedRecorder();

	void operator()();

private:
	Channel& _2disp;
	Channel& _2me;

	//std::string componentToString(Component c);
	//std::string signalToString(Signal s);

	static constexpr Component _thisComponent = Component::EMBEDDED_RECORDER;
	std::ofstream _recordfile;
	int _timeoffset;
};

#endif
