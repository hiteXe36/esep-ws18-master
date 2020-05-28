#include "MainControl.h"

//#ifdef __DUMMY_TEST
//#include "../hal/DummyHAL.h"
//#else
//#include "../hal/HAL.h"
//#endif
//#include "../servicetools/Logger.h"

#include <iostream>
#include <vector>
#include"../servicetools/enums.h"
#include "../servicetools/Machine.h"
#include "../servicetools/PukFSM1.h"
#include "../servicetools/PukFSM2.h"

//		this_thread::sleep_for(chrono::milliseconds(5000));

MainControl::MainControl(Channel& main2disp, Channel& disp2main) :
		_2disp(main2disp), _2me(disp2main)
{

}

void MainControl::operator()()
{
	if (!manualModeOn)
	{
		if (MY_MACHINE)
		{
			band1();
		}
		else if (!MY_MACHINE)
		{
			band2();
		}
	}
}

void MainControl::band1()
{
//	startCalibration();
//	LOG_WRITE("bandOne2bandTwo: " + std::to_string(_timervalues.bandOne2bandTwo));
//	LOG_WRITE("delta: " + std::to_string(_timervalues.delta));
//	LOG_WRITE("entry2hightLong: " + std::to_string(_timervalues.entry2hightLong));
//	LOG_WRITE("entry2hightShort: " + std::to_string(_timervalues.entry2hightShort));
//	LOG_WRITE("gate2exit: " + std::to_string(_timervalues.gate2exit));
//	LOG_WRITE("gate2slide: " + std::to_string(_timervalues.gate2slide));
//	LOG_WRITE("hight2gate: " + std::to_string(_timervalues.hight2gate));

	_timervalues.bandOne2bandTwo = 1500;
	_timervalues.delta = 25;
	_timervalues.entry2hightLong = 2740;
	_timervalues.entry2hightShort = 2261;
	_timervalues.hight2gate = 1350;
	_timervalues.gate2exit = 2100;
	_timervalues.gate2slide = 1353;

	std::vector<PukFSM1*> pukqueue;
	int id = 10;
	bool rightSignal = true;
	bool machine2busy = false;
	bool signalforPuckFSM = true;
	bool timeout = false;
	bool wrongPuk = false;
	int puksInSlide = 0;
	Message rcvmsg;

//	********************************************Kalibrierung*************************************************************************
//	while(inCalibration){
//		Message rcvmsg = _2me.dequeue();
//		_2disp.enqueue(Message(Signal::))
//	}

//***********************************************BAND1 MAIN***************************************************************************
	while (true)
	{
		if (pukqueue.size() != 0)
		{
			LOG_WRITE("AKTUELLER STATE VOR received!!!!!!!!!!!!!" + Enums::stateToString(pukqueue.at(0)->getState()));
			LOG_WRITE("state int: " + std::to_string(pukqueue.at(0)->getState()));
		}
		rcvmsg = _2me.dequeue();
		signalforPuckFSM = unusedSignalHandler1(rcvmsg.signal);
		LOG_WRITE("Main received message with signal: " + Enums::signalToString(rcvmsg.signal) + " from " + Enums::componentToString(rcvmsg.src));

		switch (rcvmsg.signal)
		{
		case Signal::LB_ENTRY_ENTERED:
			{
			id = id + 1;
			pukqueue.push_back(new PukFSM1(id, _2disp, _2me, _timervalues));
			if (pukqueue.size() == 1)
			{
				_2disp.enqueue(Message(Signal::MOTOR_RIGHT, Component::MOTOR, Component::MAIN));
				_2disp.enqueue(Message(Signal::GREEN_LIGHT_ON, Component::LAMP, Component::MAIN));
				_2disp.enqueue(Message(Signal::GREEN_LIGHT_ON, Component::OTHER_MACHINE, Component::MAIN));
			}

			signalforPuckFSM = false;
		}
			break;

		case Signal::LB_SLIDE_LEFT:
			{
			Timerinfo *info = new Timerinfo(3000, TIMERFORSLIDE1, TIMERFORSLIDE1);
			_2disp.enqueue(Message(Signal::STOPTIMER, Component::TIMER_CONTROL, Component::MAIN, *info));
			signalforPuckFSM = false;
		}
			break;
		case Signal::MACHINE2BUSY:
			{
			machine2busy = true;
			signalforPuckFSM = false;
		}
			break;

		case Signal::MACHINE2IDLE:
			{
			machine2busy = false;
			signalforPuckFSM = true;
		}
			break;

		case Signal::SLIDE2FULL:
			{
			_slide2full = true;
			signalforPuckFSM = false;
		}
			break;

		case Signal::SLIDE2EMPTY:
			{
			_slide2full = false;
			signalforPuckFSM = false;

		}
			break;

		case Signal::WRONGPUK:
			{
			LOG_WRITE("PUK MIT DEM TYPEN:" + Enums::pukTypeToString(rcvmsg._pukdata) + " HAT SICH ÜBERSCHLAGEN!");
			wrongPuk = true;
			signalforPuckFSM = false;
		}
			break;

		case Signal::WRONGSIGNAL:
			{
			rightSignal = false;
			signalforPuckFSM = false;
		}
			break;

		case Signal::TIMED_OUT:
			{
			if (rcvmsg._timerinfo._id == 0)
			{
				_2disp.enqueue(Message(Signal::GATE_CLOSE, Component::GATE, Component::MAIN));
			}
			else if (rcvmsg._timerinfo._id == TIMERFORSLIDE1) //SLIDE1FULL
			{
				_slide1full = true;
			}
			else
			{
				timeout = true;
			}

			signalforPuckFSM = false;

		}
			break;

		case Signal::ESTOP_PRESSED:
			{
			_2disp.enqueue(Message(Signal::ESTOP_PRESSED, Component::OTHER_MACHINE, Component::MAIN));
			reset();
			for (int i = pukqueue.size() - 1; i >= 0; i--)
			{
				auto iter = pukqueue.begin() + i;
				delete *iter;
				pukqueue.erase(iter);
			}

			_slide1full = false;
			_slide2full = false;
			id = 10;
			rightSignal = true;
			machine2busy = false;
			signalforPuckFSM = false;
			timeout = false;
			wrongPuk = false;
			puksInSlide = 0;
		}
			break;
		case Signal::ESTOP_RELEASED:
			{
			_2disp.enqueue(Message(Signal::GREEN_LIGHT_ON, Component::LAMP, Component::MAIN));
			_2disp.enqueue(Message(Signal::RED_LIGHT_OFF, Component::LAMP, Component::MAIN));
			_2disp.enqueue(Message(Signal::RED_LIGHT_OFF, Component::OTHER_MACHINE, Component::MAIN));
			_2disp.enqueue(Message(Signal::GREEN_LIGHT_ON, Component::OTHER_MACHINE, Component::MAIN));
			signalforPuckFSM = false;
		}
			break;
		default:
			;
		}

		//**************************************SIGNALE DIE NUR EINEN PUK BETREFFEN***************************************************
		if (signalforPuckFSM)
		{
			for (int i = pukqueue.size() - 1; i >= 0; i--)
			{
				LOG_WRITE("PUK1 IM STATE: " + Enums::stateToString(pukqueue.at(i)->getState()));
				rightSignal = pukqueue.at(i)->switchState(rcvmsg.signal, machine2busy, _slide1full, _slide2full, _reihung);

				if (rightSignal)
				{
					//LOG_WRITE("PUK1 IM STATE: " + Enums::stateToString(pukqueue.at(i)->getState()));

					if (rcvmsg.signal == Signal::LB_SLIDE_ENTERED || rcvmsg.signal == Signal::PASSINGCONFIRMED)
					{
						auto iter = pukqueue.begin() + i;
						delete *iter;
						pukqueue.erase(iter);
						puksInSlide++;

						LOG_WRITE("QUEUEGROEßE " + std::to_string(pukqueue.size()));

						if (pukqueue.size() == 0)
						{
							_2disp.enqueue(Message(Signal::MOTOR_STOP, Component::MOTOR, Component::MAIN));
						}

					}

					break;
				}
			}

		}
		//*****************************************************************************************************************//

//		//*****************************FEHLERBEHANDLUNG bei einem Signal das zu keinem Puk passt***************************

		//BEI BTN_START_RELESED GEHT ER ERNEUT IN DEN FEHLERZUSTAND
		if(rcvmsg.signal == Signal::MACHINE2IDLE){
			rightSignal = true;
		}

		if (!rightSignal)
		{
			wrongSignalHandle(pukqueue.size());
		}

//		//*****************************FEHLERBEHANDLUNG wenn beide Rutschen voll sind//***************************************
		if (_slide1full && _slide2full)
		{
			slidesFullHandle(pukqueue.size());
			_slide1full = false;
			_slide2full = false;
		}

//		//************************************FEHLERBEHANDLUNG wenn es einen TIMEOUT gab***************************************
//		if (timeout)
//		{
//			timeoutHandle(pukqueue.size());
//			timeout = false;
//		}
////
////		//***************************************FEHLERBEHANDLUN wenn es ein falscher Puk war*********************************
		if (wrongPuk)
		{
			wrongPukHandle(pukqueue.size());
			wrongPuk = false;
		}
		rightSignal = true;
	}

}

