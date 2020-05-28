#pragma once

#include <string>

/**
 * @brief Components used in message passing
 */
enum Component
{
//	NULL_COMPONENT,         /**< ONLY use for testing! */
	MAIN,
//	HAL,
//	SERIAL,
	OTHER_MACHINE,
//	REMOTE,
	DISPATCHER,
//	LAMP,
	MOTOR,
	GATE,
//	EMBEDDED_RECORDER,
//	SENSOR,
	HIGHT,
	DUMMYGENERATOR,
	TIMER
};

/**
 * @brief Signals used in message passing
 */
enum Signal
{
	/*****************
	 *    ACTORS
	 ****************/

	/* Light signals */
//	GREEN_LIGHT_ON,			//00
//	GREEN_LIGHT_OFF,		//01
//	GREEN_LIGHT_BLINK_SLOW, //02
//	GREEN_LIGHT_BLINK_FAST, //03
//	YELLOW_LIGHT_ON,		//04
//	YELLOW_LIGHT_OFF,		//05
//	YELLOW_LIGHT_BLINK_SLOW,//06
//	YELLOW_LIGHT_BLINK_FAST,//07
//	RED_LIGHT_ON,			//08
//	RED_LIGHT_OFF,			//09
//	RED_LIGHT_BLINK_SLOW,	//10
//	RED_LIGHT_BLINK_FAST,	//11

	/* Motor signals */
//	MOTOR_ON,				//12
//	MOTOR_OFF,				//13
//	MOTOR_SLOW,				//14
//	MOTOR_FAST,				//15
//	MOTOR_LEFT,				//16
	MOTOR_RIGHT = 1,			//17
	MOTOR_STOP = 2,				//18

	/* Gate signals */
	GATE_OPEN = 3,				//19
	GATE_CLOSE = 4,				//20


//	ACKNOWLEDGED, /**< Response from *Controllers when action done */
//	NULL_SIGNAL, /**< ONLY use for testing! */
//	TIMED_OUT,	/**< Timer Signal for Timed out */

	/* Sensor Signals */

	LB_ENTRY_ENTERED = 5, //first lightbarrier has been interrupted(object is at the lightbarrier) //23
	LB_ENTRY_LEFT = 6, //object left the first lightbarrier

	LB_HIGHT_ENTERED = 7, //lightbarrier at the hightsensor has been interrupted(object is at the lightbarrier)
	LB_HIGHT_LEFT = 8, //object left the lightbarrier at the hightsensor

	LB_METAL_ENTERED = 9, //lightbarrier at the gate has been interrupted(object is at the lightbarrier)
	LB_METAL_LEFT = 10, //object left the lightbarrier at the gate

	LB_SLIDE_ENTERED = 11, //lightbarrier at the gate has been interrupted(object is at the lightbarrier)
	LB_SLIDE_LEFT = 12, //object left the lightbarrier at the slide

	LB_EXIT_ENTERED = 13, //last lightbarrier has been interrupted(object is at the lightbarrier)
	LB_EXIT_LEFT = 14, //object left the lightbarrier at the slide

	MACHINE2BUSY = 15,
	MACHINE2IDLE = 16,
	PASS_CONFIRMED = 17,
	NOTIFY_MACHINE2 = 18,

	LOGIC_ERROR = 19,
	LOGIC_ERROR_RESOLVED = 20,

//	HIGHT_OK, //the hight is OK(means not to high)
//	HIGHT_NOT_OK, //the hight is too high

//	IS_METAL, //object contains metal
//	NOT_METAL, //object doesnt contain metal

	GETPUKDATA = 21,
	PUKDATA = 22,

    TIMEOUT,
    STARTTIMER,
    STOPALLTIMERS,
    CONTINUEALLTIMERS,





	BTN_START_PRESSED, //start button has been pressed
	BTN_START_RELEASED, //start button has been released

	BTN_STOP_PRESSED, //stop button has been pressed
	BTN_STOP_RELEASED, //stop button has been released

	BTN_RESET_PRESSED, //reset button has been pressed
	BTN_RESET_RELEASED, //reset button has been released

	ESTOP_PRESSED, //estop has been pressed
	ESTOP_RELEASED, //estop has been released

	RECEIVEDATA

//	NOT_DETERMINABLE,
//	REMOTE_CONNECTED,
//	REMOTE_NOT_CONNECTED,

//	START_HIGHT_MEASUREMENT,
//	WITH_DRILLING_PUCK,
//	FLAT_PUCK,
//	HIGH_PUCK,
//	CODE1_PUCK,
//	CODE2_PUCK,
//	CODE4_PUCK,
//	CODE5_PUCK,
//	NOT_DETERMINABLE_PUCK
};

//class Enums {
//public:
//	static std::string componentToString(Component c);
//	static std::string signalToString(Signal s);
//};

//enum class HightFSMState
//{
//	READY,
//	FIRST_EDGE,
//	SECOND_EDGE,
//	THIRD_EDGE,
//	FOURTH_EDGE,
//	FIFTH_EDGE,
//	SIXTH_EDGE,
//	FLAT_SURFACE_PUCK,
//	WITH_DRILLING_PUCK,
//	CODED_PUCK,
//	ERROR
//};

enum PUKType
{
    TYPE1 = 1,
    TYPE2_1 = 21,
    TYPE2_2 = 22,
    TYPE3_1 = 31,
    TYPE3_2 = 32,
    TYPE4_1 = 41,
    TYPE4_2 = 42,
    TYPE4_4 = 44,
    TYPE4_5 = 45,
    TYPEERROR = 99,
    TYPENULL = 0,
    TYPENODATA = 4711
};
