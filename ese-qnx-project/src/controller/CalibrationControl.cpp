/*
 * CalibrationControl.cpp
 *
 *  Created on: 09.01.2019
 *      Author: acf139
 */

#include "CalibrationControl.h"
#include "../sensor/ADC.h"
#include "../controller/HightControl.h"
#include "../hal/HAL.h"
#include "../servicetools/Machine.h"
#include "../servicetools/Logger.h"

#include <time.h>
#include <string>

CalibrationControl::CalibrationControl(Channel& cali2disp, Channel& disp2cali, ADC& adc)
: 	_2disp(cali2disp),
	_2me(disp2cali),
	_adc(adc)
{}

CalibrationControl::~CalibrationControl() {
}

void CalibrationControl::operator ()() {
	LOG_WRITE("calibrationcontrol started");
	bool notStopped = true;
	CalibrationValues calibrationValues;
	while(1) {
		auto msg = _2me.dequeue();
		if(msg.signal == Signal::STARTCALIBRATION) {
			_2disp.enqueue(*(std::make_unique<Message>(Signal::STARTCALIBRATION, Component::HIGHT, Component::CALIBRATION)));
			_2disp.enqueue(*(std::make_unique<Message>(Signal::STARTCALIBRATION, Component::OTHER_MACHINE, Component::CALIBRATION)));
			LOG_WRITE("calibration in calibration controller started");
			calibrationValues = calibrate(notStopped);
			if(notStopped) {
				LOG_WRITE("Calibration done, you might want to empty the slide");
			}
			else {
				LOG_WRITE("Calibration aborted");
			}
			LOG_WRITE("hightValues in calibrate");
			printHightValues(calibrationValues.hightValues);
			printCalibrationValues(calibrationValues);
			_2disp.enqueue(*(std::make_unique<Message>(Signal::CALIBRATIONVALUES, Component::MAIN, Component::CALIBRATION, calibrationValues)));
			_2disp.enqueue(*(std::make_unique<Message>(Signal::CALIBRATIONVALUES, Component::HIGHT, Component::CALIBRATION,	calibrationValues)));
			turnOffActors();
		}

	}

}

CalibrationValues CalibrationControl::calibrate(bool &notStopped) {
	CalibrationValues calibrationValues;
	_2disp.enqueue(*(std::make_unique<Message>(Signal::GREEN_LIGHT_BLINK_FAST, Component::LAMP, Component::MAIN)));
	notStopped = calibrateHight(calibrationValues);
	LOG_WRITE("hightValues in calibrate");
	printHightValues(calibrationValues.hightValues);
	if(notStopped) {
		notStopped = calibrateTimers(calibrationValues);
	}
	return calibrationValues;
}

