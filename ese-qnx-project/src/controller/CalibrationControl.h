/*
 * CalibrationControl.h
 *
 *  Created on: 09.01.2019
 *      Author: acf139
 */

#ifndef SRC_CONTROLLER_CALIBRATIONCONTROL_H_
#define SRC_CONTROLLER_CALIBRATIONCONTROL_H_

#include "../servicetools/Channel.h"
#include "../servicetools/CalibrationValues.h"
#include "../sensor/ADC.h"

#define DELTA_TIME_STD 25
#define BILLION  1000000000L
#define MILLION  1000000
#define THOUSAND 1000
class CalibrationControl {
private:
	Channel& _2disp;
	Channel& _2me;
	ADC& _adc;


	bool waitForSignal(Signal signal);
	CalibrationValues calibrate(bool &notStopped);
	bool calibrateHight(CalibrationValues &calibrationValues);
	bool calibrateTimers(CalibrationValues &calibrationValues);
	void printCalibrationValues(CalibrationValues calibrationValues);
	void printHightValues(HightValues hightValues);
	void printTimerValues(TimerValues timerValues);
	void startTimeMeasurement(struct timespec &start);
	double stopTimeMeasurement(struct timespec &start, struct timespec &stop);
	uint32_t hightMeasurement(uint16_t &delta);
	void abortRoutine(void);
	void turnOffActors(void);
public:
	CalibrationControl(Channel& cali2disp, Channel& disp2cali, ADC& adc);
	virtual ~CalibrationControl();
	void operator()();
};

#endif /* SRC_CONTROLLER_CALIBRATIONCONTROL_H_ */
//
//ANFANG -> Hoehe
