/*
 * PukFSM2.cpp
 *
 *  Created on: 11.01.2019
 *      Author: ace044
 */

#include "PukFSM2.h"

PukFSM2::PukFSM2(PUKType pukType, int id, Channel& main2disp, Channel& disp2main, TimerValues timervalues) :
		_pukType(pukType), _id(id), _2disp(main2disp), _2me(disp2main)
{

	_state = AWAKEN;
	_timervalues = timervalues;

}

bool PukFSM2::switchState(Signal s, int reihung)
{
	bool rightSignal = false;
	PUKType pukType;
	State state;

	LOG_WRITE("SIGNAL ERHALTEN: " + Enums::signalToString(s));

	switch (_state)
	{
	case AWAKEN:
	{
		LOG_WRITE("PUK IM ZUSTAND: " + Enums::stateToString(_state));
		rightSignal = (s == Signal::LB_ENTRY_ENTERED);
		state = State::IN_ENTRY;
	}

		break;

	case IN_ENTRY:
	{
		LOG_WRITE("PUK IM ZUSTAND: " + Enums::stateToString(_state));
		rightSignal = (s == Signal::LB_HIGHT_ENTERED);
		state = State::IN_HEIGHT;
	}

		break;

	case IN_HEIGHT:
	{
		LOG_WRITE("PUK IM ZUSTAND: " + Enums::stateToString(_state));
		rightSignal = (s == Signal::LB_METAL_ENTERED);

		if (rightSignal)
		{
			LOG_WRITE("PUKTYP: " + Enums::pukTypeToString(_pukType));
			Message datarequestmsg(Signal::GETPUKDATA, Component::HIGHT, Component::MAIN);
			_2disp.enqueue(datarequestmsg);
			Message intakedata = _2me.dequeue();

			if (intakedata.signal != Signal::PUKDATA)
			{
				LOG_WRITE("PUKTYPEN HOLEN HAT NICHT GEKLAPPT!");
			}
			pukType = intakedata._pukdata;
			LOG_WRITE("VERGLEICHSPUKTYP: " + Enums::pukTypeToString(pukType));

			if (_pukType != pukType)
			{
				_2disp.enqueue(Message(Signal::WRONGPUK, Component::OTHER_MACHINE, Component::MAIN, _id, _pukType));
				state = SLIDING;
			}
			else if (((reihung == 0 || reihung == 2) && _pukType == PUKType::TYPE3_1) || (reihung == 1 && _pukType == PUKType::TYPE2_1))
			{
				state = IN_GATE;
			}
			else
			{
				state = SLIDING;
			}
		}

	}
		break;

	case IN_GATE:
	{
		LOG_WRITE("PUK IM ZUSTAND: " + Enums::stateToString(_state));
		rightSignal = (s == Signal::LB_EXIT_ENTERED);
		state = READY;
	}

		break;
	case READY:
	{
		LOG_WRITE("PUK IM ZUSTAND: " + Enums::stateToString(_state));
		rightSignal = (s == Signal::LB_EXIT_LEFT);
		LOG_WRITE("SIGNAL ERWARTET LB_EXIT_LEFT: " + Enums::signalToString(s));
		state = IDLE;
	}
		break;
	case SLIDING:
	{
		LOG_WRITE("PUK IM ZUSTAND: " + Enums::stateToString(_state));
		rightSignal = (s == Signal::LB_SLIDE_ENTERED);
		state = SLIDINGDONE;
	}
		break;
	case SLIDINGDONE:
	{
		LOG_WRITE("PUK IM ZUSTAND: " + Enums::stateToString(_state));
		rightSignal = (s == Signal::LB_SLIDE_LEFT);
		if (rightSignal)
		{
			//RUTSCHENTIMING
			Timerinfo info(1000, TIMERFORSLIDE2, TIMERFORSLIDE2);
			_2disp.enqueue(Message(Signal::STOPTIMER, Component::TIMER_CONTROL, Component::MAIN, info));
		}
		state = IDLE;
	}
		break;
	}

	if (rightSignal)
	{
		_state = state;
		LOG_WRITE("ZUSTANDWECHSEL: " + Enums::stateToString(_state));
		action();
	}

	return rightSignal;
}

