#ifndef SRC_LAMPCONTROL_H_
#define SRC_LAMPCONTROL_H_

#include "../hal/HAL.h"
#include "../servicetools/Channel.h"

#include <thread>

/**
 * @brief Colors of lamps
 */
enum class COLOR
{
	GREEN, YELLOW, RED,
};

/**
 * @brief Blink speed for lamps
 *
 */
enum class SPEED
{
	SLOW, FAST,
};


class LampControl
{
public:
	explicit LampControl(Channel& disp2lamp);
	virtual ~LampControl();
	void operator()();
	void lampOn(COLOR c);
	void lampOff(COLOR c);
	void blinkingOn(COLOR c, SPEED s);
	void blinkingOff(COLOR c);
	void setBlinkingFlag(bool flag);
	std::thread _thread;

private:
	void blinkingLoop();
	Channel& _2me;
	bool _isGreenBlinking;
	bool _isYellowBlinking;
	bool _isRedBlinking;
	bool _isBlinkingActive;
	int _delay;
};

#endif