void MainControl::band2()
{

	Message rcvmsg;
//	do
//	{
//		rcvmsg = _2me.dequeue();
//	} while (rcvmsg.signal != Signal::CALIBRATIONVALUES);
//
//	_timervalues = rcvmsg.calibrationValues.timerValues;

	_timervalues.bandOne2bandTwo = 1500;
	_timervalues.delta = 25;
	_timervalues.entry2hightLong = 2740;
	_timervalues.entry2hightShort = 2261;
	_timervalues.hight2gate = 1350;
	_timervalues.gate2exit = 2100;
	_timervalues.gate2slide = 1353;

	LOG_WRITE("bandOne2bandTwo: " + std::to_string(_timervalues.bandOne2bandTwo));
	LOG_WRITE("delta: " + std::to_string(_timervalues.delta));
	LOG_WRITE("entry2hightLong: " + std::to_string(_timervalues.entry2hightLong));
	LOG_WRITE("entry2hightShort: " + std::to_string(_timervalues.entry2hightShort));
	LOG_WRITE("gate2exit: " + std::to_string(_timervalues.gate2exit));
	LOG_WRITE("gate2slide: " + std::to_string(_timervalues.gate2slide));
	LOG_WRITE("hight2gate: " + std::to_string(_timervalues.hight2gate));

	//	Machine1 to Machine2: 853ms
	//	Delta: 25
	//	Entry to Hight long: 2740ms
	//	Entry to Hight short: 2261ms
	//	Hight to Gate: 1257ms
	//	Gate to Exit: 1986ms
	//	Gate to Slide: 1353ms

	std::vector<PukFSM2*> pukqueue;
	bool rightSignal = true;
	bool signalForPukFSM = true;
	//********************************************************BAND2 Main*******************************************************
	while (true)
	{
		if (pukqueue.size() != 0)
		{
			LOG_WRITE("AKTUELLER STATE VOR received!!!!!!!!!!!!!" + Enums::stateToString(pukqueue.at(0)->getState()));
			LOG_WRITE(
					"state int: " + std::to_string(pukqueue.at(0)->getState()) + " puktype: " + (Enums::pukTypeToString(pukqueue.at(0)->getPukType())) + " pukid: "
							+ std::to_string(pukqueue.at(0)->getPukID()));
		}
		rcvmsg = _2me.dequeue();
		rightSignal = true;
		signalForPukFSM = unusedSignalHandler2(rcvmsg.signal);
		LOG_WRITE("Main received message with signal: " + Enums::signalToString(rcvmsg.signal) + " from " + Enums::componentToString(rcvmsg.src));
		if (pukqueue.size() != 0)
			LOG_WRITE("AKTUELLER STATE nach received!!!!!!!!!!!!!" + Enums::stateToString(pukqueue.at(0)->getState()));

		switch (rcvmsg.signal)
		{
		case Signal::MOTOR_RIGHT:
			{
			if (!pukqueue.empty())
			{
				if (pukqueue.at(0)->getState() != READY)
				{
					_2disp.enqueue(Message(Signal::MOTOR_RIGHT, Component::MOTOR, Component::MAIN));
				}

			}
			signalForPukFSM = false;

		}

			break;
		case Signal::MOTOR_STOP:
			{
			_2disp.enqueue(Message(Signal::MOTOR_STOP, Component::MOTOR, Component::MAIN));
			signalForPukFSM = false;
		}
			break;
		case Signal::PAUSEALLTIMERS:
			{
			_2disp.enqueue(Message(Signal::PAUSEALLTIMERS, Component::TIMER_CONTROL, Component::MAIN));
			signalForPukFSM = false;
			;
		}
			break;
		case Signal::CONTINUEALLTIMERS:
			{
			_2disp.enqueue(Message(Signal::CONTINUEALLTIMERS, Component::TIMER_CONTROL, Component::MAIN));
			signalForPukFSM = false;
			;
		}
			break;

		case Signal::RED_LIGHT_BLINK_FAST:
			{
			_2disp.enqueue(Message(Signal::RED_LIGHT_BLINK_FAST, Component::LAMP, Component::MAIN));
			signalForPukFSM = false;
			;
		}
			break;

		case Signal::RED_LIGHT_OFF:
			{
			_2disp.enqueue(Message(Signal::RED_LIGHT_OFF, Component::LAMP, Component::MAIN));
			signalForPukFSM = false;
			;
		}
			break;

		case Signal::GREEN_LIGHT_ON:
			{
			_2disp.enqueue(Message(Signal::GREEN_LIGHT_ON, Component::LAMP, Component::MAIN));
			signalForPukFSM = false;
			;
		}

			break;

		case Signal::GREEN_LIGHT_OFF:
			{
			_2disp.enqueue(Message(Signal::GREEN_LIGHT_OFF, Component::LAMP, Component::MAIN));
			signalForPukFSM = false;
			;
		}

			break;
		case Signal::GREEN_LIGHT_BLINK_FAST:
			{
			_2disp.enqueue(Message(Signal::GREEN_LIGHT_BLINK_FAST, Component::LAMP, Component::MAIN));
			signalForPukFSM = false;
			;
		}

			break;
		case Signal::YELLOW_LIGHT_BLINK_FAST:
			{
			_2disp.enqueue(Message(Signal::YELLOW_LIGHT_BLINK_FAST, Component::LAMP, Component::MAIN));
			signalForPukFSM = false;
			;
		}

			break;
		case Signal::YELLOW_LIGHT_OFF:
			{
			_2disp.enqueue(Message(Signal::YELLOW_LIGHT_OFF, Component::LAMP, Component::MAIN));
			signalForPukFSM = false;
			;
		}

			break;
		case Signal::YELLOW_LIGHT_ON:
			{
			_2disp.enqueue(Message(Signal::YELLOW_LIGHT_ON, Component::LAMP, Component::MAIN));
			signalForPukFSM = false;
			;
		}

			break;
		case Signal::YELLOW_LIGHT_BLINK_OFF:
			{
			_2disp.enqueue(Message(Signal::YELLOW_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
			signalForPukFSM = false;
			;
		}

			break;
		case Signal::RED_LIGHT_BLINK_OFF:
			{
			_2disp.enqueue(Message(Signal::RED_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
			signalForPukFSM = false;
			;
		}

			break;
		case Signal::GREEN_LIGHT_BLINK_OFF:
			{
			_2disp.enqueue(Message(Signal::GREEN_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
			signalForPukFSM = false;
			;
		}

			break;
		case Signal::PUKDATA:
			{
			LOG_WRITE("PUK IN DIE QUEUE: " + Enums::pukTypeToString(rcvmsg._pukdata));
			pukqueue.push_back(new PukFSM2(rcvmsg._pukdata, rcvmsg.id_info, _2disp, _2me, _timervalues));
			LOG_WRITE("State von übergebene PUK: " + Enums::stateToString(pukqueue.at(pukqueue.size() - 1)->getState()));
			signalForPukFSM = false;
			;
		}

			break;

		case Signal::ESTOP_PRESSED:
			{
			reset();

			for (int i = pukqueue.size() - 1; i >= 0; i--)
			{
				auto iter = pukqueue.begin() + i;
				delete *iter;
				pukqueue.erase(iter);
			}
			signalForPukFSM = false;
			rightSignal = true;
			_slide1full = false;
			_slide2full = false;
			_reihung = 0;
			_reihung = 0;
		}

			break;
		case Signal::TIMED_OUT:
			{
			if (rcvmsg._timerinfo._id == 0)
			{
				LOG_WRITE("AKTUELLER STATE!!!!!!!!!!!!!" + Enums::stateToString(pukqueue.at(0)->getState()));
				_2disp.enqueue(Message(Signal::GATE_CLOSE, Component::GATE, Component::MAIN));
			}
			else if (rcvmsg._timerinfo._id == TIMERFORSLIDE2)
			{
				_2disp.enqueue(Message(Signal::SLIDE2FULL, Component::OTHER_MACHINE, Component::MAIN));
			}
			else
			{
				_2disp.enqueue(Message(Signal::TIMED_OUT, Component::OTHER_MACHINE, Component::MAIN));
			}
			signalForPukFSM = false;
		}
			break;
		default:
			LOG_WRITE("Ignoring signal not handled: " + Enums::signalToString(rcvmsg.signal));
		}

		if (signalForPukFSM && !pukqueue.empty())
		{

			//AB HIER GILT: JEDES SIGNAL DAS KOMMT INTERESSIERT DEN PUK
			switch (rcvmsg.signal)
			{

			case Signal::LB_ENTRY_ENTERED:
				{
				rightSignal = pukqueue.at(0)->switchState(rcvmsg.signal, _reihung);
			}

				break;
			case Signal::LB_HIGHT_ENTERED:
				{
				rightSignal = pukqueue.at(0)->switchState(rcvmsg.signal, _reihung);
			}

				break;
			case Signal::LB_METAL_ENTERED:
				{
				rightSignal = pukqueue.at(0)->switchState(rcvmsg.signal, _reihung);

				if (rightSignal)
				{
					if (pukqueue.at(0)->getState() == IN_GATE)
					{
						_reihung++;
						_reihung = (_reihung) % 3;
					}

				}
			}

				break;
			case Signal::LB_SLIDE_ENTERED:
				{
				LOG_WRITE("IM STATE: " + Enums::stateToString(pukqueue.at(0)->getState()));
				rightSignal = pukqueue.at(0)->switchState(rcvmsg.signal, _reihung);

				break;
			}
			case Signal::LB_SLIDE_LEFT:
				{

				rightSignal = pukqueue.at(0)->switchState(rcvmsg.signal, _reihung);
				if (rightSignal)
				{
					LOG_WRITE("QUEUEGROßE AUF BAND 2" + std::to_string(pukqueue.size()));
					auto iter = pukqueue.begin();
					delete *iter;
					pukqueue.erase(iter);

					LOG_WRITE("QUEUEGROßE AUF BAND 2" + std::to_string(pukqueue.size()));
				}
				_2disp.enqueue(Message(Signal::SLIDE2EMPTY, Component::OTHER_MACHINE, Component::MAIN));

			}

				break;
			case Signal::LB_EXIT_ENTERED:
				{
				LOG_WRITE("IM STATE: " + Enums::stateToString(pukqueue.at(0)->getState()));
				rightSignal = pukqueue.at(0)->switchState(rcvmsg.signal, _reihung);
				//KONSOLENPRINT HERE!!

			}

				break;
			case Signal::LB_EXIT_LEFT:
				{
				LOG_WRITE("IM STATE: " + Enums::stateToString(pukqueue.at(0)->getState()));
				rightSignal = pukqueue.at(0)->switchState(rcvmsg.signal, _reihung);

				if (rightSignal)
				{
					auto iter = pukqueue.begin();
					delete *iter;
					pukqueue.erase(iter);
					LOG_WRITE("QUEUEGROßE AUF BAND 2: " + std::to_string(pukqueue.size()));
				}
			}

				break;
			default:
				LOG_WRITE("Ignoring signal not handled: " + Enums::signalToString(rcvmsg.signal));
			}
		}
		if (!rightSignal)
		{
			_2disp.enqueue(Message(Signal::WRONGSIGNAL, Component::OTHER_MACHINE, Component::MAIN));
			rightSignal = true;
		}

	}

}

void MainControl::sendMessage(Signal s, Component c)
{
	Message newmsg(s, c, _thisComponent);
	_2disp.enqueue(newmsg);
}

void MainControl::signalFuerFehlerStart()
{
//ACTION FÜR BAND2
	_2disp.enqueue(Message(Signal::PAUSEALLTIMERS, Component::OTHER_MACHINE, Component::MAIN));
	_2disp.enqueue(Message(Signal::MOTOR_STOP, Component::OTHER_MACHINE, Component::MAIN));
	_2disp.enqueue(Message(Signal::RED_LIGHT_BLINK_FAST, Component::OTHER_MACHINE, Component::MAIN));

//ACTION FÜR BAND1
	_2disp.enqueue(Message(Signal::PAUSEALLTIMERS, Component::TIMER_CONTROL, Component::MAIN));
	_2disp.enqueue(Message(Signal::MOTOR_STOP, Component::MOTOR, Component::MAIN));
	_2disp.enqueue(Message(Signal::RED_LIGHT_BLINK_FAST, Component::LAMP, Component::MAIN));
}

void MainControl::signalFuerFehlerStop()
{
//ACTION FÜR BAND2
	_2disp.enqueue(Message(Signal::CONTINUEALLTIMERS, Component::OTHER_MACHINE, Component::MAIN));
	_2disp.enqueue(Message(Signal::MOTOR_RIGHT, Component::OTHER_MACHINE, Component::MAIN));
	_2disp.enqueue(Message(Signal::RED_LIGHT_BLINK_OFF, Component::OTHER_MACHINE, Component::MAIN));
	_2disp.enqueue(Message(Signal::RED_LIGHT_OFF, Component::OTHER_MACHINE, Component::MAIN));

//ACTION FÜR BAND1
	_2disp.enqueue(Message(Signal::CONTINUEALLTIMERS, Component::TIMER_CONTROL, Component::MAIN));
	_2disp.enqueue(Message(Signal::RED_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::RED_LIGHT_OFF, Component::LAMP, Component::MAIN));
}

void MainControl::wrongSignalHandle(int puks)
{
	bool FehlerInBearbeitung = true;
	LOG_WRITE("FINGER WEG VON DEN SENSOREN!");
	signalFuerFehlerStart();
	LOG_WRITE("IM FEHLERAUTOMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAATEN: ");

	_2disp.enqueue(Message(Signal::GREEN_LIGHT_OFF, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_OFF, Component::OTHER_MACHINE, Component::MAIN));

	State state = ANSTEHENDUNQUITIERT;

	while (FehlerInBearbeitung)
	{
		Message rcvmsg = _2me.dequeue();

		switch (rcvmsg.signal)
		{
		case Signal::BTN_RESET_PRESSED:
			{
			if (state == ANSTEHENDUNQUITIERT)
			{
				_2disp.enqueue(Message(Signal::RED_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
				_2disp.enqueue(Message(Signal::RED_LIGHT_ON, Component::LAMP, Component::MAIN));
				state = ANSTEHENDQUITIERT;
			}
		}
			break;
		case Signal::BTN_START_PRESSED:
			{
			if (state == ANSTEHENDQUITIERT)
			{
				FehlerInBearbeitung = false;
			}

		}
			break;
		default:
			;
		}

	}

	_2disp.enqueue(Message(Signal::GREEN_LIGHT_ON, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_ON, Component::OTHER_MACHINE, Component::MAIN));

	if (puks > 0)
	{
		_2disp.enqueue(Message(Signal::MOTOR_RIGHT, Component::MOTOR, Component::MAIN));
	}

	signalFuerFehlerStop();
}

void MainControl::wrongPukHandle(int puks)
{
	bool FehlerInBearbeitung = true;
	State state = ANSTEHENDUNQUITIERT;
	signalFuerFehlerStart();

	_2disp.enqueue(Message(Signal::GREEN_LIGHT_BLINK_FAST, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::YELLOW_LIGHT_BLINK_FAST, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_BLINK_FAST, Component::OTHER_MACHINE, Component::MAIN));
	_2disp.enqueue(Message(Signal::YELLOW_LIGHT_BLINK_FAST, Component::OTHER_MACHINE, Component::MAIN));

	while (FehlerInBearbeitung)
	{
		Message rcvmsg = _2me.dequeue();

		switch (rcvmsg.signal)
		{
		case Signal::BTN_RESET_PRESSED:
			{
			if (state == ANSTEHENDUNQUITIERT)
			{
				_2disp.enqueue(Message(Signal::RED_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
				_2disp.enqueue(Message(Signal::RED_LIGHT_ON, Component::LAMP, Component::MAIN));
				state = ANSTEHENDQUITIERT;
			}

		}
			break;
		case Signal::BTN_START_PRESSED:
			{
			if (state == ANSTEHENDQUITIERT)
			{
				FehlerInBearbeitung = false;
			}
			default:;
		}
		}

	}
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_BLINK_OFF, Component::OTHER_MACHINE, Component::MAIN));
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_ON, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_ON, Component::OTHER_MACHINE, Component::MAIN));
	_2disp.enqueue(Message(Signal::YELLOW_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::YELLOW_LIGHT_BLINK_OFF, Component::OTHER_MACHINE, Component::MAIN));

	if (puks > 0)
	{
		_2disp.enqueue(Message(Signal::MOTOR_RIGHT, Component::MOTOR, Component::MAIN));
	}

	signalFuerFehlerStop();
}
void MainControl::slidesFullHandle(int puks)
{
	bool FehlerInBearbeitung = true;

	signalFuerFehlerStart();
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_OFF, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_OFF, Component::OTHER_MACHINE, Component::MAIN));

	State state = ANSTEHENDUNQUITIERT;

	while (FehlerInBearbeitung)
	{
		Message rcvmsg = _2me.dequeue();

		switch (rcvmsg.signal)
		{
		case Signal::BTN_RESET_PRESSED:
			{
			if (state == ANSTEHENDUNQUITIERT)
			{
				_2disp.enqueue(Message(Signal::RED_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
				_2disp.enqueue(Message(Signal::RED_LIGHT_ON, Component::LAMP, Component::MAIN));
				state = ANSTEHENDQUITIERT;
			}

		}
			break;
		case Signal::BTN_START_PRESSED:
			{
			if (state == ANSTEHENDQUITIERT)
			{
				FehlerInBearbeitung = false;
			}
			default:;
		}
		}

	}

	_2disp.enqueue(Message(Signal::GREEN_LIGHT_ON, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_ON, Component::OTHER_MACHINE, Component::MAIN));

	if (puks > 0)
	{
		_2disp.enqueue(Message(Signal::MOTOR_RIGHT, Component::MOTOR, Component::MAIN));
	}

	signalFuerFehlerStop();
}

void MainControl::timeoutHandle(int puks)
{
	bool FehlerInBearbeitung = true;
	State state = ANSTEHENDUNQUITIERT;
	signalFuerFehlerStart();

	_2disp.enqueue(Message(Signal::GREEN_LIGHT_OFF, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::YELLOW_LIGHT_ON, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_OFF, Component::OTHER_MACHINE, Component::MAIN));
	_2disp.enqueue(Message(Signal::YELLOW_LIGHT_ON, Component::OTHER_MACHINE, Component::MAIN));

	while (FehlerInBearbeitung)
	{
		Message rcvmsg = _2me.dequeue();

		switch (rcvmsg.signal)
		{
		case Signal::BTN_RESET_PRESSED:
			{
			if (state == ANSTEHENDUNQUITIERT)
			{
				_2disp.enqueue(Message(Signal::RED_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
				_2disp.enqueue(Message(Signal::RED_LIGHT_ON, Component::LAMP, Component::MAIN));
				state = ANSTEHENDQUITIERT;
			}

		}
			break;
		case Signal::BTN_START_PRESSED:
			{
			if (state == ANSTEHENDQUITIERT)
			{
				FehlerInBearbeitung = false;
			}

		}
			break;
		default:
			;
		}

	}
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_ON, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::YELLOW_LIGHT_OFF, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_ON, Component::OTHER_MACHINE, Component::MAIN));
	_2disp.enqueue(Message(Signal::YELLOW_LIGHT_OFF, Component::OTHER_MACHINE, Component::MAIN));

	if (puks > 0)
	{
		_2disp.enqueue(Message(Signal::MOTOR_RIGHT, Component::MOTOR, Component::MAIN));
	}

	signalFuerFehlerStop();
}

void MainControl::startCalibration()
{
	_2disp.enqueue(Message(Signal::STARTCALIBRATION, Component::CALIBRATION, Component::MAIN));
	Message rcvmsg;
	do
	{
		rcvmsg = _2me.dequeue();
	}
	while (rcvmsg.signal != Signal::CALIBRATIONVALUES);

	_timervalues = rcvmsg.calibrationValues.timerValues;

}

bool MainControl::unusedSignalHandler1(Signal s)
{
	bool pukForFSM = true;

	switch (s)
	{
	case Signal::LB_ENTRY_LEFT:
		{
		pukForFSM = false;

	}
		break;
	case Signal::LB_HIGHT_LEFT:
		{
		pukForFSM = false;

	}
		break;
	case Signal::LB_METAL_LEFT:
		{
		pukForFSM = false;

	}
		break;
	case Signal::LB_EXIT_LEFT:
		{
		pukForFSM = false;

	}
		break;
	case Signal::GATE_OPEN:
		{
		pukForFSM = false;
	}
		break;
	case Signal::GATE_CLOSE:
		{
		pukForFSM = false;
	}
		break;
	case Signal::IS_METAL:
		{
		pukForFSM = false;
	}
		break;
	case Signal::NOT_METAL:
		{
		pukForFSM = false;
	}
		break;
	case Signal::BTN_START_RELEASED:
		{
		pukForFSM = false;
	}
		break;

	default:
		LOG_WRITE("UNHANDLESIGNALLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL: " + Enums::signalToString(s));
	}

	return pukForFSM;
}

bool MainControl::unusedSignalHandler2(Signal s)
{
	bool pukForFSM = true;

	switch (s)
	{
	case Signal::LB_ENTRY_LEFT:
		{
		pukForFSM = false;

	}
		break;
	case Signal::LB_HIGHT_LEFT:
		{
		pukForFSM = false;

	}
		break;
	case Signal::LB_METAL_LEFT:
		{
		pukForFSM = false;

	}
		break;
	case Signal::GATE_OPEN:
		{
		pukForFSM = false;
	}
		break;
	case Signal::GATE_CLOSE:
		{
		pukForFSM = false;
	}
		break;
	case Signal::IS_METAL:
		{
		pukForFSM = false;
	}
		break;
	case Signal::NOT_METAL:
		{
		pukForFSM = false;
	}
		break;
	case Signal::BTN_START_RELEASED:
		{
		pukForFSM = false;
	}
		break;
	default:
		LOG_WRITE("UNHANDLESIGNALLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL: " + Enums::signalToString(s));;
	}

	return pukForFSM;
}

void MainControl::reset()
{
	_2disp.enqueue(Message(Signal::GATE_CLOSE, Component::GATE, Component::MAIN));
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::GREEN_LIGHT_OFF, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::YELLOW_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::YELLOW_LIGHT_OFF, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::RED_LIGHT_ON, Component::LAMP, Component::MAIN));
	_2disp.enqueue(Message(Signal::MOTOR_STOP, Component::MOTOR, Component::MAIN));
}

MainControl::~MainControl()
{
//NULL;
}

