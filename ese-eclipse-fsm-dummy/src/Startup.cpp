 /**
 * The main Startup-Class. All Controller threads, etc. are invoked here.
 * @author Adem-Can Agdas, Paul Mathia
 */
//#ifndef __ACTUATOR_TEST

//#ifdef __DUMMY_TEST
//#include "hal/DummyHAL.h"
//#else
//#include "hal/HAL.h"
//#endif /* __DUMMY_TEST */

#include "controller/MainControl.h"
//#include "controller/LampControl.h"
#include "controller/GateControl.h"
#include "controller/MotorControl.h"
//#include "controller/EmbeddedRecorder.h"
//#include "remoteControl/RemoteControl.h"
//#include "controller/SensorControl.h"
#include "controller/Dispatcher.h"
#include "servicetools/Channel.h"
#include "servicetools/Message.h"
#include "servicetools/enums.h"
//#include "servicetools/Logger.h"
//#include "test/HALTest.cpp"
//#include "test/SerialTest.cpp"
//#include "test/RemoteTest.cpp"
//#include "test/TimerTest.cpp"
//#include "test/InterruptTest.cpp"
//#include "controller/HightControl.h"
//#include "servicetools/HightValues.h"
//#include "test/HightFSMTest.cpp"
#include "controller/DummyGenerator.h"
#include "controller/SerialControl.h"
#include "controller/PUKTypeControl.h"
#include "controller/TimerControl.h"


#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

void RunMain()
{

//	LOG_ENABLE_CONSOLE_OUTPUT
//	LOG_WRITE("TRANSMISSION STARTS HERE");
//	LOG_WRITE("--------------------------------------------------------------------------------------------");
//	LOG_WRITE("--------------------------------------------------------------------------------------------");
//	LOG_WRITE("--------------------------------------------------------------------------------------------");
	
	Channel disp2gate(5);
	Channel disp2motor(5);
	Channel disp2main(5);
//	Channel disp2lamp(5);
//	Channel disp2emrec(5);
//	Channel disp2sens(5);
//	Channel disp2hight(5);
//	Channel disp2remote(10);
	Channel disp2serial(5);
	Channel disp2puktc(5);
	Channel disp2timer(5);
	Channel omniChannel(30);

//	LOG_WRITE("Erstelle und starte Threads.");

	// TODO: replace with puck values from calibration
//	struct HightValues hightValues;
//	hightValues.hightOfHighPuck = 2260;
//	hightValues.hightOfFlatPuck= 2550;
//	hightValues.hightOfOne= 2635;
//	hightValues.hightOfZero= 2465;
//	hightValues.delta = 30;

	MainControl mc(omniChannel, disp2main);
//	LampControl lc(disp2lamp);
	MotorControl moc(disp2motor);
	GateControl gc(disp2gate);
//	EmbeddedRecorder er(omniChannel,disp2emrec);
//	SensorControl sc(omniChannel, disp2sens);
//	HightControl hc(omniChannel, disp2hight, hightValues);
//	RemoteControl rc(omniChannel, disp2remote);

	DummyGenerator dummygenerator(omniChannel);
	SerialControl serialcontrollerthread(disp2serial,omniChannel);
	PUKTypeControl puktcthread(disp2puktc,omniChannel);
	TimerControl timercontroller(disp2timer);

	Dispatcher dc(disp2gate, disp2motor, /*disp2lamp, disp2emrec, disp2sens, disp2hight, disp2remote,*/ disp2main, disp2serial, disp2puktc, disp2timer, omniChannel/*, EXECMODE::MAIN*/);

	std::thread mct(std::ref(mc));
//	std::thread lct(std::ref(lc));
//	std::thread hct(std::ref(hc));
//	std::thread sct(std::ref(sc));
	std::thread moct(std::ref(moc));
	std::thread gct(std::ref(gc));
//	std::thread ert(std::ref(er));
//	std::thread rct(std::ref(rc));
	std::thread dct(std::ref(dc));
	std::thread sct(std::ref(serialcontrollerthread));
	std::thread dummygeneratorthread(std::ref(dummygenerator));
	std::thread puktct(std::ref(puktcthread));
	std::thread timercontrollerthread(std::ref(timercontroller));

	mct.join();
	dct.join();
	gct.join();
//	lct.join();
//	ert.join();
//	rct.join();
//	hct.join();
//	sct.join();
}

//void RunTest()
//{
//#ifdef __SERIAL_TEST
//	SerialTest();
//#endif
//
//#ifdef __REMOTE_TEST
//	RemoteTest();
//#endif
//
//#ifdef __INTERRUPT_TEST
//	InterruptTest();
//#endif
//
//#ifdef __ACTUATOR_TEST
//	HALTest();
//#endif
//
//#ifdef __TIMER_TES
//	TimerTest();
//#endif
//
//}

int main()
{
//#ifdef __TEST
//	RunTest();
//#else
//	// HightFSMTest();	/* uncomment this and comment RunMain() to run HightFSMTest instead of main */
	RunMain();
//#endif /* ifdef TEST */
}



