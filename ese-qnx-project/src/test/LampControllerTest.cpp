/*
 * LampControllerTest.cpp
 *
 *  Created on: 07.01.2019
 *  Author: pm
 */

#ifdef __DUMMY_TEST
#include "../hal/DummyHAL.h"
#else
#include "../hal/HAL.h"
#endif

#include "../servicetools/Logger.h"
#include "../servicetools/Channel.h"

#include <iostream>
#include <chrono>
#include <thread>
#include "../servicetools/Message.h"

#include "../controller/LampControl.h"

class LampControllerTest {

public:
	LampControllerTest(){
		Channel testChannel(15);
		LampControl lmpContr(testChannel);

		std::cout << "Starte Lampentest 1: Blinking-Loop Test" << std::endl;
		lmpContr.setBlinkingFlag(true);
		lmpContr.blinkingOn(COLOR::GREEN, SPEED::FAST);
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		//lmpContr.blinkingOff(COLOR::GREEN);
		lmpContr.blinkingOn(COLOR::YELLOW, SPEED::FAST);
		lmpContr.blinkingOff(COLOR::GREEN);


		std::cout << "Lampentest 1 beendet." << std::endl;
	}

//	/**
//	 *
//	 */
//	void LampControl::operator ()() {
//		while (true) {
//			Message rcvMsg = _2me.dequeue();
//			LOG_WRITE("LampController received message.");
//			switch (rcvMsg.signal) {
//
//			case Signal::GREEN_LIGHT_ON:
//				lampOn(COLOR::GREEN);
//				break;
//			case Signal::GREEN_LIGHT_OFF:
//				lampOff(COLOR::GREEN);
//				break;
//			case Signal::GREEN_LIGHT_BLINK_SLOW:
//				blinkingLoop(true, COLOR::GREEN);
//				break;
//			case Signal::GREEN_LIGHT_BLINK_FAST:
//				blinkingLoop(true, COLOR::GREEN);
//				break;
//			case Signal::GREEN_LIGHT_BLINK_OFF:
//				blinkingLoop(false, COLOR::GREEN);
//				break;
//			case Signal::YELLOW_LIGHT_ON:
//				lampOn(COLOR::YELLOW);
//				break;
//			case Signal::YELLOW_LIGHT_OFF:
//				lampOff(COLOR::YELLOW);
//				break;
//			case Signal::YELLOW_LIGHT_BLINK_SLOW:
//				blinkingLoop(true, COLOR::YELLOW);
//				break;
//			case Signal::YELLOW_LIGHT_BLINK_FAST:
//				blinkingLoop(true, COLOR::YELLOW);
//				break;
//			case Signal::YELLOW_LIGHT_BLINK_OFF:
//				blinkingLoop(true, COLOR::YELLOW);
//				break;
//			case Signal::RED_LIGHT_ON:
//				lampOn(COLOR::RED);
//				break;
//			case Signal::RED_LIGHT_OFF:
//				lampOff(COLOR::RED);
//				break;
//			case Signal::RED_LIGHT_BLINK_SLOW:
//				blinkingLoop(true, COLOR::RED);
//				break;
//			case Signal::RED_LIGHT_BLINK_FAST:
//				blinkingLoop(true, COLOR::RED);
//				break;
//			case Signal::RED_LIGHT_BLINK_OFF:
//				blinkingLoop(true, COLOR::RED);
//				break;
//			default:
//				LOG_WRITE("Cryptic Signal in Layer 8 detected");
//				break;
//			}
//		}
//	}


};


