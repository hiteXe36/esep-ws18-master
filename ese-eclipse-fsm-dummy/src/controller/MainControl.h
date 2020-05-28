#pragma once

#include "../servicetools/Channel.h"
#include "../servicetools/enums.h"

class MainControl
{
public:
	explicit MainControl(Channel& main2disp, Channel& disp2main);
	void operator()();
	virtual ~MainControl();

	Channel& _2disp;
	Channel& _2me;

	void sendMessage(Signal s, Component c);
private:
	static constexpr Component _thisComponent = Component::MAIN;
};
