#ifndef SRC_MESSAGE_H_
#define SRC_MESSAGE_H_

#include "enums.h"
#include "Logger.h"
#include "timerinfo.h"
#include "CalibrationValues.h"
#include <cstdint>
#include <cstring>
#include <time.h>

#ifndef MSG_PAYLOAD_SIZE
#define MSG_PAYLOAD_SIZE	(32)
#endif

struct Message
{
	Message() {}


	/**
	 * @brief Reconstruct Message from bytes
	 *
	 * @param bytes to reconstruct
	 */
	Message(char *bytes)
	{
		/* TODO: test! */
		/* copy sizeof(Message) bytes starting at start address of this message */
		memcpy(this, (const void *) bytes, sizeof(Message));
	}

	/**
	 * @brief Constructor for messages without payload.
	 * 		  Payload will be initialized to {0}
	 *
	 * @param signal 	Signal for the message
	 * @param dest	 	Destination of this message
	 */
	Message(Signal signal, Component dest)
			: signal(signal), dest(dest), payload
			{ 0 }
	{
	}

	/**
	 * @brief Constructor for messages without payload.
	 * 		  Payload will be initialized to {0}
	 *
	 * @param signal 	Signal for the message
	 * @param dest	 	Destination of this message
	 * @param src 		Source of this message
	 */
	Message(Signal signal, Component dest, Component src)
			: signal(signal), dest(dest), src(src), payload
			{ 0 }
	{
	}

	/**
	 * @brief Constructor for messages without payload.
	 * 		  Payload will be initialized to {0}
	 *
	 * @param signal 	Signal for the message
	 * @param dest	 	Destination of this message
	 * @param src 		Source of this message
<<<<<<< HEAD
	 * @param pukType   pukType of this message
	 */
	Message(Signal signal, Component dest, Component src, PUKType pukType)
			: signal(signal), dest(dest), src(src), _pukdata(pukType), payload
			{ 0 }
	{
	}

	/**
	 * @brief Constructor for messages without payload.
	 * 		  Payload will be initialized to {0}
	 *
	 * @param signal 	Signal for the message
	 * @param dest	 	Destination of this message
	 * @param src 		Source of this message
	 */
	Message(Signal signal, Component dest, Component src, char *payload)
			: signal(signal), dest(dest), src(src), payload
			{ 0 }
	{
		memcpy(this->payload, (const void *) payload, sizeof(Message));
	}
	//-------------------------------------------------------------------------------------------
	/**
	 * @brief Constructor for messages without payload and timestamp.
	 * 		  Payload will be initialized to {0}
	 *
	 * @param signal 	Signal for the message
	 * @param dest	 	Destination of this message
	 * @param src 		Source of this message
	 */
	Message(Signal signal, Component dest, Component src, int timestamp)
			: signal(signal), dest(dest), src(src), timestamp(timestamp), payload
			{ 0 }
	{
	}
	//-------------------------------------------------------------------------------------------

	/**
	 * @brief Constructor for messages with payload
	 *
	 * @param signal 	Signal for the message
	 * @param dest	 	Destination of this message
	 * @param payload	Payload of the message
	 */
	Message(Signal signal, Component dest, char *payload)
			: signal(signal), dest(dest)
	{
		memcpy(this->payload, (const void *) payload, sizeof(Message));
	}

	/**
	 * @brief Constructor for messages with payload for Timer
	 * @param signal 	Signal for the message
	 * @param payload	ID of the Timer
	 */
	Message(Signal signal, const char *payload, size_t len)
			: signal(signal), dest(Component::NULL_COMPONENT), src(Component::NULL_COMPONENT),
			  timestamp{0}, _pukdata{PUKType::TYPENULL}
	{
		if (len > sizeof(this->payload)) {
			LOG_WRITE("ERROR: Payload overflow!");
		}
		memcpy(this->payload, (const void *) payload, sizeof(this->payload));
	}

//    Message(Signal signal, Component dest, Component src)
//    : _timerinfo(Timerinfo(0,0,0))
//    {
//        this->signal = signal;
//        this->dest = dest;
//        this->src = src;
//        this->_pukdata = PUKType::TYPENULL;
////            this->_timerinfo = Timerinfo(0,0,0);
//        this->_idinfo = -1;
//    }
	Message(Signal signal, Component dest, Component src, Timerinfo info)
	: signal(signal), dest(dest), src(src), timestamp{0}, _pukdata{PUKType::TYPENULL}, _timerinfo(info){

	}

	Message(Signal signal, Component dest, Component src, int id, PUKType pukType)
	: signal(signal), dest(dest), src(src), timestamp{0}, _pukdata(pukType), id_info(id) {
	}

	/**
	 * @brief Constructor for messages with calibrationValues.
	 *
	 * @param signal 	Signal for the message
	 * @param dest	 	Destination of this message
	 * @param src 		Source of this message
	 * @param cv	    calibrationValues
	 */
	Message(Signal signal, Component dest, Component src, CalibrationValues cv)
			: signal(signal), dest(dest), src(src), calibrationValues(cv)
	{
	}

	//-------------------------------------------------------------------------------------------
	/**
	 * @brief Constructor for messages without payload and timestamp.
	 * 		  Payload will be initialized to {0}
	 *
	 * @param signal 	Signal for the message
	 * @param dest	 	Destination of this message
	 * @param src 		Source of this message
	 */
	Message(Signal signal, Component dest, Component src, timespec timespec)
			: signal(signal), dest(dest), src(src), _timespec(timespec)
	{
	}

	/**
	 * @brief Signal for the message
	 */
	Signal signal;

	/**
	 * @brief Destination of the message
	 */
	Component dest;

	/**
	 * @brief source of the message
	 */
	Component src;

	int timestamp;

	struct timespec _timespec;
	/**
	 * @brief Payload of the message
	 */
	CalibrationValues calibrationValues;
	PUKType _pukdata;
	char payload[MSG_PAYLOAD_SIZE];
	int id_info;
	Timerinfo _timerinfo;


};

#endif /* SRC_MESSAGE_H_ */
