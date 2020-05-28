//SO NICHT! _2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_RIGHT, Component::DISPATCHER, Component::MAIN)));
//SO! _2disp.(Message(Signal::MOTOR_RIGHT, Component::Dispatcher, ))
/*
 * PukFSM.cpp
 *
 *  Created on: 07.01.2019
 *      Author: ace044
 */

#include "PukFSM1.h"

#include "../servicetools/Message.h"
#include <thread>
#include <chrono>
#include "../controller/MainControl.h"

PukFSM1::PukFSM1(int id, Channel& main2disp, Channel& disp2main, TimerValues timervalues) :
		_id(id), _2disp(main2disp), _2me(disp2main)
{

//	_2disp = main2disp;
//	_2me = disp2main;
	_state = IN_ENTRY;
	_pukType = TYPENULL;
	_timervalues = timervalues;
	action();
}

PukFSM1::~PukFSM1()
{
	// TODO Auto-generated destructor stub
}

/**
 * Diese Methode ist Zuständig für den Wechsel des Zustandes.
 *
 * liefert false, fals das Signal nicht zu dem Zustand passt. True sonst.
 */
bool PukFSM1::switchState(Signal s, bool machine2busy, bool slide1full, bool slide2full, int reihung)
{
	bool rightSignal = false;
	State state;

	switch (_state)
	{
	case IN_ENTRY:
		{
		rightSignal = (s == Signal::LB_HIGHT_ENTERED);
		state = IN_HEIGHT;
	}
		break;
	case IN_HEIGHT:
		{
		rightSignal = (s == Signal::LB_METAL_ENTERED);

		if (rightSignal)
		{
			Message datarequestmsg(Signal::GETPUKDATA, Component::HIGHT, Component::MAIN);
			_2disp.enqueue(datarequestmsg);
			Message intakedata = _2me.dequeue();

			if (intakedata.signal != Signal::PUKDATA)
			{
				LOG_WRITE("PUKTYPEN HOLEN HAT NICHT GEKLAPPT!");
			}
			_pukType = intakedata._pukdata;

			if (!slide2full
					&& (_pukType == PUKType::TYPE3_1 || _pukType == PUKType::TYPE2_1 || _pukType == PUKType::TYPE4_2 || _pukType == PUKType::TYPE4_5))
			{
				state = IN_GATE;
			}
			else if (!slide1full)
			{
				state = SLIDING;
			}
			else if (!slide2full)
			{
				state = IN_GATE;
			}

		}

	}
		break;
	case SLIDING:
		{
		rightSignal = (s == Signal::LB_SLIDE_ENTERED);
		state = SLIDINGDONE;

	}
		break;
	case IN_GATE:
		{
		rightSignal = (s == Signal::LB_EXIT_ENTERED);

		if (machine2busy)
		{
			state = WAIT4BAND2;
		}
		else
		{
			state = PASSING;
		}

	}
		break;
	case WAIT4BAND2:
		{
		rightSignal = (s == Signal::MACHINE2IDLE);
		if (rightSignal)
		{
			_2disp.enqueue(Message(Signal::MOTOR_RIGHT, Component::MOTOR, Component::MAIN));
			_2disp.enqueue(Message(Signal::CONTINUEALLTIMERS, Component::TIMER_CONTROL, Component::MAIN));
		}
		state = PASSING;

	}
		break;
	case PASSING:
		{
		rightSignal = (s == Signal::PASSINGCONFIRMED);
		state = ENDSTATE2;
	}

		break;
	}

	if (rightSignal)
	{
		std::cout << "Statewechsel" << std::endl;
		_state = state;
		std::cout << "Aktueller Zustand:" << Enums::stateToString(_state);
		action();
	}

	return rightSignal;
}

/**
 * führt die funktion des Zustandes aus.
 */
