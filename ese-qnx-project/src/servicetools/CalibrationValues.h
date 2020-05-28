/*
 * HightValues.h
 *
 *  Created on: 13.12.2018
 *      Author: acf139
 */

#ifndef SRC_SERVICETOOLS_CALIBRATIONVALUES_H_
#define SRC_SERVICETOOLS_CALIBRATIONVALUES_H_

#include <stdint.h>

struct HightValues {
	uint16_t hightOfOne;
	uint16_t hightOfZero;
	uint16_t hightOfFlatPuck;
	uint16_t hightOfHighPuck;
	uint16_t drillingHight;
	uint16_t delta;
};

struct TimerValues {
	int entry2hightShort;
	int entry2hightLong;
	int hight2gate;
	int gate2slide;
	int gate2exit;
	int bandOne2bandTwo;
	int delta;
};

struct CalibrationValues {
	struct HightValues hightValues;
	struct TimerValues timerValues;
};
#endif /* SRC_SERVICETOOLS_CALIBRATIONVALUES_H_ */
