#ifndef __DUMMY_TEST
#include "SerialReader.h"
#include "../servicetools/Logger.h"

#include <cstring> // for std::strerror
#include <cerrno> // for errno

SerialReader::SerialReader(Channel& channel, Serial& serial)
		: channel_(channel), serial_(serial)
{
}

void SerialReader::operator()()
{
	LOG_WRITE("Started SerialReader thread");
	while (isRunning)
	{
		LOG_WRITE("Waiting for bytes from serial line");
		int res = serial_.recv(buffer_, sizeof(Message));
		LOG_WRITE("Got bytes from serial line");
		if (res != sizeof(Message))
		{
			throw std::logic_error("The length of receive Messages smaller than sizeof(Message)");
		}
		auto msg = std::make_unique < Message > (static_cast<Message>(buffer_));
		std::string ausgabe;
		ausgabe = "Converted bytes from serial to message! Content:";
		ausgabe += "Signal: " + Enums::signalToString(msg->signal) + "\n "
				+ "Destination: " + Enums::componentToString(msg->dest) + "\n "
				+ "Source: " + Enums::componentToString(msg->src);
		LOG_WRITE(ausgabe);

		msg->src = Component::OTHER_MACHINE;
		msg->dest = Component::MAIN;

		ausgabe = "Changed value of src and dest of the message! Content:";
		ausgabe += "Signal: " + Enums::signalToString(msg->signal) + "\n "
				+ "Destination: " + Enums::componentToString(msg->dest) + "\n "
				+ "Source: " + Enums::componentToString(msg->src);
		LOG_WRITE(ausgabe);
		channel_.enqueue(*msg);
		LOG_WRITE("Sent message to SerialController");
	}
}

SerialReader::~SerialReader()
{

}
#endif