bool CalibrationControl::calibrateHight(CalibrationValues &calibrationValues) {
	struct HightValues hightValues;
	struct TimerValues timerValues;
	Message msg;

	uint16_t delta = 0;
	LOG_WRITE("calibratingHight");
	/**
	* Machine2 takes puk from Machine1 then waits for machine1 to finish
	*/
	if(MY_MACHINE == MACHINE2) {
		Message msg;

		do {
			msg = _2me.dequeue();
			LOG_WRITE(std::string("Signal received: ") + Enums::signalToString(msg.signal));//TODO remove
		} while(msg.signal != Signal::CALIBRATIONSIGNAL_REQ);

		if(msg.signal == Signal::STOPCALIBRATION || msg.signal == Signal::BTN_STOP_PRESSED) {
			abortRoutine();
			return false;
		}
		LOG_WRITE("Message received: timespec.sec " + std::to_string(msg._timespec.tv_sec));
		LOG_WRITE("Message received: timespec.ns " + std::to_string(msg._timespec.tv_nsec));
		_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_RIGHT, Component::MOTOR, Component::CALIBRATION)));


		if(!waitForSignal(Signal::LB_ENTRY_ENTERED)) {
			abortRoutine();
			return false;
		}

		_2disp.enqueue(*(std::make_unique<Message>(Signal::CALIBRATIONSIGNAL_ACK, Component::OTHER_MACHINE, Component::CALIBRATION)));
		_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_LEFT, Component::MOTOR, Component::CALIBRATION)));
		
		calibrationValues.timerValues.bandOne2bandTwo = 0; //cant be set on band2

		if(!waitForSignal(Signal::CALIBRATIONSIGNAL_ACK)) {
			abortRoutine();
			return false;
		}		
		_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_STOP, Component::MOTOR, Component::CALIBRATION)));
		
		//wait for machine1 to finish
		do {
			msg = _2me.dequeue();
		} while((msg.signal != Signal::CALIBRATIONSIGNAL_FIN) && (msg.signal != Signal::STOPCALIBRATION) && (msg.signal != Signal::BTN_STOP_PRESSED));
		
		if((msg.signal == Signal::BTN_STOP_PRESSED) || (msg.signal == Signal::STOPCALIBRATION)) {
			abortRoutine();
			return false;
		}
	}



	//Measure With Drilling Puck
	LOG_WRITE("please put a puck with drilling on the conveyor");
	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_RIGHT, Component::MOTOR, Component::CALIBRATION)));
	if(!waitForSignal(Signal::LB_HIGHT_ENTERED)) {
		abortRoutine();
		return false;
	}
	LOG_WRITE("measuring Hight");
	hightValues.drillingHight = hightMeasurement(delta);
	LOG_WRITE("Hight measured");
	LOG_WRITE("with Drilling measured");



	//Measure Flat Puck

	LOG_WRITE("please put a flat puck on the conveyor");
	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_RIGHT, Component::MOTOR, Component::CALIBRATION)));
	if(!waitForSignal(Signal::LB_HIGHT_ENTERED)) {
		abortRoutine();
		return false;
	}

	hightValues.hightOfFlatPuck = hightMeasurement(delta);


	/**
	 * Measure High Puck
	 */

	LOG_WRITE("please put a high puck on the conveyor");
	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_RIGHT, Component::MOTOR, Component::CALIBRATION)));
	if(!waitForSignal(Signal::LB_HIGHT_ENTERED)) {
		abortRoutine();
		return false;
	}


	hightValues.hightOfHighPuck = hightMeasurement(delta);


	//Measure Ones
	LOG_WRITE("please put a 5 coded puck on the conveyor");
	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_RIGHT, Component::MOTOR, Component::CALIBRATION)));
	if(!waitForSignal(Signal::LB_HIGHT_ENTERED)) {
		abortRoutine();
		return false;
	}

	hightValues.hightOfOne =  hightMeasurement(delta);


	//Measure Zeroes
	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_RIGHT, Component::MOTOR, Component::CALIBRATION)));
	if(!waitForSignal(Signal::HIGHT_NOT_OK)) {
		abortRoutine();
		return false;
	}
	if(!waitForSignal(Signal::HIGHT_OK)) {
		abortRoutine();
		return false;
	}

	hightValues.hightOfZero = hightMeasurement(delta);

	
	//set delta	
	if(delta < DELTA_MAX && delta > DELTA_MIN) {
		hightValues.delta = delta;
	}
	else {
		hightValues.delta = DELTA_MAX;
	}
	
	//drive back to the beginning	
	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_LEFT, Component::MOTOR, Component::CALIBRATION)));
	if(!waitForSignal(Signal::LB_ENTRY_ENTERED)) {
		abortRoutine();
		return false;
	}
	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_STOP, Component::MOTOR, Component::CALIBRATION)));

	LOG_WRITE("hightValues in CalibrationControl");
	printHightValues(hightValues);

	calibrationValues.hightValues = hightValues;
	return true;
}

