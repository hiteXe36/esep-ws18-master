#include "EmbeddedRecorder.h"

#ifdef __DUMMY_TEST
#include "../hal/DummyHAL.h"
#else
#include "../hal/HAL.h"
#endif
#include "../servicetools/Logger.h"
#include "../servicetools/enums.h"


#include <string>


EmbeddedRecorder::EmbeddedRecorder(Channel& emrec2disp, Channel& disp2emrec)
		: _2disp(emrec2disp), _2me(disp2emrec)//, _timeoffset(time(NULL))
{
	_recordfile.open("recording.txt", std::ofstream::out | std::ofstream::app);
	_recordfile << "RECORDFILE. Build: " << __DATE__ << ", " << __TIME__ << std::endl;
	_timeoffset = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch()).count();
}

void EmbeddedRecorder::operator()()
{
	while (true)
	{
		Message rcvMsg = _2me.dequeue();
		int now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		_recordfile << "NEW MESSAGE AT SEC" << std::endl << now - _timeoffset
				<< std::endl;
		_recordfile << Enums::signalToString(rcvMsg.signal) << std::endl;
		_recordfile << Enums::componentToString(rcvMsg.dest)
				<< std::endl;
		_recordfile << Enums::componentToString(rcvMsg.src) << std::endl;
		_recordfile << "END OF MESSAGE" << std::endl;
	}

}

EmbeddedRecorder::~EmbeddedRecorder()
{

}

//std::string EmbeddedRecorder::componentToString(Component c)
//{
//	switch (c)
//	{
//		case Component::NULL_COMPONENT:
//			return "NULL_COMPONENT";
//			break;
//		case Component::MAIN:
//			return "MAIN";
//			break;
//		case Component::HAL:
//			return "HAL";
//			break;
//		case Component::SERIAL:
//			return "SERIAL";
//			break;
//		case Component::REMOTE:
//			return "REMOTE";
//			break;
//		case Component::DISPATCHER:
//			return "DISPATCHER";
//			break;
//		case Component::LAMP:
//			return "LAMP";
//			break;
//		case Component::MOTOR:
//			return "MOTOR";
//			break;
//		case Component::GATE:
//			return "GATE";
//			break;
//		case Component::EMBEDDED_RECORDER:
//			return "EMBEDDED_RECORDER";
//			break;
//		default:
//			return "UNKNOWN COMPONENT";
//	}
//}

///**
// * Function to convert a Signal-Object into a std::string
// */
//std::string EmbeddedRecorder::signalToString(Signal s)
//{
//	switch (s)
//	{
//		case Signal::GREEN_LIGHT_ON:
//			return "GREEN_LIGHT_ON";
//			break;
//		case Signal::GREEN_LIGHT_OFF:
//			return "GREEN_LIGHT_OFF";
//			break;
//		case Signal::GREEN_LIGHT_BLINK_SLOW:
//			return "GREEN_LIGHT_BLINK_SLOW";
//			break;
//		case Signal::GREEN_LIGHT_BLINK_FAST:
//			return "GREEN_LIGHT_BLINK_FAST";
//			break;
//		case Signal::GREEN_LIGHT_BLINK_OFF:
//			return "GREEN_LIGHT_BLINK_OFF";
//			break;
//		case Signal::YELLOW_LIGHT_ON:
//			return "YELLOW_LIGHT_ON";
//			break;
//		case Signal::YELLOW_LIGHT_OFF:
//			return "YELLOW_LIGHT_OFF";
//			break;
//		case Signal::YELLOW_LIGHT_BLINK_SLOW:
//			return "YELLOW_LIGHT_SLOW";
//			break;
//		case Signal::YELLOW_LIGHT_BLINK_FAST:
//			return "YELLOW_LIGHT_BLINK_FAST";
//			break;
//		case Signal::YELLOW_LIGHT_BLINK_OFF:
//			return "YELLOW_LIGHT_BLINK_OFF";
//			break;
//		case Signal::RED_LIGHT_ON:
//			return "RED_LIGHT_ON";
//			break;
//		case Signal::RED_LIGHT_OFF:
//			return "RED_LIGHT_OFF";
//			break;
//		case Signal::RED_LIGHT_BLINK_SLOW:
//			return "RED_LIGHT_BLINK_SLOW";
//			break;
//		case Signal::RED_LIGHT_BLINK_FAST:
//			return "RED_LIGHT_BLINK_FAST";
//			break;
//		case Signal::RED_LIGHT_BLINK_OFF:
//			return "RED_LIGHT_BLINK_OFF";
//			break;
//		case Signal::MOTOR_ON:
//			return "MOTOR_ON";
//			break;
//		case Signal::MOTOR_OFF:
//			return "MOTOR_OFF";
//			break;
//		case Signal::MOTOR_SLOW:
//			return "MOTOR_SLOW";
//			break;
//		case Signal::MOTOR_FAST:
//			return "MOTOR_FAST";
//			break;
//		case Signal::MOTOR_LEFT:
//			return "MOTOR_LEFT";
//			break;
//		case Signal::MOTOR_RIGHT:
//			return "MOTOR_RIGHT";
//			break;
//		case Signal::MOTOR_STOP:
//			return "MOTOR_STOP";
//			break;
//		case Signal::GATE_OPEN:
//			return "GATE_OPEN";
//			break;
//		case Signal::GATE_CLOSE:
//			return "GATE_CLOSE";
//			break;
//		case Signal::NULL_SIGNAL:
//			return "NULL_SIGNAL";
//			break;
//		default:
//			return "UNKNOWN SIGNAL";
//			break;
//	}
//}
