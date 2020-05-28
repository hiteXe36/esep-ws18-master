#include "enums.h"

std::string Enums::pukTypeToString(PUKType pukType) {
	switch (pukType) {
	case PUKType::TYPE1:
		return "Flat Puck";
	case PUKType::TYPE2_1:
		return "With Drilling Metal";
	case PUKType::TYPE2_2:
		return "High Puck";
	case PUKType::TYPE3_1:
		return "With Drilling No Metal";
	case PUKType::TYPE3_2:
		return "High Puck";
	case PUKType::TYPE4_1:
		return "Coded 1";
	case PUKType::TYPE4_2:
		return "Coded 2";
	case PUKType::TYPE4_4:
		return "Coded 4";
	case PUKType::TYPE4_5:
		return "Coded 5";
	case PUKType::TYPEERROR:
		return "Error Type";
	case PUKType::TYPENULL:
		return "Type Null";
	case PUKType::TYPENODATA:
		return "No Data";

	default:
		return "UnKnoWn PuCk tYpE";

	}
}

std::string Enums::componentToString(Component c) {
	switch (c) {

	case Component::NULL_COMPONENT:
		return "NULL_COMPONENT";
		break;
	case Component::MAIN:
		return "MAIN";
		break;
	case Component::HAL:
		return "HAL";
		break;
	case Component::SERIAL:
		return "SERIAL";
		break;
	case Component::OTHER_MACHINE:
		return "OTHER_MACHINE";
		break;
	case Component::REMOTE:
		return "REMOTE";
		break;
	case Component::LAMP:
		return "LAMP";
		break;
	case Component::MOTOR:
		return "MOTOR";
		break;
	case Component::GATE:
		return "GATE";
		break;
	case Component::EMBEDDED_RECORDER:
		return "EMBEDDED_RECORDER";
		break;
	case Component::HIGHT:
		return "HIGHT";
		break;
	case Component::SENSOR:
		return "SENSOR";
		break;
	case Component::DISPATCHER:
		return "DISPATCHER";
		break;
	case Component::CALIBRATION:
		return "CALIBRATION";
		break;
	case Component::TIMER:
		return "Timer";
		break;
	case Component::TIMER_CONTROL:
			return "Timer Control";
			break;
		//Wenn neue Componente kommen, bitte hier hinzufügen
	default:
		return "Unknown Component";

	}
}

