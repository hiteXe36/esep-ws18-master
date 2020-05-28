/*
 * HightFSMTest.cpp
 *
 *  Created on: 13.12.2018
 *      Author: acf139
 */

#include "../controller/HightControl.h"
#include "../servicetools/Logger.h"
#include "../servicetools/Message.h"
#include "../servicetools/enums.h"
#include "../hal/HAL.h"
#include "../sensor/ADC.h"
#include "../sensor/GPIO.h"
#include "../sensor/InterruptHandler.h"
#include "../controller/SensorControl.h"

class HightFSMTest {

public:
	HightFSMTest() {
		LOG_WRITE("Start of HightFSMTest!");
		//prints hightvalues for one puck



		struct HightValues hightValues = calibrate();
		printReferenceHightValues(hightValues);
////
//
		testHightMeasurement(hightValues);
		LOG_WRITE("End of HightFSMTest!");
	}

	virtual ~HightFSMTest() {

	}

	void testHightMeasurement(struct HightValues hightValues) {
		ADC adc;

		Channel toTest(5);

		Channel testToSens(5);
		Channel testToHight(5);


		HightControl hc(toTest, testToHight, adc, hightValues);
		SensorControl sc(toTest, testToSens);

		std::thread mct(std::ref(hc));
		std::thread sct(std::ref(sc));

		Message msg;

		HALINSTANCE.motorRight();
		std::cout << "press the stop button to stop the test" << std::endl;

		do {
			msg = toTest.dequeue();
			if(msg.dest == Component::HIGHT)
			{
				testToHight.enqueue(msg);
				std::cout << "To FSM->" << Enums::signalToString(msg.signal) << std::endl;
			}
			else if(msg.signal == Signal::LB_METAL_ENTERED) {
				std::cout << "To FSM->" << "GETPUKDATA" << std::endl;
				testToHight.enqueue(*(std::make_unique<Message>(Signal::GETPUKDATA, Component::HIGHT, Component::SENSOR)));
			}
			else if(msg.src == Component::HIGHT){
				std::cout << "From FSM" << std::endl;
				std::cout << "Signal: " << Enums::signalToString(msg.signal) << std::endl;
				std::cout << "PukType: " << Enums::pukTypeToString(msg._pukdata) << std::endl;
			}
		} while(msg.signal != Signal::BTN_STOP_RELEASED);


		if(msg.signal != Signal::BTN_STOP_RELEASED) {
			std::cout << Enums::signalToString(msg.signal) << std::endl;
		}

		HALINSTANCE.motorStop();


	}
//	Delta: 60
//	Flat Puck: 2554 (2494 - 2614)
//	High Puck: 2329 (2269 - 2389)
//	Zero: 2513 (2453 - 2573)
//	One: 2688 (2618 - 2748)
	void printReferenceHightValues(struct HightValues hightValues) {
		std::cout << "Delta: " << hightValues.delta << std::endl;
		std::cout << "Flat Puck: " << hightValues.hightOfFlatPuck << std::endl;
		std::cout << "High Puck: " << hightValues.hightOfHighPuck << std::endl;
		std::cout << "Zero: " << hightValues.hightOfZero << std::endl;
		std::cout << "One: " << hightValues.hightOfOne << std::endl;
	}

	struct HightValues calibrate(){
		ADC adc;
		GPIO gpio0(GPIO0_BASE);
		InterruptHandler interruptHandler(&gpio0, GPIO0_INTB);
		interruptHandler.enableInterrupt(PINS_FOR_IRQ_GENERATION);
		Signal signal;

		struct HightValues hightValues;

		uint16_t minVal = UINT16_MAX;
		uint16_t maxVal = 0;

		uint32_t flatHight = 0;
		uint32_t highHight = 0;
		uint32_t oneHight = 0;
		uint32_t zeroHight = 0;
		uint32_t drillingHight = 0;
		uint16_t delta = 0;
		uint16_t measuredVal;


		//Measure With Drilling Puck
		std::cout << "please put a puck with drilling on the conveyor" << std::endl;
		HALINSTANCE.motorRight();
		do {
			signal = interruptHandler.awaitInterruptGPIO();
		} while(signal != Signal::LB_HIGHT_ENTERED); //wait till puck is in hightmeasurement
		HALINSTANCE.motorStop();

		for(int i = 0; i < NO_OF_SAMPLES_FOR_CALIBRATION; i++) {
			measuredVal = adc.read();
			if(measuredVal < minVal) {
				minVal = measuredVal;
			}
			if(measuredVal > maxVal) {
				maxVal = measuredVal;
			}
			drillingHight  = drillingHight  + measuredVal;
			std::cout << measuredVal << std::endl;
		}
		hightValues.drillingHight = drillingHight / NO_OF_SAMPLES_FOR_CALIBRATION;

		if(delta < (maxVal - minVal)) {
			delta = maxVal - minVal;
		}
		std::cout << "maxVal: " << maxVal << std::endl;
		std::cout << "minVal: " << minVal << std::endl;




		//Measure Flat Puck
		minVal = UINT16_MAX;
		maxVal = 0;

