/**
 * The Hardware-Abstraction-Layer (HAL) - Class.
 * This class is an interface to set, clear or read bits from the GPIOs.
 * It uses a Meyer-Singleton-Pattern.
 * @author Adem-Can Agdas, Paul Mathia
 */
#ifndef __DUMMY_TEST

#ifndef HAL_H_
#define HAL_H_

/// Macro to get current HAL instance
#define HALINSTANCE HAL::getHAL()

#include <stdint.h>

/// Hardware Abstraction Layer (HAL) to access hardware on system
class HAL
{
private:
	HAL();
	virtual ~HAL();

	static constexpr uint64_t GPIO1_BASE = 0x4804C000; /// Base address GPIO1 (page 182)
	static constexpr uint64_t GPIO2_BASE = 0x481AC000; /// Base address GPIO2 (page 183)
	static constexpr uint64_t GPIO_SIZE = 0x1000; /// Memory size of base address

	static constexpr int SETDATAOUT = 0x194; /// Address to set data on output register
	static constexpr int CLEARDATAOUT = 0x190; /// Address to clear data on output register

	static constexpr int GATE = 19;

	static constexpr int LIGHT_GREEN = 18;
	static constexpr int LIGHT_YELLOW = 17;
	static constexpr int LIGHT_RED = 16;

	static constexpr int MOTOR_STOP = 15;
	static constexpr int MOTOR_SLOW = 14;
	static constexpr int MOTOR_LEFT = 13;
	static constexpr int MOTOR_RIGHT = 12;

	uintptr_t gpio1;        /// Pointer to initialized GPIO1
	uintptr_t gpio2;
public:
	static HAL& getHAL();

	void greenLightOn();
	void greenLightOff();
	void greenLightBlinking(bool flag);


	void yellowLightOn();
	void yellowLightOff();

	void redLightOn();
	void redLightOff();
	void redLightBlinkingOn();

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