std::string Enums::signalToString(Signal s) {
	switch (s) {
	case Signal::GREEN_LIGHT_ON:
		return "GREEN_LIGHT_ON";
		break;
	case Signal::GREEN_LIGHT_OFF:
		return "GREEN_LIGHT_OFF";
		break;
	case Signal::GREEN_LIGHT_BLINK_SLOW:
		return "GREEN_LIGHT_BLINK_SLOW";
		break;
	case Signal::GREEN_LIGHT_BLINK_FAST:
		return "GREEN_LIGHT_BLINK_FAST";
		break;
	case Signal::GREEN_LIGHT_BLINK_OFF:
		return "GREEN_LIGHT_BLINK_OFF";
		break;
	case Signal::YELLOW_LIGHT_ON:
		return "YELLOW_LIGHT_ON";
		break;
	case Signal::YELLOW_LIGHT_OFF:
		return "YELLOW_LIGHT_OFF";
		break;
	case Signal::YELLOW_LIGHT_BLINK_SLOW:
		return "YELLOW_LIGHT_BLINK_SLOW";
		break;
	case Signal::YELLOW_LIGHT_BLINK_FAST:
		return "YELLOW_LIGHT_BLINK_FAST";
		break;
	case Signal::YELLOW_LIGHT_BLINK_OFF:
		return "YELLOW_LIGHT_BLINK_OFF";
		break;
	case Signal::RED_LIGHT_ON:
		return "RED_LIGHT_ON";
		break;
	case Signal::RED_LIGHT_OFF:
		return "RED_LIGHT_OFF";
		break;
	case Signal::RED_LIGHT_BLINK_SLOW:
		return "RED_LIGHT_BLINK_SLOW";
		break;
	case Signal::RED_LIGHT_BLINK_FAST:
		return "RED_LIGHT_BLINK_FAST";
		break;
	case Signal::RED_LIGHT_BLINK_OFF:
		return "RED_LIGHT_BLINK_OFF";
		break;
	case Signal::MOTOR_ON:
		return "MOTOR_ON";
		break;
	case Signal::MOTOR_OFF:
		return "MOTOR_OFF";
		break;
	case Signal::MOTOR_SLOW:
		return "MOTOR_SLOW";
		break;
	case Signal::MOTOR_FAST:
		return "MOTOR_FAST";
		break;
	case Signal::MOTOR_LEFT:
		return "MOTOR_LEFT";
		break;
	case Signal::MOTOR_RIGHT:
		return "MOTOR_RIGHT";
		break;
	case Signal::MOTOR_STOP:
		return "MOTOR_STOP";
		break;
	case Signal::GATE_OPEN:
		return "GATE_OPEN";
		break;
	case Signal::GATE_CLOSE:
		return "GATE_CLOSE";
		break;
	case Signal::ACKNOWLEDGED:
		return "ACKNOWLEDGED";
		break;
	case Signal::NULL_SIGNAL:
		return "NULL_SIGNAL";
		break;
	case Signal::LB_ENTRY_ENTERED:
		return "LB_ENTRY_ENTERED";
		break;
	case Signal::LB_ENTRY_LEFT:
		return "LB_ENTRY_LEFT";
		break;
	case Signal::LB_HIGHT_ENTERED:
		return "LB_HIGHT_ENTERED";
		break;
	case Signal::LB_HIGHT_LEFT:
		return "LB_HIGHT_LEFT";
		break;
	case Signal::LB_METAL_ENTERED:
		return "LB_METAL_ENTERED";
		break;
	case Signal::LB_METAL_LEFT:
		return "LB_METAL_LEFT";
		break;
	case Signal::LB_SLIDE_ENTERED:
		return "LB_SLIDE_ENTERED";
		break;
	case Signal::LB_SLIDE_LEFT:
		return "LB_SLIDE_LEFT";
		break;
	case Signal::LB_EXIT_ENTERED:
		return "LB_EXIT_ENTERED";
		break;
	case Signal::LB_EXIT_LEFT:
		return "LB_EXIT_LEFT";
		break;
	case Signal::HIGHT_OK:
		return "HIGHT_OK";
		break;
	case Signal::HIGHT_NOT_OK:
		return "HIGHT_NOT_OK";
		break;
	case Signal::IS_METAL:
		return "IS_METAL";
		break;
	case Signal::NOT_METAL:
		return "NOT_METAL";
		break;
	case Signal::BTN_START_PRESSED:
		return "BTN_START_PRESSED";
		break;
	case Signal::BTN_START_RELEASED:
		return "BTN_START_RELEASED";
		break;
	case Signal::BTN_STOP_PRESSED:
		return "BTN_STOP_PRESSED";
		break;
	case Signal::BTN_STOP_RELEASED:
		return "BTN_STOP_RELEASED";
		break;
	case Signal::BTN_RESET_PRESSED:
		return "BTN_RESET_PRESSED";
		break;
	case Signal::BTN_RESET_RELEASED:
		return "BTN_RESET_RELEASED";
		break;
	case Signal::ESTOP_PRESSED:
		return "ESTOP_PRESSED";
		break;
	case Signal::ESTOP_RELEASED:
		return "ESTOP_RELEASED";
		break;
	case Signal::START_HIGHT_MEASUREMENT:
		return "START_HIGHT_MEASUREMENT";
		break;
	case Signal::WITH_DRILLING_PUCK:
		return "WITH_DRILLING_PUCK";
		break;
	case Signal::FLAT_PUCK:
		return "FLAT_PUCK";
		break;
	case Signal::HIGH_PUCK:
		return "HIGH_PUCK";
		break;
	case Signal::CODE1_PUCK:
		return "CODE1_PUCK";
		break;
	case Signal::CODE2_PUCK:
		return "CODE2_PUCK";
		break;
	case Signal::CODE4_PUCK:
		return "CODE4_PUCK";
		break;
	case Signal::CODE5_PUCK:
		return "CODE5_PUCK";
		break;
	case Signal::NOT_DETERMINABLE_PUCK:
		return "NOT_DETERMINABLE_PUCK";
		break;
	case Signal::NOT_DETERMINABLE:
		return "NOT_DETERMINABLE";
		break;
	case Signal::GETPUKDATA:
		return "GETPUKDATA";
		break;
	case Signal::PUKDATA:
		return "PUKDATA";
		break;
	case Signal::STARTCALIBRATION:
		return "STARTCALIBRATION";
		break;
	case Signal::CALIBRATIONABORTED:
		return "CALIBRATIONABORTED";
		break;
	case Signal::CALIBRATIONVALUES:
		return "CALIBRATIONVALUES";
		break;
	case Signal::STARTTIMER:
		return "STARTTIMER";
		break;
	case Signal::STOPALLTIMERS:
		return "STOPALLTIMERS";
		break;
	case Signal::STOPTIMER:
		return "STOPTIMER";
		break;
	case Signal::PAUSEALLTIMERS:
		return "PAUSEALLTIMERS";
		break;
	case Signal::CONTINUEALLTIMERS:
		return "CONTINUEALLTIMERS";
		break;
	case Signal::MACHINE2BUSY:
		return "MACHINE2BUSY";
		break;
	case Signal::MACHINE2IDLE:
		return "MACHINE2IDLE";
		break;
	case Signal::SLIDE2FULL:
		return "SLIDE2FULL";
		break;
	case Signal::SLIDE2EMPTY:
		return "SLIDE2EMPTY";
		break;
	case Signal::REIHUNG:
		return "REIHUNG";
		break;
	case Signal::CALIBRATIONSIGNAL_REQ:
		return "CALIBRATIONSIGNAL_REQ";
		break;
	case Signal::CALIBRATIONSIGNAL_ACK:
		return "CALIBRATIONSIGNAL_ACK";
		break;
	case Signal::TIMED_OUT:
		return "TIMED_OUT";
		break;
	case Signal::REMOTE_CONNECTED:
		return "REMOTE_CONNECTED";
		break;
	case Signal::REMOTE_NOT_CONNECTED:
		return "REMOTE_NOT_CONNECTED";
		break;
	case Signal::WRONGPUK:
		return "WRONGPUK";
		break;
	case Signal::WRONGSIGNAL:
		return "WRONGSIGNAL";
		break;
	case Signal::PASSINGCONFIRMED:
		return "PASSINGCONFIRMED";
		break;
	case Signal::STOPCALIBRATION:
		return "STOPCALIBRATION";
		break;
	case Signal::CALIBRATIONSIGNAL_FIN:
		return "CALIBRATIONSIGNAL_FIN";
		break;

		//Wenn neue Signale kommen, bitte hier hinzufügen
	default:
		return "Unknown Signal";
	}
}