void PukFSM1::action()
{

	switch (_state)
	{
	case IN_ENTRY:
		{
		//TIMING
//		Timerinfo info(_timervalues.entry2hightLong, _id, _id);
//		_2disp.enqueue(Message(Signal::STARTTIMER, Component::TIMER_CONTROL, Component::MAIN, info));

	}

		break;

	case IN_HEIGHT:
		{
		//TIMING
//		Timerinfo info(_timervalues.hight2gate, _id, _id);
//		_2disp.enqueue(Message(Signal::STOPTIMER, Component::TIMER_CONTROL, Component::MAIN, info));
//		_2disp.enqueue(Message(Signal::STARTTIMER, Component::TIMER_CONTROL, Component::MAIN, info));

//ACTION
	}

		break;

	case SLIDING:
		{
		//TIMING
//		Timerinfo info(_timervalues.gate2slide, _id, _id);
//		_2disp.enqueue(Message(Signal::STOPTIMER, Component::TIMER_CONTROL, Component::MAIN, info));
//		_2disp.enqueue(Message(Signal::STARTTIMER, Component::TIMER_CONTROL, Component::MAIN, info));

//ACTION
		_2disp.enqueue(Message(Signal::YELLOW_LIGHT_BLINK_SLOW, Component::LAMP, Component::MAIN));

	}

		break;
	case IN_GATE:
		{
		//TIMING
//		Timerinfo info(_timervalues.gate2exit, _id, _id);
//		_2disp.enqueue(Message(Signal::STOPTIMER, Component::TIMER_CONTROL, Component::MAIN, info));
//		_2disp.enqueue(Message(Signal::STARTTIMER, Component::TIMER_CONTROL, Component::MAIN, info));

//ACTION
		_2disp.enqueue(Message(Signal::GATE_OPEN, Component::GATE, Component::MAIN));
		Timerinfo info2(1000, TIMERIDFORGATE, TIMERIDFORGATE);
		_2disp.enqueue(Message(Signal::STARTTIMER, Component::TIMER_CONTROL, Component::MAIN, info2));
		_2disp.enqueue(Message(Signal::PUKDATA, Component::OTHER_MACHINE, Component::MAIN, _id, _pukType));
	}

		break;
	case WAIT4BAND2:
		{
		//TIMING
//		Timerinfo info(10000, _id, _id);
//		_2disp.enqueue(Message(Signal::STOPTIMER, Component::TIMER_CONTROL, Component::MAIN, info));

//ACTION
		_2disp.enqueue(Message(Signal::MOTOR_STOP, Component::MOTOR, Component::MAIN));
		_2disp.enqueue(Message(Signal::PAUSEALLTIMERS, Component::TIMER_CONTROL, Component::MAIN));
	}

		break;
	case PASSING:
		{
		//TIMING
//		Timerinfo info(_timervalues.bandOne2bandTwo, _id, _id);
//		_2disp.enqueue(Message(Signal::STOPTIMER, Component::TIMER_CONTROL, Component::MAIN, info));
//		_2disp.enqueue(Message(Signal::STARTTIMER, Component::TIMER_CONTROL, Component::MAIN, info));

//ACTION
		_2disp.enqueue(Message(Signal::CONTINUEALLTIMERS, Component::TIMER_CONTROL, Component::MAIN));
		_2disp.enqueue(Message(Signal::MOTOR_RIGHT, Component::OTHER_MACHINE, Component::MAIN));
	}

		break;

	case SLIDINGDONE:
		{
		//TIMING
//		Timerinfo info(10000, _id, _id);
//		_2disp.enqueue(Message(Signal::STOPTIMER, Component::TIMER_CONTROL, Component::MAIN, info));

		//TIMER FOR SLIDES//PRÜFEN OB VOLL
		Timerinfo info2(3000, TIMERFORSLIDE1, TIMERFORSLIDE1);
		_2disp.enqueue(Message(Signal::STARTTIMER, Component::TIMER_CONTROL, Component::MAIN, info2));

//RUTSCHENTIMING

//ACTION
		_2disp.enqueue(Message(Signal::YELLOW_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
	}
		break;

	case ENDSTATE2:
		{
		//TIMING
//		Timerinfo info(10000, _id, _id);
//		_2disp.enqueue(Message(Signal::STOPTIMER, Component::TIMER_CONTROL, Component::MAIN, info));

	}
		break;
	default:
		LOG_WRITE("UNKNOWN STATE!");
	}

}

PUKType PukFSM1::getPukType()
{
	return _pukType;
}

State PukFSM1::getState()
{
	return _state;
}

int PukFSM1::getID()
{
	return _id;
}
