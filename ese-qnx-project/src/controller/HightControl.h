/*
 * HightControl.h
 *
 *  Created on: 16.11.2018
 *      Author: acf139
 */

#ifndef SRC_CONTROLLER_HIGHTCONTROL_H_
#define SRC_CONTROLLER_HIGHTCONTROL_H_

#include "../servicetools/CalibrationValues.h"
#include "../servicetools/enums.h"
#include "../servicetools/Channel.h"
#include "../sensor/ADC.h"

#define BAND_HIGHT 3641 //based on 5000 measurements on band 6
#define ZERO_HIGHT 2487 //the hight of a coded zero based on 50 measurements on band 6
#define ONE_HIGHT 2657 //the hight of a coded one based on 50 measurements on band 6
#define FLAT_PUCK_HIGHT 2528 //the hight of a flat puck based on 50 measurements on band 6
#define HIGH_PUCK_HIGHT 2301 //the hight of a high puck based on 50 measurements on band 6
#define DELTA_MAX 60 //the highest delta in over 5000 measurements was 48
#define DELTA_MIN 48 //the highest delta in over 5000 measurements was 48
#define NO_OF_SAMPLES_FOR_CALIBRATION 50 //overflow could be produced if number is too high...500 is okay //TODO check on more systems 500 didnt seem to work on band 8...
#define NO_OF_SAMPLES_FOR_MEASUREMENT 20
//static constexpr Component _thisComponent = Component::HIGHTCTRL;

class HightControl {
private:
	Channel& _2disp;
	Channel& _2me;
	ADC& _adc;
	HightFSMState _state;
	uint16_t _hightValueArr[3];
	struct HightValues _refHight;
	std::queue <PUKType> _measuredPucks;

	void sendPUKData();
	void setMetal();
public:
	HightControl(Channel& hight2disp, Channel& disp2hight, ADC& adc, struct HightValues hightValues);
	virtual ~HightControl();
	void operator()();
};

#endif /* SRC_CONTROLLER_HIGHTCONTROL_H_ */