/**
 * Converts a passed string into a Component-Object
 */
Component Enums::convertStringToComponent(std::string ss) {
	//LOG_WRITE("Converting String to Component");

	if (ss.compare("MAIN") == 0) {
		return Component::MAIN;
	} else if (ss.compare("HAL") == 0) {
		return Component::HAL;
	} else if (ss.compare("SERIAL") == 0) {
		return Component::SERIAL;
	} else if (ss.compare("REMOTE") == 0) {
		return Component::REMOTE;
	} else if (ss.compare("DISPATCHER") == 0) {
		return Component::DISPATCHER;
	} else if (ss.compare("LAMP") == 0) {
		return Component::LAMP;
	} else if (ss.compare("MOTOR") == 0) {
		return Component::MOTOR;
	} else if (ss.compare("GATE") == 0) {
		return Component::GATE;
	} else if (ss.compare("EMBEDDED_RECORDER") == 0) {
		return Component::EMBEDDED_RECORDER;
	} else if (ss.compare("CALIBRATION") == 0) {
		return Component::CALIBRATION;
	} else if (ss.compare("OTHER_MACHINE") == 0) {
		return Component::OTHER_MACHINE;
	} else if (ss.compare("HIGHT") == 0) {
		return Component::HIGHT;
	} else if (ss.compare("TIMER") == 0) {
		return Component::TIMER;
	}

	else {
		LOG_WRITE(
				"The component " + ss
						+ " this string specified does not exist");
		return Component::NULL_COMPONENT;
	}
}

