#ifndef __DUMMY_TEST
#include "SerialController.h"
#include "../servicetools/Channel.h"
#include "../servicetools/Logger.h"
#include "../serial/SerialReader.h"
#include "../serial/SerialWriter.h"
#include "../serial/Serial.h"
#include <cstddef>
#include <iostream>
#include <thread>

using namespace std;

SerialController::SerialController(Channel& serialC2Disp, Channel& disp2SerialC) :
		disp2SerialController(disp2SerialC), serialController2Disp(
				serialC2Disp), isRunning { true } {

}

void SerialController::operator()() {
	Serial serial("/dev/ser4");
//	Serial serial("/dev/ser1");		// use this if VM

	/* create channel, pass it to reader/writer
	 * and run reader/writer in thread
	 */

	Channel writeChannel(3);
	SerialWriter writer(writeChannel, serial);
	thread writerThread(ref(writer));

	SerialReader reader(disp2SerialController, serial);
	thread readerThread(ref(reader));

	while (isRunning) {
		/* wait for Message from Dispatcher */
		LOG_WRITE("Waiting for message from Main or SerialReader");
		auto msg = std::make_unique < Message > (disp2SerialController.dequeue());
		if (msg->dest == Component::MAIN) {
			/*Send to Main*/
			LOG_WRITE("Got message from SerialReader to Main");
			serialController2Disp.enqueue(*msg);
		} else if (msg->dest == Component::OTHER_MACHINE) {
			/* Send to SerialWriter */
			LOG_WRITE("Got message from Main to OTHER_MACHINE");
			writeChannel.enqueue(*msg);
		} else {
			LOG_WRITE("Unknown Signal!");
			continue;
		}
	}

	writerThread.join();
	readerThread.join();
}

SerialController::~SerialController() {
}
#endif
