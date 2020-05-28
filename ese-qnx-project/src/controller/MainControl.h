#pragma once

#include "../servicetools/Channel.h"
#include "../servicetools/enums.h"
#include "../servicetools/CalibrationValues.h"
class MainControl
{
public:
	explicit MainControl(Channel& main2disp, Channel& disp2main);
	void operator()();
	virtual ~MainControl();

	Channel& _2disp;
	Channel& _2me;

	bool _slide1full = false;
	bool _slide2full = false;
	int _reihung = 0;
	bool manualModeOn = false;
	static const int TIMERFORSLIDE1 = 1;
	static const int TIMERFORSLIDE2 = 2;

	TimerValues _timervalues;



	void signalFuerFehlerStart();
	void signalFuerFehlerStop();

	bool unusedSignalHandler1(Signal s);
	bool unusedSignalHandler2(Signal s);
	void wrongSignalHandle(int puks);
	void wrongPukHandle(int puks);
	void slidesFullHandle(int puks);
	void timeoutHandle(int puks);
	void startCalibration();
	void band1();
	void band2();
	void reset();

	void sendMessage(Signal s, Component c);
private:
	static constexpr Component _thisComponent = Component::MAIN;
};