std::string Enums::stateToString(State s) {

	switch (s) {
	case State::IN_ENTRY:
		return "IN_ENTRY";
		break;
	case State::IN_HEIGHT:
		return "IN_HEIGHT";
		break;
	case State::SLIDING:
		return "SLIDING";
		break;
	case State::IN_GATE:
		return "IN_GATE";
		break;
	case State::WAIT4BAND2:
		return "WAIT4BAND2";
		break;
	case State::READY:
		return "READY";
		break;
	case State::PASSING:
		return "PASSING";
		break;
	case State::ENDSTATE:
		return "ENDSTATE";
		break;
	case State::ENDSTATE2:
		return "ENDSTATE2";
		break;
	case State::IDLE:
		return "IDLE";
		break;
	case State::SLIDINGDONE:
		return "SLIDINGDONE";
		break;
	case State::ANSTEHENDUNQUITIERT:
		return "ANSTEHENDUNQUITIERT";
		break;
	case State::ANSTEHENDQUITIERT:
		return "ANSTEHENDQUITIERT";
		break;
	case State::AWAKEN:
		return "AWAKEN";
		break;
	default:
		return "Unknown State";

	}

}

/**
 * Converts a passed string into a Signal-Object
 */
Signal Enums::convertStringToSignal(std::string ss) {
	//LOG_WRITE("Converting String to Signal");

	if (ss.compare("GREEN_LIGHT_ON") == 0) {
		return Signal::GREEN_LIGHT_ON;
	} else if (ss.compare("GREEN_LIGHT_OFF") == 0) {
		return Signal::GREEN_LIGHT_OFF;
	} else if (ss.compare("GREEN_LIGHT_BLINK_SLOW") == 0) {
		return Signal::GREEN_LIGHT_BLINK_SLOW;
	} else if (ss.compare("GREEN_LIGHT_BLINK_FAST") == 0) {
		return Signal::GREEN_LIGHT_BLINK_FAST;
	} else if (ss.compare("GREEN_LIGHT_BLINK_OFF") == 0) {
		return Signal::GREEN_LIGHT_BLINK_OFF;
	}

	else if (ss.compare("YELLOW_LIGHT_ON") == 0) {
		return Signal::YELLOW_LIGHT_ON;
	} else if (ss.compare("YELLOW_LIGHT_OFF") == 0) {
		return Signal::YELLOW_LIGHT_OFF;
	} else if (ss.compare("YELLOW_LIGHT_BLINK_SLOW") == 0) {
		return Signal::YELLOW_LIGHT_BLINK_SLOW;
	} else if (ss.compare("YELLOW_LIGHT_BLINK_FAST") == 0) {
		return Signal::YELLOW_LIGHT_BLINK_FAST;
	} else if (ss.compare("YELLOW_LIGHT_BLINK_OFF") == 0) {
		return Signal::YELLOW_LIGHT_BLINK_OFF;
	}

	else if (ss.compare("RED_LIGHT_ON") == 0) {
		return Signal::RED_LIGHT_ON;
	} else if (ss.compare("RED_LIGHT_OFF") == 0) {
		return Signal::RED_LIGHT_OFF;
	} else if (ss.compare("RED_LIGHT_BLINK_SLOW") == 0) {
		return Signal::RED_LIGHT_BLINK_SLOW;
	} else if (ss.compare("RED_LIGHT_BLINK_FAST") == 0) {
		return Signal::RED_LIGHT_BLINK_FAST;
	} else if (ss.compare("RED_LIGHT_BLINK_OFF") == 0) {
		return Signal::RED_LIGHT_BLINK_OFF;
	}

	else if (ss.compare("MOTOR_ON") == 0) {
		return Signal::MOTOR_ON;
	} else if (ss.compare("MOTOR_OFF") == 0) {
		return Signal::MOTOR_OFF;
	} else if (ss.compare("MOTOR_SLOW") == 0) {
		return Signal::MOTOR_SLOW;
	} else if (ss.compare("MOTOR_FAST") == 0) {
		return Signal::MOTOR_FAST;
	} else if (ss.compare("MOTOR_LEFT") == 0) {
		return Signal::MOTOR_LEFT;
	} else if (ss.compare("MOTOR_RIGHT") == 0) {
		return Signal::MOTOR_RIGHT;
	} else if (ss.compare("MOTOR_STOP") == 0) {
		return Signal::MOTOR_STOP;
	}

	else if (ss.compare("GATE_OPEN") == 0) {
		return Signal::GATE_OPEN;
	} else if (ss.compare("GATE_CLOSE") == 0) {
		return Signal::GATE_CLOSE;
	}

	else {
		//LOG_WRITE("The signal " + ss + " this string specified does not exist");
		return Signal::NULL_SIGNAL;
	}
}

