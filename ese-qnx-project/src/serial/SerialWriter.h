#ifndef __DUMMY_TEST

#ifndef SRC_SERIALWRITER_H_
#define SRC_SERIALWRITER_H_

#include "../servicetools/Channel.h"
#include "Serial.h"
#include "../servicetools/Message.h"

/**
 * @brief SerialWriter is a Functor that writes to the serial connection
 */
class SerialWriter
{
public:
	/**
	 * @brief Constructor for SerialWriter
	 *
	 * @param channel Channel for communication
	 * @param serial Serial connection to use
	 */
	SerialWriter(Channel& channel, Serial& serial);

	/**
	 * @brief Destructor for SerialWriter
	 */
	virtual ~SerialWriter();

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

#endif /* SRC_SERIALWRITER_H_ */
#endif
