  /*
 * HightControl.cpp
 *
 *  Created on: 16.11.2018
 *      Author: acf139
 */
#ifdef __DUMMY_TEST
#include "../hal/DummyHAL.h"
#else


#include "HightControl.h"
#include "../hal/HAL.h"
#endif

#include "../sensor/ADC.h"
#include "../servicetools/Message.h"
#include <queue>


HightControl::HightControl(Channel& hight2disp, Channel& disp2hight, ADC& adc, struct HightValues hightValues)
: 	_2disp(hight2disp),
	_2me(disp2hight),
	_adc(adc),
	_refHight(hightValues)
{
	_state = HightFSMState::READY;
}

HightControl::~HightControl() {
	// TODO Auto-generated destructor stub
}

void HightControl::sendPUKData() {
//	std::cout << "send PUK Data" << std::endl;
 	if(_measuredPucks.empty()) {
		_2disp.enqueue(*(std::make_unique<Message>(Signal::PUKDATA, Component::MAIN, Component::HIGHT, PUKType::TYPENODATA)));
	}
	else {
		std::cout << "send PUK Data: " << Enums::pukTypeToString(_measuredPucks.front()) << std::endl;
		_2disp.enqueue(*(std::make_unique<Message>(Signal::PUKDATA, Component::MAIN, Component::HIGHT, _measuredPucks.front())));
		_measuredPucks.pop();
	}
}

void HightControl::setMetal() {
	if(_measuredPucks.empty()) {
		std::cout << "received Signal from Metal detector but no Puck was in the queue" << std::endl;
	}
	else if(_measuredPucks.front() == PUKType::TYPE3_1) {
		_measuredPucks.front() = PUKType::TYPE2_1;
	}
	else {
		_measuredPucks.front() = PUKType::TYPEERROR;
		std::cout << "isMetal but no Drilling" << std::endl;
	}
}