bool CalibrationControl::calibrateTimers(CalibrationValues &calibrationValues) {
	//https://www.qnx.com/developers/docs/6.4.1/neutrino/lib_ref/c/clock_gettime.html

	TimerValues timerValues;
	struct timespec start, stop;
	LOG_WRITE("please remove the puck from LB_ENTRY and put it in rightmost position at LB_ENTRY on the band to calibrate the longest way possible to LB_HIGHT and press the START_BTN to start the calibration");

	if(!waitForSignal(Signal::BTN_START_RELEASED)) {
		abortRoutine();
		return false;
	}
	startTimeMeasurement(start);

	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_RIGHT, Component::MOTOR, Component::CALIBRATION)));
	if(!waitForSignal(Signal::LB_ENTRY_LEFT)) {
		abortRoutine();
		return false;
	}

	if(!waitForSignal(Signal::LB_HIGHT_ENTERED)) {
		abortRoutine();
		return false;
	}

	timerValues.entry2hightLong = stopTimeMeasurement(start, stop);
	LOG_WRITE("entry2hight Long in ms: " + std::to_string(timerValues.entry2hightLong)); //TODO remove

	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_LEFT, Component::MOTOR, Component::CALIBRATION)));

	if(!waitForSignal(Signal::LB_ENTRY_ENTERED)) {
		abortRoutine();
		return false;
	}

	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_RIGHT, Component::MOTOR, Component::CALIBRATION)));

	startTimeMeasurement(start);


	if(!waitForSignal(Signal::LB_HIGHT_ENTERED)) {
		abortRoutine();
		return false;
	}

	timerValues.entry2hightShort = stopTimeMeasurement(start, stop);
	LOG_WRITE("entry2hight Short in ms: " + std::to_string(timerValues.entry2hightShort));


	//measure hight2gate
	startTimeMeasurement(start);

	if(!waitForSignal(Signal::LB_METAL_ENTERED)) {
		abortRoutine();
		return false;
	}

	timerValues.hight2gate = stopTimeMeasurement(start, stop);
	LOG_WRITE("hight2gate in ms: " + std::to_string(timerValues.hight2gate));

	_2disp.enqueue(*(std::make_unique<Message>(Signal::GATE_OPEN, Component::GATE, Component::CALIBRATION)));

	//measure gate to exit
	startTimeMeasurement(start);

	if(!waitForSignal(Signal::LB_EXIT_ENTERED)) {
		abortRoutine();
		return false;
	}
	timerValues.gate2exit = stopTimeMeasurement(start, stop);
	LOG_WRITE("gate2exit in ms: " + std::to_string(timerValues.gate2exit));



	if(MY_MACHINE == MACHINE1) {
		/**
		* send to machine 2
		*/

		Message msg;

		startTimeMeasurement(start);

		LOG_WRITE("send to other Machine..."); //TODO remove

		//send to machine 2
		_2disp.enqueue(*(std::make_unique<Message>(Signal::CALIBRATIONSIGNAL_REQ, Component::OTHER_MACHINE, Component::CALIBRATION))); //TODO start.tv_sec, start.tv_sec ?

		//wait for signal from other machine
		if(!waitForSignal(Signal::CALIBRATIONSIGNAL_ACK)) {
			abortRoutine();
			return false;
		}

		timerValues.bandOne2bandTwo = stopTimeMeasurement(start, stop);

		_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_LEFT, Component::MOTOR, Component::CALIBRATION)));
		if(!waitForSignal(Signal::LB_EXIT_ENTERED)) {
			abortRoutine();
			return false;
		}
		_2disp.enqueue(*(std::make_unique<Message>(Signal::CALIBRATIONSIGNAL_ACK, Component::OTHER_MACHINE, Component::CALIBRATION)));
		LOG_WRITE("Band1 to Band2 " + std::to_string(timerValues.bandOne2bandTwo) + "ms"); //TODO remove
	}


	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_LEFT, Component::MOTOR, Component::CALIBRATION)));

	if(!waitForSignal(Signal::LB_METAL_LEFT)) {
		abortRoutine();
		return false;
	}

	_2disp.enqueue(*(std::make_unique<Message>(Signal::GATE_CLOSE, Component::GATE, Component::CALIBRATION)));
	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_RIGHT, Component::MOTOR, Component::CALIBRATION)));

	if(!waitForSignal(Signal::LB_METAL_ENTERED)) {
		abortRoutine();
		return false;
	}

	startTimeMeasurement(start);

	if(!waitForSignal(Signal::LB_SLIDE_ENTERED)) {
		abortRoutine();
		return false;
	}

	timerValues.gate2slide = stopTimeMeasurement(start, stop);
	LOG_WRITE("gate2slide in ms: " + std::to_string(timerValues.gate2slide) + "ms");


	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_STOP, Component::MOTOR, Component::CALIBRATION)));
	timerValues.delta = DELTA_TIME_STD;

	_2disp.enqueue(*(std::make_unique<Message>(Signal::CALIBRATIONSIGNAL_FIN, Component::OTHER_MACHINE, Component::CALIBRATION, timerValues.bandOne2bandTwo)));
	if(MY_MACHINE == MACHINE1) {
		//wait for machine 2 to finish
		if(!waitForSignal(Signal::CALIBRATIONSIGNAL_FIN)) {
			abortRoutine();
			return false;
		}
	}
	calibrationValues.timerValues = timerValues;
	return true;
}

