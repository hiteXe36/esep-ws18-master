//#include "../controller/SerialController.h"
#include "../hal/HAL.h"
#include <iostream>

#include "../sensor/ADC.h"
#include "../sensor/GPIO.h"
#include "../sensor/InterruptHandler.h"
#include "../servicetools/enums.h"
#include "../controller/MainControl.h"
#include "../controller/LampControl.h"
#include "../controller/GateControl.h"
#include "../controller/MotorControl.h"
#include "../controller/EmbeddedRecorder.h"
#include "../remoteControl/RemoteControl.h"
#include "../controller/SensorControl.h"
#include "../controller/Dispatcher.h"
#include "../servicetools/Channel.h"
#include "../servicetools/Message.h"
#include "../servicetools/enums.h"
#include "../servicetools/Logger.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
//#include <thread>
//#include <string>
void displayMenu();
// spruhl73.pdf - Page 180 - 183
class InterruptTest {
public:
	InterruptTest() {

		std::cout << "start Interrupts" << std::endl;
		std::string signalInput[] = {
				"LB_ENTRY_ENTERED", //0
				"LB_ENTRY_LEFT", //1
				"LB_HIGHT_ENTERED", //2
				"LB_HIGHT_LEFT",
				"LB_METAL_ENTERED",
				"LB_METAL_LEFT",
				"LB_SLIDE_ENTERED",
				"LB_SLIDE_LEFT",
				"LB_EXIT_ENTERED",
				"LB_EXIT_LEFT",
				"HIGHT_OK",
				"HIGHT_NOT_OK",
				"IS_METAL",
				"NOT_METAL",
				"BTN_START_PRESSED",
				"BTN_START_RELEASED",
				"BTN_STOP_PRESSED",
				"BTN_STOP_RELEASED",
				"BTN_RESET_PRESSED",
				"BTN_RESET_RELEASED",
				"ESTOP_PRESSED",
				"ESTOP_RELEASED",
				"NOT_DETERMINABLE"
		};
		Signal signal;
		bool isRunning = true;

		ADC adc;
		GPIO gpio0(GPIO0_BASE);
		InterruptHandler interruptHandler(&gpio0, GPIO0_INTB);
		interruptHandler.enableInterrupt(PINS_FOR_IRQ_GENERATION);
		//displayMenu();
		while (isRunning) {
			signal = interruptHandler.awaitInterruptGPIO();

			std::cout << signalInput[((int)signal) - 23] << std::endl;
//			if (signal == Signal::BTN_START_RELEASED) {
//
//				std::cout << "" << std::endl;
//				HALINSTANCE.simpleMotorRight();
//				signal = interruptHandler.awaitInterruptGPIO();
//				while (signal != Signal::LB_HIGHT_ENTERED) {
//					signal = interruptHandler.awaitInterruptGPIO();
//				}
//				std::cout << adc.read() << std::endl;
//				signal = interruptHandler.awaitInterruptGPIO();
//				if (signal == Signal::HIGHT_NOT_OK) {
//					std::cout << adc.read() << std::endl;
//				}
//				signal = interruptHandler.awaitInterruptGPIO();
//
//				if (signal == Signal::HIGHT_OK) {
//					std::cout << adc.read() << std::endl;
//				}
//				signal = interruptHandler.awaitInterruptGPIO();
//				if (signal == Signal::HIGHT_NOT_OK) {
//					std::cout << adc.read() << std::endl;
//				}
//				signal = interruptHandler.awaitInterruptGPIO();
//				if (signal == Signal::HIGHT_OK) {
//					std::cout << adc.read() << std::endl;
//				}
//				signal = interruptHandler.awaitInterruptGPIO();
//
//				if (signal == Signal::HIGHT_NOT_OK) {
//					std::cout << adc.read() << std::endl;
//				}
//
//			HALINSTANCE.motorStop();
//
//			} else if (signal == Signal::BTN_STOP_RELEASED) {
//				isRunning = false;
//			} else if (signal == Signal::ESTOP_PRESSED) {
//				displayMenu();
//			}
		}
//		std::cout << "exiting" << std::endl;
//		interruptHandler.disableInterrupt();
	}

	void displayMenu() {
		std::cout << "[Stop Button]  terminate Program" << std::endl;
		std::cout << "[Start Button] start hight measurement" << std::endl;
		std::cout << "[Reset Button] stop hight measurement" << std::endl;
		std::cout << "[E-Stop]       display help menu" << std::endl;
	}
};