void HightControl::operator ()(){
	//TODO Timer need to be used, LB_HIGHT_LEFT might never appear
	std::cout << "hightControl started" << std::endl;
	Message msg;
	uint8_t type;
	uint32_t hightValue;
	while(1) {
		switch(_state) {
			case HightFSMState::CALIBRATION:
				std::cout << "HightFSM in Calibration" << std::endl;
				msg = _2me.dequeue();
				if(msg.signal == Signal::CALIBRATIONVALUES) {
					_state = HightFSMState::READY;
					_refHight = msg.calibrationValues.hightValues;
					std::cout << "HightFSM received CalibrationValues" << std::endl;
				}
			break;
			case HightFSMState::READY:
				msg = _2me.dequeue();
				if(msg.signal == Signal::LB_HIGHT_ENTERED) {
					_state = HightFSMState::FIRST_EDGE;
					hightValue = 0;
					for(int i = 0; i < NO_OF_SAMPLES_FOR_MEASUREMENT; i++) {
						hightValue += _adc.read();
					}
					_hightValueArr[2] = hightValue / NO_OF_SAMPLES_FOR_MEASUREMENT;
				}
				else if(msg.signal == Signal::GETPUKDATA) {
					sendPUKData();
				}
				else if(msg.signal == Signal::IS_METAL) {
					setMetal();
				}
				else if(msg.signal == Signal::STARTCALIBRATION) {
					_state = HightFSMState::CALIBRATION;
				}
			break;
			case HightFSMState::FIRST_EDGE:
				msg = _2me.dequeue();
				if(msg.signal == Signal::HIGHT_NOT_OK) {
					_state = HightFSMState::SECOND_EDGE;
				}
				else if (msg.signal == Signal::LB_HIGHT_LEFT) {
					_state = HightFSMState::FLAT_SURFACE_PUCK;
				}
				else if(msg.signal == Signal::GETPUKDATA) {
					sendPUKData();
				}
				else if(msg.signal == Signal::IS_METAL) {
					setMetal();
				}
				else if(msg.signal == Signal::STARTCALIBRATION) {
					_state = HightFSMState::CALIBRATION;
				}
			break;
			case HightFSMState::SECOND_EDGE:
				msg = _2me.dequeue();
				if(msg.signal == Signal::HIGHT_OK) {
					_state = HightFSMState::THIRD_EDGE;
					hightValue = 0;
					for(int i = 0; i < NO_OF_SAMPLES_FOR_MEASUREMENT; i++) {
						hightValue += _adc.read();
					}
					_hightValueArr[1] = hightValue / NO_OF_SAMPLES_FOR_MEASUREMENT;
				}
				else if(msg.signal == Signal::LB_HIGHT_LEFT) {
					_state = HightFSMState::ERROR;
				}
				else if(msg.signal == Signal::GETPUKDATA) {
					sendPUKData();
				}
				else if(msg.signal == Signal::IS_METAL) {
					setMetal();
				}
				else if(msg.signal == Signal::STARTCALIBRATION) {
					_state = HightFSMState::CALIBRATION;
				}
			break;
			case HightFSMState::THIRD_EDGE:
				msg = _2me.dequeue();
				if(msg.signal == Signal::HIGHT_NOT_OK) {
					_state = HightFSMState::FOURTH_EDGE;
				}
				else if(msg.signal == Signal::LB_HIGHT_LEFT) {
					_state = HightFSMState::WITH_DRILLING_PUCK;
				}
				else if(msg.signal == Signal::GETPUKDATA) {
					sendPUKData();
				}
				else if(msg.signal == Signal::IS_METAL) {
					setMetal();
				}
				else if(msg.signal == Signal::STARTCALIBRATION) {
					_state = HightFSMState::CALIBRATION;
				}
			break;
			case HightFSMState::FOURTH_EDGE:
				msg = _2me.dequeue();
				if(msg.signal == Signal::HIGHT_OK) {
					_state = HightFSMState::FIFTH_EDGE;
					hightValue = 0;
					for(int i = 0; i < NO_OF_SAMPLES_FOR_MEASUREMENT; i++) {
						hightValue += _adc.read();
					}
					_hightValueArr[0] = hightValue / NO_OF_SAMPLES_FOR_MEASUREMENT;
				}
				else if(msg.signal == Signal::LB_HIGHT_LEFT) {
					_state = HightFSMState::ERROR;
				}
				else if(msg.signal == Signal::GETPUKDATA) {
					sendPUKData();
				}
				else if(msg.signal == Signal::IS_METAL) {
					setMetal();
				}
				else if(msg.signal == Signal::STARTCALIBRATION) {
					_state = HightFSMState::CALIBRATION;
				}
			break;
			case HightFSMState::FIFTH_EDGE:
				msg = _2me.dequeue();
				if(msg.signal == Signal::HIGHT_NOT_OK) {
					_state = HightFSMState::SIXTH_EDGE;
				}
				else if(msg.signal == Signal::LB_HIGHT_LEFT) {
					_state = HightFSMState::WITH_DRILLING_PUCK;
				}
				else if(msg.signal == Signal::GETPUKDATA) {
					sendPUKData();
				}
				else if(msg.signal == Signal::IS_METAL) {
					setMetal();
				}
				else if(msg.signal == Signal::STARTCALIBRATION) {
					_state = HightFSMState::CALIBRATION;
				}
			break;
			case HightFSMState::SIXTH_EDGE:
				msg = _2me.dequeue();
				if(msg.signal == Signal::LB_HIGHT_LEFT) {
					_state = HightFSMState::CODED_PUCK;
				}
				else if(msg.signal == Signal::HIGHT_NOT_OK) {
					_state = HightFSMState::ERROR;
				}
				else if(msg.signal == Signal::GETPUKDATA) {
					sendPUKData();
				}
				else if(msg.signal == Signal::IS_METAL) {
					setMetal();
				}
				else if(msg.signal == Signal::STARTCALIBRATION) {
					_state = HightFSMState::CALIBRATION;
				}
			break;
			case HightFSMState::CODED_PUCK:
				type = 0;
				std::cout << "coded_puck hight values: " << std::endl;
				for(int i = 0; i < 3; i++) { //iterate over array
					if((_hightValueArr[i] < (_refHight.hightOfOne + _refHight.delta)) && (_hightValueArr[i] > (_refHight.hightOfOne - _refHight.delta))) { //bit is 1
						type = type | (1 << i);
					}
					std::cout << i << ": " << _hightValueArr[i] << std::endl;
				}
				_state = HightFSMState::READY;
				switch(type) {
				case 1:
					_measuredPucks.push(PUKType::TYPE4_1);
				break;
				case 2:
					_measuredPucks.push(PUKType::TYPE4_2);
				break;
				case 4:
					_measuredPucks.push(PUKType::TYPE4_4);
				break;
				case 5:
					_measuredPucks.push(PUKType::TYPE4_5);
				break;
				default:
					_measuredPucks.push(PUKType::TYPEERROR);
				break;
				}
			break;
			case HightFSMState::FLAT_SURFACE_PUCK:
				if((_hightValueArr[2] < (_refHight.hightOfHighPuck + _refHight.delta)) && (_hightValueArr[2] > (_refHight.hightOfHighPuck - _refHight.delta))) {
					_measuredPucks.push(PUKType::TYPE3_2);
				}
				else if((_hightValueArr[2] < (_refHight.hightOfFlatPuck + _refHight.delta)) && (_hightValueArr[2] > (_refHight.hightOfFlatPuck - _refHight.delta))) {
					_measuredPucks.push(PUKType::TYPE1);
				}
				else {
					_measuredPucks.push(PUKType::TYPEERROR);
				}

				_state = HightFSMState::READY;
			break;
			case HightFSMState::WITH_DRILLING_PUCK:
				if((_hightValueArr[2] < (_refHight.drillingHight + _refHight.delta)) && ((_hightValueArr[2] > _refHight.drillingHight - _refHight.delta))) {
					_measuredPucks.push(PUKType::TYPE3_1);
				}
				else {
					_measuredPucks.push(PUKType::TYPEERROR);
				}
				_state = HightFSMState::READY;
			break;
			case HightFSMState::ERROR:
				_measuredPucks.push(PUKType::TYPEERROR);
				_state = HightFSMState::READY;
			break;
		}
	}

}

