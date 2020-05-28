 /**
 * The main Startup-Class. All Controller threads, etc. are invoked here.
 * @author Adem-Can Agdas, Paul Mathia
 */
//#ifndef __ACTUATOR_TEST

#ifdef __DUMMY_TEST
#include "hal/DummyHAL.h"
#else
#include "hal/HAL.h"
#endif /* __DUMMY_TEST */

#include "controller/MainControl.h"
#include "controller/LampControl.h"
#include "controller/GateControl.h"
#include "controller/MotorControl.h"
#include "controller/EmbeddedRecorder.h"
#include "remoteControl/RemoteControl.h"
#include "controller/SensorControl.h"
#include "controller/Dispatcher.h"
#include "servicetools/Channel.h"
#include "servicetools/Message.h"
#include "servicetools/enums.h"
#include "servicetools/Logger.h"
#include "test/HALTest.cpp"
#include "test/SerialTest.cpp"
#include "test/RemoteTest.cpp"
#include "test/TimerTest.cpp"
#include "test/CalibrationTest.cpp"
#include "test/InterruptTest.cpp"
#include "controller/HightControl.h"
#include "servicetools/CalibrationValues.h"
#include "test/HightFSMTest.cpp"
#include "controller/TimerControl.h"
#include "controller/SerialController.h"
#include "sensor/ADC.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

void RunMain()
{

	LOG_ENABLE_CONSOLE_OUTPUT
	LOG_WRITE("TRANSMISSION STARTS HERE");
	LOG_WRITE("--------------------------------------------------------------------------------------------");
	LOG_WRITE("--------------------------------------------------------------------------------------------");
	LOG_WRITE("--------------------------------------------------------------------------------------------");
	
	Channel disp2gate(5);
	Channel disp2motor(5);
	Channel disp2main(5);
	Channel disp2lamp(5);
	Channel disp2emrec(5);
	Channel disp2sens(5);
	Channel disp2hight(5);
	Channel disp2cali(5);
	Channel disp2serial(5);
	Channel disp2remote(10);
	Channel disp2TimerControl(30);
	Channel omniChannel(30);

	LOG_WRITE("Creating and starting threads.");

	// TODO: replace with puck values from calibration
	ADC adc;

	struct HightValues hightValues;
	hightValues.hightOfHighPuck = HIGH_PUCK_HIGHT;
	hightValues.hightOfFlatPuck= FLAT_PUCK_HIGHT;
	hightValues.hightOfOne= ONE_HIGHT;
	hightValues.hightOfZero= ZERO_HIGHT;
	hightValues.drillingHight =3250;
	hightValues.delta = DELTA_MAX;


	MainControl mc(omniChannel, disp2main);
	LampControl lc(disp2lamp);
	MotorControl moc(disp2motor);
	GateControl gc(disp2gate);
	EmbeddedRecorder er(omniChannel,disp2emrec);
	SensorControl sc(omniChannel, disp2sens);
	HightControl hc(omniChannel, disp2hight, adc, hightValues);
	RemoteControl rc(omniChannel, disp2remote);
	TimerControl tc(omniChannel,disp2TimerControl);
	CalibrationControl cc(omniChannel, disp2cali, adc);
	SerialController sec(omniChannel, disp2serial);
	Dispatcher dc(disp2gate, disp2motor, disp2lamp, disp2emrec, disp2sens, disp2hight, disp2remote,disp2TimerControl, disp2cali, disp2serial, disp2main, omniChannel, EXECMODE::MAIN);

	std::thread tct(std::ref(tc));
	std::thread mct(std::ref(mc));
	std::thread lct(std::ref(lc));
	std::thread hct(std::ref(hc));
	std::thread sct(std::ref(sc));
	std::thread moct(std::ref(moc));
	std::thread sect(std::ref(sec));
	std::thread gct(std::ref(gc));
	std::thread ert(std::ref(er));
	std::thread rct(std::ref(rc));
	std::thread dct(std::ref(dc));
	std::thread cct(std::ref(cc));

	tct.join();
	cct.join();
	mct.join();
	dct.join();
	gct.join();
	lct.join();
	ert.join();
	rct.join();
	hct.join();
	sct.join();
	sect.join();
	moct.join();
}

void RunTest()
{
#ifdef __SERIAL_TEST
	SerialTest();
#endif

#ifdef __REMOTE_TEST
	RemoteTest();
#endif

#ifdef __INTERRUPT_TEST
	InterruptTest();
#endif

#ifdef __ACTUATOR_TEST
	HALTest();
#endif

#ifdef __TIMER_TES
	TimerTest();
#endif

}

int main()
{
#ifdef __TEST
	RunTest();
#else
	RunMain();
#endif /* ifdef TEST */
}



