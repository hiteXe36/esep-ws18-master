/*
 * Serial.h
 * Hier wird eine Leitung für die Übertragung gebildet.
 * Konfiguration der Leitung.
 *  Created on: 17.10.2018
 *      Author: ace146
 */
#ifndef __DUMMY_TEST
#ifndef SRC_SERIAL_H_
#define SRC_SERIAL_H_

#include <string>
#include <cstdint>

class Serial
{
private:
	int fdesc_; // File Descriptor

public:
	Serial(const char *dev);
	~Serial();
	void init(int baud);
	int send(char* buffer, unsigned char numBytes);
	int recv(char* p, size_t length);
};

#endif /* SRC_SERIAL_H_ */
#endif
