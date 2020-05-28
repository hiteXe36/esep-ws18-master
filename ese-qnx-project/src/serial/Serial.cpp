#ifndef __DUMMY_TEST

#include "Serial.h"
#include <termios.h>

#include <iostream>
#include <fcntl.h> //open()
#include <unistd.h> //close()
#include <cstring> // for std::strerror
#include <cerrno> // for errno

using namespace std::string_literals;

Serial::Serial(const char *dev)
{	// /dev/ser4
	// Open File descriptor
	fdesc_ = open(dev, O_RDWR);
	if (fdesc_ < 0)
	{
		throw std::logic_error("Failed to open "s + dev + " - "
				+ std::strerror(errno));
	}
	init (B19200); // default baud rate
}

Serial::~Serial()
{
	// Close File descriptor
	if (close(fdesc_) < 0)
	{
		throw std::logic_error("Failed to close serial "s
				+ std::strerror(errno));
	}
}

void Serial::init(int baud)
{
	struct termios ts; //Configuration of the serial connection
	tcflush(fdesc_, TCIOFLUSH); //empty the connection line for I/O
	tcgetattr(fdesc_, &ts); // save the attributes ot fdesc in the ts-Struct
	cfsetispeed(&ts, baud);
	cfsetospeed(&ts, baud);
	ts.c_cflag &= ~CSIZE;
	ts.c_cflag &= ~CSTOPB; /* one stop bit */
	ts.c_cflag &= ~PARENB; /* no parity bit */
	ts.c_cflag |= CS8; /* character size (8-bit) */
	ts.c_lflag &= ~ICANON; /* noncanonical mode */
	ts.c_cflag |= CREAD; /* enable receiving characters */
	ts.c_cflag |= CLOCAL; /* disable modem control */
	tcsetattr(fdesc_, TCSANOW, &ts); //makes changes immediately
}

int Serial::send(char* buffer, unsigned char numBytes)
{
	int res = write(fdesc_, buffer, numBytes);
	if (res < 0)
	{
		throw std::logic_error("Failed to write "s + std::strerror(errno));
	}
	return res;
}

int Serial::recv(char* p, size_t length)
{
	size_t received = 0;
	do
	{
		// return with up to sizeof(char) bytes when 10000 * 0.1 has expired or sizeof(char) bytes available
		int res = read(fdesc_, p + received, sizeof(char));
		if (res == 0)
		{
			break;
		}
		else if (res < 0)
		{
			throw std::logic_error("Failed to read "s + std::strerror(errno));
		}
		else
		{
			received += res;
		}
	}
	while (received != length);
	return received;
}

#endif
