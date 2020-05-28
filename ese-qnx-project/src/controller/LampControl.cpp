#include "LampControl.h"

#ifdef __DUMMY_TEST
#include "../hal/DummyHAL.h"
#else
#include "../hal/HAL.h"
#endif
#include "../servicetools/Logger.h"

LampControl::LampControl(Channel& disp2lamp)

:
		_2me(disp2lamp), _isGreenBlinking(false), _isYellowBlinking(false), _isRedBlinking(
				false), _isBlinkingActive(true), _delay(500)
{
	_thread = std::thread(&LampControl::blinkingLoop, this);
}

LampControl::~LampControl() {
	_thread.join();
}

void LampControl::blinkingLoop() {
	while (_isBlinkingActive) {
		if (_isGreenBlinking) {
			LampControl::lampOn(COLOR::GREEN);
		}
		if (_isYellowBlinking) {
			LampControl::lampOn(COLOR::YELLOW);
		}
		if (_isRedBlinking) {
			LampControl::lampOn(COLOR::RED);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(_delay));
		if (_isGreenBlinking) {
			LampControl::lampOff(COLOR::GREEN);
		}
		if (_isYellowBlinking) {
			LampControl::lampOff(COLOR::YELLOW);
		}
		if (_isRedBlinking) {
			LampControl::lampOff(COLOR::RED);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(_delay));
	}
}

//ERGAENZT: das Switch rcvMsg.signal um die Signale COLOR_LIGHT_BLINK_FAST/SLOW und OFF ergaenzt
void LampControl::operator ()() {
	while (true) {
		Message rcvMsg = _2me.dequeue();
		LOG_WRITE("Lamp received message.");
		switch (rcvMsg.signal) {
		case Signal::GREEN_LIGHT_ON:
			lampOn(COLOR::GREEN);
			break;
		case Signal::GREEN_LIGHT_OFF:
			lampOff(COLOR::GREEN);
			break;
		case Signal::GREEN_LIGHT_BLINK_SLOW:
			blinkingOn(COLOR::GREEN, SPEED::SLOW);
			break;
		case Signal::GREEN_LIGHT_BLINK_FAST:
			blinkingOn(COLOR::GREEN, SPEED::FAST);
			break;
		case Signal::GREEN_LIGHT_BLINK_OFF:
			blinkingOff(COLOR::GREEN);
			break;
		case Signal::YELLOW_LIGHT_ON:
			lampOn(COLOR::YELLOW);
			break;
		case Signal::YELLOW_LIGHT_OFF:
			lampOff(COLOR::YELLOW);
			break;
		case Signal::YELLOW_LIGHT_BLINK_SLOW:
			blinkingOn(COLOR::YELLOW, SPEED::SLOW);
			break;
		case Signal::YELLOW_LIGHT_BLINK_FAST:
			blinkingOn(COLOR::YELLOW, SPEED::FAST);
			break;
		case Signal::YELLOW_LIGHT_BLINK_OFF:
			blinkingOff(COLOR::YELLOW);
			break;
		case Signal::RED_LIGHT_ON:
			lampOn(COLOR::RED);
			break;
		case Signal::RED_LIGHT_OFF:
			lampOff(COLOR::RED);
			break;
		case Signal::RED_LIGHT_BLINK_SLOW:
			blinkingOn(COLOR::RED, SPEED::SLOW);
			break;
		case Signal::RED_LIGHT_BLINK_FAST:
			blinkingOn(COLOR::RED, SPEED::FAST);
			break;
		case Signal::RED_LIGHT_BLINK_OFF:
			blinkingOff(COLOR::RED);
			break;
		default:
			LOG_WRITE("Cryptic Signal in Layer 8 detected");
			break;
		}
	}
}

void LampControl::lampOn(COLOR c) {
	switch (c) {
	case COLOR::GREEN:
		HALINSTANCE.greenLightOn();
		break;
		case COLOR::YELLOW:
		HALINSTANCE.yellowLightOn();
		break;
		case COLOR::RED:
		HALINSTANCE.redLightOn();
		break;
	}
}

void LampControl::lampOff(COLOR c) {
	switch (c) {
	case COLOR::GREEN:
		HALINSTANCE.greenLightOff();
		break;
		case COLOR::YELLOW:
		HALINSTANCE.yellowLightOff();
		break;
		case COLOR::RED:
		HALINSTANCE.redLightOff();
		break;
	}
}

//ERGAENZT:
void LampControl::blinkingOn(COLOR c, SPEED s) {
	if (s == SPEED::FAST) {
		_delay = 500;
	} else {
		_delay = 1000;
	}
	switch (c) {
	case COLOR::GREEN:
		_isGreenBlinking = true;
		break;

	case COLOR::YELLOW:
		_isYellowBlinking = true;
		break;

	case COLOR::RED:
		_isRedBlinking = true;
		break;
	}
}

//ERGAENZT:
void LampControl::blinkingOff(COLOR c) {
	switch (c) {
	case COLOR::GREEN:
		_isGreenBlinking = false;
		lampOff(COLOR::GREEN);
		break;
	case COLOR::YELLOW:
		_isYellowBlinking = false;
		lampOff(COLOR::YELLOW);
		break;
	case COLOR::RED:
		_isRedBlinking = false;
		lampOff(COLOR::RED);
		break;
	}
}

void LampControl::setBlinkingFlag(bool flag) {
	_isBlinkingActive = flag;
}
