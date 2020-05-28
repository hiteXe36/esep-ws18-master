#include "../servicetools/Message.h"
#include "../servicetools/enums.h"
#include "../servicetools/Channel.h"
#include "../servicetools/Logger.h"
#include "../controller/SerialController.h"
#include <memory>
#include <thread>
#include <iostream>
#include <string>

//#define MACHINE_1
#define MACHINE_2

using namespace std;
using namespace std::string_literals;

class SerialTest {
public:
	SerialTest() {
		LOG_WRITE("Running test1: first send to main, then receive from OTHER_MACHINE");
		test1();
	}

	void test1() {
		LOG_WRITE("Starting test1");
		Channel disp2SerialController(10);
		Channel serialController2Disp(3);
		SerialController sc(disp2SerialController, serialController2Disp);
		thread sc_thread(ref(sc));

#ifdef MACHINE_1
		auto msg_1 =
				make_unique < Message
						> (Signal::NULL_SIGNAL, Component::OTHER_MACHINE, Component::MAIN);
		std::string ausgabe_1 = "Sending message from MACHINE_1! Content:";
		ausgabe_1 += "Signal: " + Enums::signalToString(msg_1->signal) + "\n"
				+ "Destination: " + Enums::componentToString(msg_1->dest) + "\n "
				+ "Source: " + Enums::componentToString(msg_1->src);
		LOG_WRITE(ausgabe_1);
		disp2SerialController.enqueue(*msg_1);
#endif

#ifdef MACHINE_2
		LOG_WRITE("Waiting for Message from MACHINE_1");
		auto msg_2 = make_unique < Message > (serialController2Disp.dequeue());
		std::string ausgabe_2;
		ausgabe_2 = "Got message from MACHINE_1! Content:";
		ausgabe_2 += "Signal: " + Enums::signalToString(msg_2->signal) + "\n "
				+ "Destination: " + Enums::componentToString(msg_2->dest) + "\n "
				+ "Source: " + Enums::componentToString(msg_2->src);
		LOG_WRITE(ausgabe_2);

		msg_2->signal = Signal::ACKNOWLEDGED;
		msg_2->dest = Component::OTHER_MACHINE;
		msg_2->src = Component::MAIN;

		ausgabe_2.clear();
		ausgabe_2 = "Sending message to MACHINE_1! Content:";
		ausgabe_2 += "Signal: " + Enums::signalToString(msg_2->signal) + "\n "
				+ "Destination: " + Enums::componentToString(msg_2->dest) + "\n "
				+ "Source: " + Enums::componentToString(msg_2->src);
		disp2SerialController.enqueue(*msg_2);
		LOG_WRITE("Sent message to MACHINE_1");
#endif

		sc_thread.join();
	}

	~SerialTest() {
		// TODO Auto-generated destructor stub
	}

};