void CalibrationControl::printHightValues(struct HightValues hightValues) {
	LOG_WRITE("Delta: " + std::to_string(hightValues.delta));
	LOG_WRITE("Flat Puck: " + std::to_string(hightValues.hightOfFlatPuck));
	LOG_WRITE("High Puck: " + std::to_string(hightValues.hightOfHighPuck));
	LOG_WRITE("Zero: " + std::to_string(hightValues.hightOfZero));
	LOG_WRITE("One: " + std::to_string(hightValues.hightOfOne));
}

void CalibrationControl::printCalibrationValues(struct CalibrationValues caliValues) {
	LOG_WRITE("print calibrationValues");
	LOG_WRITE("print hightValues");
	printHightValues(caliValues.hightValues);
	LOG_WRITE("print timerValues");
	printTimerValues(caliValues.timerValues);
}

void CalibrationControl::printTimerValues(struct TimerValues timerValues) {
	LOG_WRITE("Delta: " + std::to_string(timerValues.delta));
	LOG_WRITE("Entry to Hight short: " + std::to_string(timerValues.entry2hightShort) + "ms");
	LOG_WRITE("Entry to Hight long: " + std::to_string(timerValues.entry2hightLong) + "ms");
	LOG_WRITE("Hight to Gate: " + std::to_string(timerValues.hight2gate) + "ms");
	LOG_WRITE("Gate to Exit: " + std::to_string(timerValues.gate2exit) + "ms");
	LOG_WRITE("Gate to Slide: " + std::to_string(timerValues.gate2slide) + "ms");
	LOG_WRITE("Machine1 to Machine2: " + std::to_string(timerValues.bandOne2bandTwo) + "ms");
}

/**
 * returns false if stopcalibration signal was received else true
 */
bool CalibrationControl::waitForSignal(Signal signal) {
	Message msg;
	do {
		msg = _2me.dequeue();
	} while((msg.signal != signal) && (msg.signal != Signal::STOPCALIBRATION) && (msg.signal != Signal::BTN_STOP_PRESSED));

	if((msg.signal == Signal::STOPCALIBRATION) || (msg.signal == Signal::BTN_STOP_PRESSED)) {
		return false;
	}
	else {
		return true;
	}
}

void CalibrationControl::startTimeMeasurement(struct timespec &start) {
	if(clock_gettime(CLOCK_REALTIME, &start) == -1) {
		perror("clock gettime");
	}
}

double CalibrationControl::stopTimeMeasurement(struct timespec &start, struct timespec &stop) {
	if(clock_gettime(CLOCK_REALTIME, &stop) == -1) {
		perror("clock gettime");
	}
	return ((stop.tv_sec - start.tv_sec) * THOUSAND) + (double)(stop.tv_nsec - start.tv_nsec) / (double)MILLION;
}

uint32_t CalibrationControl::hightMeasurement(uint16_t &delta) {
	uint32_t hightAverage = 0;
	uint16_t measuredVal;
	uint16_t minVal = UINT16_MAX;
	uint16_t maxVal = 0;

	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_STOP, Component::MOTOR, Component::CALIBRATION)));

	for(int i = 0; i < NO_OF_SAMPLES_FOR_CALIBRATION; i++) {
		measuredVal = _adc.read();
		if(measuredVal < minVal) {
			minVal = measuredVal;
		}
		if(measuredVal > maxVal) {
			maxVal = measuredVal;
		}
		hightAverage = hightAverage  + measuredVal;
		LOG_WRITE(std::to_string(measuredVal)); //TODO remove
	}
	 hightAverage = hightAverage / NO_OF_SAMPLES_FOR_CALIBRATION;

	if(delta < (maxVal - minVal)) {
		delta = maxVal - minVal;
	}

	return hightAverage;
}


void CalibrationControl::abortRoutine(void) {
	_2disp.enqueue(*(std::make_unique<Message>(Signal::STOPCALIBRATION, Component::OTHER_MACHINE, Component::CALIBRATION)));
	turnOffActors();
}

void CalibrationControl::turnOffActors(void) {
	_2disp.enqueue(*(std::make_unique<Message>(Signal::GATE_CLOSE, Component::GATE, Component::CALIBRATION)));
	_2disp.enqueue(*(std::make_unique<Message>(Signal::MOTOR_STOP, Component::MOTOR, Component::CALIBRATION)));
	_2disp.enqueue(*(std::make_unique<Message>(Signal::GREEN_LIGHT_BLINK_OFF, Component::LAMP, Component::CALIBRATION)));
}
