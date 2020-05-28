#include "../servicetools/Message.h"
#include "../servicetools/enums.h"
#include "../servicetools/Channel.h"
#include "../servicetools/Logger.h"
#include "../remoteControl/RemoteControl.h"
#include <memory>
#include <thread>
#include <iostream>
#include <string>


class RemoteTest {

public:
	RemoteTest() {
		LOG_WRITE("Running test1: Receive Log-Message with the aid of EmbeddedRecorder");
		test1();
	}

	void test1(){
		LOG_WRITE("Starting test1");
		Channel disp2SerialController(10);
		Channel serialController2Disp(3);
		RemoteControl rc(serialController2Disp, disp2SerialController);
	}
	virtual ~RemoteTest() {

	}
};
