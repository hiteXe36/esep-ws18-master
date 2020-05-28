#ifndef SRC_ENUMS_H_
#define SRC_ENUMS_H_

#include <string>
#include "../servicetools/Logger.h"

/**
 * @brief Components used in message passing
 */
enum class Component
{
	NULL_COMPONENT,         /**< ONLY use for testing! */
	MAIN,
	HAL,
	SERIAL,
	OTHER_MACHINE,
	REMOTE,
	DISPATCHER,
	LAMP,
	MOTOR,
	GATE,
	EMBEDDED_RECORDER,
	SENSOR,
	HIGHT,
	TIMER,
	TIMER_CONTROL,
	CALIBRATION,
};

/**
 * @brief Signals used in message passing
 */
enum class Signal
{
	/*****************
	 *    ACTORS
	 ****************/

	/* Light signals */
	GREEN_LIGHT_ON,			//00
	GREEN_LIGHT_OFF,		//01
	GREEN_LIGHT_BLINK_SLOW, //02
	GREEN_LIGHT_BLINK_FAST, //03
	GREEN_LIGHT_BLINK_OFF,	//04

	YELLOW_LIGHT_ON,		//05
	YELLOW_LIGHT_OFF,		//06
	YELLOW_LIGHT_BLINK_SLOW,//07
	YELLOW_LIGHT_BLINK_FAST,//08
	YELLOW_LIGHT_BLINK_OFF,	//09

	RED_LIGHT_ON,			//10
	RED_LIGHT_OFF,			//11
	RED_LIGHT_BLINK_SLOW,	//12
	RED_LIGHT_BLINK_FAST,	//13
	RED_LIGHT_BLINK_OFF, 	//14

	/* Motor signals */
	MOTOR_ON,				//15
	MOTOR_OFF,				//16
	MOTOR_SLOW,				//17
	MOTOR_FAST,				//18
	MOTOR_LEFT,				//19
	MOTOR_RIGHT,			//20
	MOTOR_STOP,				//21

	/* Gate signals */
	GATE_OPEN,				//22
	GATE_CLOSE,				//23


	ACKNOWLEDGED, /**< Response from *Controllers when action done */
	NULL_SIGNAL, /**< ONLY use for testing! */
	TIMED_OUT,	/**< Timer Signal for Timed out */

	/* Sensor Signals */
	LB_ENTRY_ENTERED, //first lightbarrier has been interrupted(object is at the lightbarrier) //23
	LB_ENTRY_LEFT, //object left the first lightbarrier

	LB_HIGHT_ENTERED, //lightbarrier at the hightsensor has been interrupted(object is at the lightbarrier)
	LB_HIGHT_LEFT, //object left the lightbarrier at the hightsensor

	LB_METAL_ENTERED, //lightbarrier at the gate has been interrupted(object is at the lightbarrier)
	LB_METAL_LEFT, //object left the lightbarrier at the gate

	LB_SLIDE_ENTERED, //lightbarrier at the gate has been interrupted(object is at the lightbarrier)
	LB_SLIDE_LEFT, //object left the lightbarrier at the slide

	LB_EXIT_ENTERED, //last lightbarrier has been interrupted(object is at the lightbarrier)
	LB_EXIT_LEFT, //object left the lightbarrier at the slide

	HIGHT_OK, //the hight is OK(means not to high)
	HIGHT_NOT_OK, //the hight is too high

	IS_METAL, //object contains metal
	NOT_METAL, //object doesnt contain metal

	BTN_START_PRESSED, //start button has been pressed
	BTN_START_RELEASED, //start button has been released

	BTN_STOP_PRESSED, //stop button has been pressed
	BTN_STOP_RELEASED, //stop button has been released

	BTN_RESET_PRESSED, //reset button has been pressed
	BTN_RESET_RELEASED, //reset button has been released

	ESTOP_PRESSED, //estop has been pressed
	ESTOP_RELEASED, //estop has been released

	NOT_DETERMINABLE,
	REMOTE_CONNECTED,
	REMOTE_NOT_CONNECTED,

	START_HIGHT_MEASUREMENT,
	WITH_DRILLING_PUCK,
	FLAT_PUCK,
	HIGH_PUCK,
	CODE1_PUCK,
	CODE2_PUCK,
	CODE4_PUCK,
	CODE5_PUCK,
	NOT_DETERMINABLE_PUCK,
	WRONGPUK,
	WRONGSIGNAL,

	GETPUKDATA,
	PUKDATA,
	PASSINGCONFIRMED,


    STARTTIMER,
    STOPALLTIMERS,
    STOPTIMER,
    PAUSEALLTIMERS,
    CONTINUEALLTIMERS,
	MACHINE2BUSY,
	MACHINE2IDLE,
	SLIDE2FULL,
	SLIDE2EMPTY,
	REIHUNG,
	STARTCALIBRATION,
	STOPCALIBRATION,
	CALIBRATIONABORTED, //TODO unused?
	CALIBRATIONVALUES,

	CALIBRATIONSIGNAL_REQ,
	CALIBRATIONSIGNAL_ACK,
	CALIBRATIONSIGNAL_FIN,


};

enum PUKType
{
    TYPE1 = 1,
    TYPE2_1 = 21, //type 2_1 and 3_1 are the same for the hightController which one should be sent to mainController?
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

enum State{
	IN_ENTRY,
	IN_HEIGHT,
	SLIDING,
	IN_GATE,
	WAIT4BAND2,
	PASSING,
	READY,
	ENDSTATE,
	ENDSTATE2,
	IDLE,
	SLIDINGDONE,
	ANSTEHENDUNQUITIERT,
	ANSTEHENDQUITIERT,
	AWAKEN,
};

class Enums {
public:
	static std::string componentToString(Component c);
	static std::string signalToString(Signal s);
	static std::string pukTypeToString(PUKType pukType);
	static std::string stateToString(State s);
	static Component convertStringToComponent(std::string ss);
	static Signal convertStringToSignal(std::string ss);
};

enum class HightFSMState
{
	CALIBRATION,
	READY,
	FIRST_EDGE,
	SECOND_EDGE,
	THIRD_EDGE,
	FOURTH_EDGE,
	FIFTH_EDGE,
	SIXTH_EDGE,
	FLAT_SURFACE_PUCK,
	WITH_DRILLING_PUCK,
	CODED_PUCK,
	ERROR
};





#endif