void PukFSM2::action()
{
	switch (_state)
	{
	case IN_ENTRY:
	{

////		TIMING
//		Timerinfo info(_timervalues.entry2hightLong, _id, _id);
//		_2disp.enqueue(Message(Signal::STARTTIMER, Component::TIMER_CONTROL, Component::MAIN, info));

		//ACTION
		_2disp.enqueue(Message(Signal::MACHINE2BUSY, Component::OTHER_MACHINE, Component::MAIN));
		_2disp.enqueue(Message(Signal::PASSINGCONFIRMED, Component::OTHER_MACHINE, Component::MAIN));
	}

		break;
	case IN_HEIGHT:
	{
		//TIMING
//		Timerinfo info(_timervalues.hight2gate, _id, _id);
//		_2disp.enqueue(Message(Signal::STOPTIMER, Component::TIMER_CONTROL, Component::MAIN, info));
//		_2disp.enqueue(Message(Signal::STARTTIMER, Component::TIMER_CONTROL, Component::MAIN, info));
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
		LOG_WRITE("AKTUELLE STATE: " + Enums::stateToString(_state));

	}
		break;

	case SLIDING:
	{
		//TIMING
//		Timerinfo info(_timervalues.gate2slide, _id, _id);
//		_2disp.enqueue(Message(Signal::STOPTIMER, Component::TIMER_CONTROL, Component::MAIN, info));
//		_2disp.enqueue(Message(Signal::STARTTIMER, Component::TIMER_CONTROL, Component::MAIN, info));

		//ACTION
		_2disp.enqueue(Message(Signal::YELLOW_LIGHT_BLINK_FAST, Component::LAMP, Component::MAIN));
	}
		break;
	case SLIDINGDONE:
	{
		//TIMING
//		Timerinfo info(20000, _id, _id);
//		_2disp.enqueue(Message(Signal::STOPTIMER, Component::TIMER_CONTROL, Component::MAIN, info));

		//TIMER FOR SLIDE FULL
		Timerinfo info2(2000, TIMERFORSLIDE2, TIMERFORSLIDE2);
		_2disp.enqueue(Message(Signal::STARTTIMER, Component::TIMER_CONTROL, Component::MAIN, info2));

		//RUTSCHENTIMING
//		Timerinfo info(1000, TIMERFORSLIDE1, TIMERFORSLIDE1);
//		_2disp.enqueue(Message(Signal::STARTTIMER, Component::TIMER_CONTROL, Component::MAIN, info));

//ACTION
		_2disp.enqueue(Message(Signal::MOTOR_STOP, Component::MOTOR, Component::MAIN));
		_2disp.enqueue(Message(Signal::MACHINE2IDLE, Component::OTHER_MACHINE, Component::MAIN));
		_2disp.enqueue(Message(Signal::YELLOW_LIGHT_BLINK_OFF, Component::LAMP, Component::MAIN));
	}
		break;
	case READY:
	{

		//TIMING
//		Timerinfo info(_timervalues.gate2exit, _id, _id);
//		_2disp.enqueue(Message(Signal::STOPTIMER, Component::TIMER_CONTROL, Component::MAIN, info));

		//ACTION
		_2disp.enqueue(Message(Signal::MOTOR_STOP, Component::MOTOR, Component::MAIN));
		//LOG_WRITE("DER PUK HAT DAS ENDE ERREICHT BITTE ENTNEHMEN /n Typ:" + Enums::pukTypeToString(_pukType) + "/n ID:" + _id + "");
	}
		break;

	case IDLE:
	{
		_2disp.enqueue(Message(Signal::MACHINE2IDLE, Component::OTHER_MACHINE, Component::MAIN));
	}
		break;
	}
}

PUKType PukFSM2::getPukType()
{
	return _pukType;

}

State PukFSM2::getState()
{
	return _state;

}

int PukFSM2::getPukID()
{
	return _id;
}

PukFSM2::~PukFSM2()
{
	// TODO Auto-generated destructor stub
}

