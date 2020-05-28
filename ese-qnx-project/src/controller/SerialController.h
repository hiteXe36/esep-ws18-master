#ifndef __DUMMY_TEST

#ifndef SRC_SERIALCONTROLLER_H_
#define SRC_SERIALCONTROLLER_H_

#include "../serial/Serial.h"
#include "../servicetools/Channel.h"
#include "../servicetools/Message.h"
#include <memory>

/**
 * @brief Controller for SerialReader and SerialWriter
 */
class SerialController
{
private:
	Channel& disp2SerialController;				/// Channel for communication
	Channel& serialController2Disp;

public:
	/**
	 * @brief Constructor for SerialController
	 *
	 * @param channel Channel for communication
	 * @param serial Serial connection to use
	 */
	explicit SerialController(Channel& serialC2Disp, Channel& disp2SerialC);

	/**
	 * @brief Destructor for SerialController
	 */
	virtual ~SerialController();

	/**
	 * @brief overload () operator
	 */
	void operator()();
	volatile bool isRunning
	{ true };		/// Running status flag

};

#endif /* SRC_SERIALCONTROLLER_H_ */
#endif
