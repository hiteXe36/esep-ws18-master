/**
 * Tutorium ESEP - WS18
 *
 * Author: Moritz Höwer (Moritz.Hoewer@haw-hamburg.de)
 */
#include "../sensor/GPIO.h"

#include <sys/neutrino.h>
#include <sys/mman.h>
#include <hw/inout.h>
#include <stdexcept> // for the exceptions
#include <cstring> // for std::strerror
#include <cerrno> // for errno
#include <string> // for std::string



/**
 *
 * @param baseAddress - the base address of the GPIO module in memory
 * @throw std::invalid_argument if mapping fails due to illegal base address
 */
GPIO::GPIO(const uint64_t baseAddress) :
		_baseAddress(MAP_DEVICE_FAILED) {
	_baseAddress = mmap_device_io(GPIO_SIZE, baseAddress);
	if (_baseAddress == MAP_DEVICE_FAILED) {
		throw std::invalid_argument(
				"Failed to map IO-memory - "
						+ std::string(std::strerror(errno)));
	}

}

/**
 * Destruct a GPIO abstraction.
 * This will release the memory owned by this module.
 */
GPIO::~GPIO() {
	unmapMemory();
}

/**
 * Configure pins for input.
 *
 * @param pins - the pins to configure as input
 */
void GPIO::setInput(uint32_t pins) {
	uint32_t currentConfig = readRegister(OE);
	writeRegister(OE, currentConfig | pins);
}

/**
 * Configure pins for output.
 *
 * @param pins - the pins to configure as output
 */
void GPIO::setOutput(uint32_t pins) {
	uint32_t currentConfig = readRegister(OE);
	writeRegister(OE, currentConfig & ~pins);
}

/**
 * Set multiple bits.
 *
 * @param bits - the bits to set
 */
void GPIO::setBits(uint32_t bits) {
	writeRegister(SETDATAOUT, bits);
}

/**
 * Clear multiple bits.
 *
 * @param bits - the bits to clear
 */
void GPIO::clearBits(uint32_t bits) {
	writeRegister(CLEARDATAOUT, bits);
}

/**
 * Read all bits of the GPIO bank.
 *
 * @return the value of all bits of the GPIO bank
 * @throw std::logic_error if mapped memory is bad.
 */
uint32_t GPIO::read() {
	return readRegister(DATAIN);
}


/**
 * Enable interrupt request for selected bits.
 * This will set up edge detection for rising and falling flanks.
 *
 * @param bits - the bits to enable interrupt for
 */
void GPIO::enableIRQ(uint32_t bits) {
	//	ThreadCtl( _NTO_TCTL_IO, 0 );
	//enable interrupt generation
	writeRegister(IRQSTATUS_SET_1, readRegister(IRQSTATUS_SET_1) | bits);

	//for falling and rising edge
	writeRegister(RISINGDETECT, readRegister(RISINGDETECT) | bits);
	writeRegister(FALLINGDETECT, readRegister(FALLINGDETECT) | bits);

	//make sure leveldetection is turned off
	writeRegister(LEVELDETECT0, readRegister(LEVELDETECT0) & ~bits);
	writeRegister(LEVELDETECT1, readRegister(LEVELDETECT1) & ~bits);

	//enable debouncing
	writeRegister(DEBOUNCENABLE, readRegister(DEBOUNCENABLE) | bits);
	writeRegister(DEBOUNCINGTIME, 255);
}

/**
 * Disable interrupt request for selected bits.
 * This will also remove edge detection for rising and falling flanks.
 *
 * @param bits - the bits to disable interrupt for
 */
void GPIO::disableIRQ(uint32_t bits) {
//	ThreadCtl( _NTO_TCTL_IO, 0 );
	//disable Interrupt generation
	writeRegister(IRQSTATUS_CLR_1, readRegister(IRQSTATUS_CLR_1) | bits);
	
	//disable detection for falling and rising edges
	writeRegister(RISINGDETECT, readRegister(RISINGDETECT) & ~bits);
	writeRegister(FALLINGDETECT, readRegister(FALLINGDETECT) & ~bits);
	
	//make sure leveldetection is turned off
	writeRegister(LEVELDETECT0, readRegister(LEVELDETECT0) & ~bits);
	writeRegister(LEVELDETECT1, readRegister(LEVELDETECT1) & ~bits);

	//disable debouncing
	writeRegister(DEBOUNCENABLE, readRegister(DEBOUNCENABLE) & ~bits);
}

/**
 * Check which bits have triggered the interrupt request.
 *
 * @return the bits that have pending IRQs
 */
uint32_t GPIO::getIRQBits() {
	return readRegister(IRQSTATUS_1);
}

/**
 * Clear the interrupt request for selected bits.
 *
 * @param bits - the bits to clear the IRQ for
 */
void GPIO::clearIRQ(uint32_t bits) {
	writeRegister(IRQSTATUS_1, bits);
}

/**
 * Unmaps the memory if it was mapped.
 */
void GPIO::unmapMemory() {
	if (_baseAddress != MAP_DEVICE_FAILED) {
		munmap_device_io(_baseAddress, GPIO_SIZE);
		_baseAddress = MAP_DEVICE_FAILED;
	}
}

/**
 * Writes to a specific register.
 *
 * @param offset - the offset of the register
 * @param value - the value to set
 */
void GPIO::writeRegister(int offset, uint32_t value) {
	out32(_baseAddress + offset, value);
}

/**
 * Reads a specific register.
 *
 * @param offset - the offset of the register
 * @return the value of the register
 */
uint32_t GPIO::readRegister(int offset) {
	return in32(_baseAddress + offset);
}
