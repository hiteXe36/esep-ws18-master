#ifndef __DUMMY_TEST

#ifndef SRC_SERIALREADER_H_
#define SRC_SERIALREADER_H_

#include "../servicetools/Channel.h"
#include "Serial.h"
#include "../servicetools/Message.h"

/**
 * @brief SerialReader is a Functor that reads messages from the serial connection
 */
class SerialReader
{
public:
	/**
	 * @brief Constructor for SerialReader
	 *
	 * @param channel Channel for communication
	 * @param serial Serial connection to use
	 */
	SerialReader(Channel& channel, Serial& serial);

	/**
	 * @brief Destructor for SerialReader
	 */
	virtual ~SerialReader();

	/**
	 * @brief overload () operator
	 */
	void operator()();
private:
	Channel& channel_;				/// Channel for communication
	Serial serial_;					/// Serial connection to use
	char buffer_[sizeof(Message)];	/// Temporary to save received message
	volatile bool isRunning;		/// Running status flag
};

#endif /* SRC_SERIALREADER_H_ */
#endif