		std::cout << "please put a flat puck on the conveyor" << std::endl;
		HALINSTANCE.motorRight();
		do {
			signal = interruptHandler.awaitInterruptGPIO();
		} while(signal != Signal::LB_HIGHT_ENTERED); //wait till puck is in hightmeasurement
		HALINSTANCE.motorStop();

		for(int i = 0; i < NO_OF_SAMPLES_FOR_CALIBRATION; i++) {
			measuredVal = adc.read();
			if(measuredVal < minVal) {
				minVal = measuredVal;
			}
			if(measuredVal > maxVal) {
				maxVal = measuredVal;
			}
			flatHight = flatHight + measuredVal;
			std::cout << measuredVal << std::endl;
		}
		hightValues.hightOfFlatPuck = flatHight / NO_OF_SAMPLES_FOR_CALIBRATION;

		if(delta < (maxVal - minVal)) {
			delta = maxVal - minVal;
		}
		std::cout << "maxVal: " << maxVal << std::endl;
		std::cout << "minVal: " << minVal << std::endl;



		/**
		 * Measure High Puck
		 */
		minVal = UINT16_MAX;
		maxVal = 0;

		std::cout << "please put a high puck on the conveyor" << std::endl;
		HALINSTANCE.motorRight();

		do {
			signal = interruptHandler.awaitInterruptGPIO();
		} while(signal != Signal::LB_HIGHT_ENTERED);
		HALINSTANCE.motorStop();
		for(int i = 0; i < NO_OF_SAMPLES_FOR_CALIBRATION; i++) {
			measuredVal = adc.read();
			highHight = highHight + measuredVal;
			if(measuredVal < minVal) {
				minVal = measuredVal;
			}
			if(measuredVal > maxVal) {
				maxVal = measuredVal;
			}
			std::cout << measuredVal << std::endl;
		}
		hightValues.hightOfHighPuck = highHight / NO_OF_SAMPLES_FOR_CALIBRATION;

		if(delta < (maxVal - minVal)) {
			delta = maxVal - minVal;
		}
		std::cout << "maxVal: " << maxVal << std::endl;
		std::cout << "minVal: " << minVal << std::endl;



		//Measure Ones
		minVal = UINT16_MAX;
		maxVal = 0;

		std::cout << "please put a 5 coded puck on the conveyor" << std::endl;
		HALINSTANCE.motorRight();
		do {
			signal = interruptHandler.awaitInterruptGPIO();
		} while(signal != Signal::LB_HIGHT_ENTERED);
		HALINSTANCE.motorStop();
		for(int i = 0; i < NO_OF_SAMPLES_FOR_CALIBRATION; i++) {
			measuredVal = adc.read();
			oneHight = oneHight + measuredVal;

			if(measuredVal < minVal) {
				minVal = measuredVal;
			}
			if(measuredVal > maxVal) {
				maxVal = measuredVal;
			}

			std::cout << measuredVal << std::endl;
		}
		hightValues.hightOfOne = oneHight / NO_OF_SAMPLES_FOR_CALIBRATION;

		if(delta < (maxVal - minVal)) {
			delta = maxVal - minVal;
		}
		std::cout << "maxVal: " << maxVal << std::endl;
		std::cout << "minVal: " << minVal << std::endl;


		//Measure Zeroes
		minVal = UINT16_MAX;
		maxVal = 0;

		HALINSTANCE.motorRight();
		do {
			signal = interruptHandler.awaitInterruptGPIO();
		} while(signal != Signal::HIGHT_NOT_OK);
		do {
			signal = interruptHandler.awaitInterruptGPIO();
		} while(signal != Signal::HIGHT_OK);
		HALINSTANCE.motorStop();
		for(int i = 0; i < NO_OF_SAMPLES_FOR_CALIBRATION; i++) {
			measuredVal = adc.read();
			zeroHight = zeroHight + measuredVal;
			if(measuredVal < minVal) {
				minVal = measuredVal;
			}
			if(measuredVal > maxVal) {
				maxVal = measuredVal;
			}
			std::cout << measuredVal << std::endl;
		}
		std::cout << "maxVal: " << maxVal << std::endl;
		std::cout << "minVal: " << minVal << std::endl;
		hightValues.hightOfZero = zeroHight / NO_OF_SAMPLES_FOR_CALIBRATION;

		HALINSTANCE.motorRight();
		do {
			signal = interruptHandler.awaitInterruptGPIO();
		} while(signal != Signal::LB_HIGHT_LEFT);
		HALINSTANCE.motorStop();
//		if(delta > DELTA_MAX) {
		std::cout << "highest delta in calibrated values was " << delta << std::endl;
		hightValues.delta = DELTA_MAX;
//			std::cout << "delta was greater than DELTA_MAX, delta was " << delta << std::endl;
//		}
//		else {
//			hightValues.delta = delta;
//		}
		interruptHandler.disableInterrupt();
		return hightValues;
	}

};


//Delta: 60
//Flat Puck: 2555
//High Puck: 2323
//Zero: 2507
//One: 2689





