#ifdef __DUMMY_TEST

#ifndef DummyHAL_H_
#define DummyHAL_H_

#define HALINSTANCE DummyHAL::getHAL()

class DummyHAL
{
private:
	DummyHAL();
	virtual ~DummyHAL();
	//Constants...
public:
	static DummyHAL& getHAL();
	void greenLightOn();
	void greenLightOff();
	void yellowLightOn();
	void yellowLightOff();
	void redLightOn();
	void redLightOff();
	void gateOpen();
	void gateClose();
	void motorOff();
	void motorOn();
	void motorSlow();
	void motorFast();

	void motorLeft();
	void motorRight();

	void simpleMotorLeft();
	void simpleMotorRight();
	void simpleMotorLeftOff();
	void simpleMotorRightOff();

	void motorStop();
};

#endif
#endif
