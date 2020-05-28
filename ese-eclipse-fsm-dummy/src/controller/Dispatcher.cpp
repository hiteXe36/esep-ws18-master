#include "Dispatcher.h"

#include "../servicetools/enums.h"
//#include "../servicetools/Logger.h"

#include <iostream>

Dispatcher::Dispatcher(
        Channel& disp2gate,
        Channel& disp2motor,
		/*Channel& disp2lamp,*/
        /*Channel& disp2emrec,*/
        /*Channel& disp2sens,*/
		/*Channel& disp2hight,*/
        /*Channel& disp2Remote,*/
        Channel& disp2main,
        Channel& disp2serial,
        Channel& disp2puktc,
        Channel& disp2timer,
		Channel& toDisp/*,
		 EXECMODE mode*/)
:
		_2gate(disp2gate),
		_2motor(disp2motor),
		/*_2lamp(disp2lamp),*/
		/*_2emrec(disp2emrec),*/
		/*_2sens(disp2sens),*/
		/*_2hight(disp2hight),*/
		/*_2remoteControl(disp2Remote),*/
		_2main(disp2main),
		_2serial(disp2serial),
		_2puktc(disp2puktc),
		_2timer(disp2timer),
		_2me(toDisp)/*,
		_currentmode(mode)*/
{
    //	if (mode == EXECMODE::REPLAY) {
    //		LOG_WRITE("Running REPLAY-Mode...");
    //		_readstream.open("recording.txt");
    //	}
}

void Dispatcher::operator ()() {
//	if (_currentmode == EXECMODE::MAIN) {
//		LOG_WRITE("Running EXECMODE...");
//		bool rcConnected = false;
		while (true)
		{
			Message rcvMsg = _2me.dequeue();

//			LOG_WRITE("Dispatcher hat Message bekommen");

//			_2emrec.enqueue(rcvMsg);
//			if (rcConnected) {
//				strncpy(rcvMsg.payload, "LOG", sizeof("LOG"));
//				_2remoteControl.enqueue(rcvMsg);
//			}

			switch (rcvMsg.dest)
			{
			case Component::MAIN:
//				LOG_WRITE("Message ist für Main. Dispatcher leitet weiter...");
				_2main.enqueue(rcvMsg);
				break;
//			case Component::LAMP:
////				LOG_WRITE("Message ist für Lamp. Dispatcher leitet weiter...");
//				_2lamp.enqueue(rcvMsg);
//				break;
			case Component::MOTOR:
//				LOG_WRITE("Message ist für Motor. Dispatcher leitet weiter...");
				_2motor.enqueue(rcvMsg);
				break;
			case Component::GATE:
//				LOG_WRITE("Message ist für Gate. Dispatcher leitet weiter...");
				_2gate.enqueue(rcvMsg);
				break;
			case Component::OTHER_MACHINE:
			    _2serial.enqueue(rcvMsg);
			    break;
			case Component::HIGHT:
			    _2puktc.enqueue(rcvMsg);
			    break;
            case Component::TIMER:
                _2timer.enqueue(rcvMsg);
                break;
//			case Component::HIGHT:
////				LOG_WRITE("Message ist für Hight. Dispatcher leitet weiter...");
//				_2hight.enqueue(rcvMsg);
//				break;
//			case Component::DISPATCHER:
////				LOG_WRITE("Message ist für Dispatcher. Dispatcher behalten");
//				if (rcvMsg.signal == Signal::REMOTE_CONNECTED) {
//					rcConnected = true;
//				}
//				else if (rcvMsg.signal == Signal::REMOTE_NOT_CONNECTED) {
//					rcConnected = false;
//				}
//				break;
			default:
//				LOG_WRITE("Wrong Signal detected");
				break;
			}
		}
//	} else {
////		LOG_WRITE("Reconstructing the Msg-List");
//		std::vector<Message> msgList;
//		msgList = readfromfile();
////		LOG_WRITE("DONE reconstructing the messages");
//		//iterator für die msgListe
//		//list<Message>::iterator it;
////		LOG_WRITE("Starting to send the reconstructed messages");
//		int listSize = msgList.size();
//		int donedeed = 0;
//		for (int i = 0; i < listSize; i++) {
//			Message actualMessage = msgList.at(i);
//			std::cout << "DONEDEED " << donedeed << std::endl;
//			std::cout << "TIMESTAMP " << actualMessage.timestamp << std::endl;
//			std::cout << "ACTUAL " << actualMessage.timestamp - donedeed << std::endl;
//			std::this_thread::sleep_for(std::chrono::milliseconds(actualMessage.timestamp - donedeed));
//
//			donedeed += actualMessage.timestamp - donedeed;
//			switch (actualMessage.dest) {
//			case Component::MAIN:
//				LOG_WRITE(
//						"REPLAYMODE: Message ist für Main. Dispatcher leitet weiter...");
//				_2main.enqueue(actualMessage);
//				break;
//			case Component::LAMP:
//				LOG_WRITE(
//						"REPLAYMODE: Message ist für Lamp. Dispatcher leitet weiter...");
//				_2lamp.enqueue(actualMessage);
//				break;
//			case Component::MOTOR:
//				LOG_WRITE(
//						"REPLAYMODE: Message ist für Motor. Dispatcher leitet weiter...");
//				_2motor.enqueue(actualMessage);
//				break;
//			case Component::GATE:
//				LOG_WRITE(
//						"REPLAYMODE: Message ist für Gate. Dispatcher leitet weiter...");
//				_2gate.enqueue(actualMessage);
//				break;
//			default:
//				LOG_WRITE("REPLAYMODE: Wrong Signal detected");
//				break;
//			}
//
//		}
//		LOG_WRITE("End of function 'readFromFile'");
//	}

}

Dispatcher::~Dispatcher() {

}

//std::vector<Message> Dispatcher::readfromfile() {
//	LOG_WRITE("READING FROM FILE");
//	std::string output;
//	std::vector<Message> msgList;
//
//	while (true) {
//		std::string read;
//		bool reachedend = false;
//
//		do
//
//		{
//			std::getline(_readstream, read);
//			if (read.compare("ENDOFRECORDFILE") == 0) {
//				//output << "END OF RECORD FILE" << std::endl;
//				LOG_WRITE("END OF FILE");
//				reachedend = true;
//				break;
//				//return output;
//			}
//		} while (read.compare("NEW MESSAGE AT SEC") != 0);
//
//		if (reachedend) {
//			LOG_WRITE("REACHED the END");
//			break;
//		}
//
//		Component src;
//		Component dest;
//		Signal sig;
//
//		std::getline(_readstream, read);
//		//Mach irgendwas mit der Timestamp...
//		int timestamp = std::stoi(read);
//
//		std::getline(_readstream, read);
//
//		//helpfunctioncall
//		sig = convertStringToSignal(read);
//		std::getline(_readstream, read);
//		dest = convertStringToComponent(read);
//		std::getline(_readstream, read);
//		src = convertStringToComponent(read);
//		//output << read << std::endl;
//		Message newmsg(sig, dest, src, timestamp);
//		msgList.push_back(newmsg);
//	}
//
//	return msgList;
//}
//
///*
// * Help Functions
// */
//Signal Dispatcher::convertStringToSignal(std::string ss) {
//	LOG_WRITE("Converting String to Signal");
//
//	if (ss.compare("GREEN_LIGHT_ON") == 0) {
//		return Signal::GREEN_LIGHT_ON;
//	} else if (ss.compare("GREEN_LIGHT_OFF") == 0) {
//		return Signal::GREEN_LIGHT_OFF;
//	} else if (ss.compare("GREEN_LIGHT_BLINK_SLOW") == 0) {
//		return Signal::GREEN_LIGHT_BLINK_SLOW;
//	} else if (ss.compare("GREEN_LIGHT_BLINK_FAST") == 0) {
//		return Signal::GREEN_LIGHT_BLINK_FAST;
//	}
//
//	else if (ss.compare("YELLOW_LIGHT_ON") == 0) {
//		return Signal::YELLOW_LIGHT_ON;
//	} else if (ss.compare("YELLOW_LIGHT_OFF") == 0) {
//		return Signal::YELLOW_LIGHT_OFF;
//	} else if (ss.compare("YELLOW_LIGHT_BLINK_SLOW") == 0) {
//		return Signal::YELLOW_LIGHT_BLINK_SLOW;
//	} else if (ss.compare("YELLOW_LIGHT_BLINK_FAST") == 0) {
//		return Signal::YELLOW_LIGHT_BLINK_FAST;
//	}
//
//	else if (ss.compare("RED_LIGHT_ON") == 0) {
//		return Signal::RED_LIGHT_ON;
//	} else if (ss.compare("RED_LIGHT_OFF") == 0) {
//		return Signal::RED_LIGHT_OFF;
//	} else if (ss.compare("RED_LIGHT_BLINK_SLOW") == 0) {
//		return Signal::RED_LIGHT_BLINK_SLOW;
//	} else if (ss.compare("RED_LIGHT_BLINK_FAST") == 0) {
//		return Signal::RED_LIGHT_BLINK_FAST;
//	}
//
//	else if (ss.compare("MOTOR_ON") == 0) {
//		return Signal::MOTOR_ON;
//	} else if (ss.compare("MOTOR_OFF") == 0) {
//		return Signal::MOTOR_OFF;
//	} else if (ss.compare("MOTOR_SLOW") == 0) {
//		return Signal::MOTOR_SLOW;
//	} else if (ss.compare("MOTOR_FAST") == 0) {
//		return Signal::MOTOR_FAST;
//	} else if (ss.compare("MOTOR_LEFT") == 0) {
//		return Signal::MOTOR_LEFT;
//	} else if (ss.compare("MOTOR_RIGHT") == 0) {
//		return Signal::MOTOR_RIGHT;
//	} else if (ss.compare("MOTOR_STOP") == 0) {
//		return Signal::MOTOR_STOP;
//	}
//
//	else if (ss.compare("GATE_OPEN") == 0) {
//		return Signal::GATE_OPEN;
//	} else if (ss.compare("GATE_CLOSE") == 0) {
//		return Signal::GATE_CLOSE;
//	}
//
//	else {
//		LOG_WRITE("The signal " + ss + " this string specified does not exist");
//		return Signal::NULL_SIGNAL;
//	}
//}
//
///**
// * Second helpfunction
// */
//
//Component Dispatcher::convertStringToComponent(std::string ss) {
//	LOG_WRITE("Converting String to Component");
//
//	if (ss.compare("MAIN") == 0) {
//		return Component::MAIN;
//	} else if (ss.compare("HAL") == 0) {
//		return Component::HAL;
//	} else if (ss.compare("SERIAL") == 0) {
//		return Component::SERIAL;
//	} else if (ss.compare("REMOTE") == 0) {
//		return Component::REMOTE;
//	} else if (ss.compare("DISPATCHER") == 0) {
//		return Component::DISPATCHER;
//	} else if (ss.compare("LAMP") == 0) {
//		return Component::LAMP;
//	} else if (ss.compare("MOTOR") == 0) {
//		return Component::MOTOR;
//	} else if (ss.compare("GATE") == 0) {
//		return Component::GATE;
//	} else if (ss.compare("EMBEDDED_RECORDER") == 0) {
//		return Component::EMBEDDED_RECORDER;
//	}
//
//	else {
//		LOG_WRITE(
//				"The component " + ss
//						+ " this string specified does not exist");
//		return Component::NULL_COMPONENT;
//	}
//}
//
