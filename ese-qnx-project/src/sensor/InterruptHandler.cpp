/**
 * Tutorium ESEP - WS18
 *
 * Author: Moritz Höwer (Moritz.Hoewer@haw-hamburg.de)
 */
#include "../sensor/InterruptHandler.h"

#include <sys/neutrino.h> // for message channel

#include <stdexcept> // for the exceptions
#include <cstring> // for std::strerror
#include <cerrno> // for errno
#include <string> // for std::string
#include "../sensor/GPIO.h"

using namespace std::string_literals;

/**
 * Constructs an InterrupHandler.
 *
 * @param gpio - the GPIO module to be used as interrupt source
 * @param code - the code that will be transferred in the pulse messages
 */
InterruptHandler::InterruptHandler(GPIO* gpio, int intrVecNum) :
		gpio(gpio), channelID(-1), connectionID(-1), pulseMessageCode(
		_PULSE_CODE_MINAVAIL), interruptHandlerID(-1), intrVecNum(intrVecNum), event { } {
			
	channelID = ChannelCreate(0); //create a channel to receive interrupts(pulse messages) on
	if (channelID < 0) {
		throw std::logic_error(
				"Failed to create channel - "s + std::strerror(errno));
	}
	connectionID = ConnectAttach(0, 0, channelID, 0, 0); //create a connection to the channel
	if (connectionID < 0) {
		ChannelDestroy(channelID); //cleanup channel
		throw std::logic_error(
				"Failed to connect to channel - "s + std::strerror(errno));
	}
	
	//initialize an event of type sigevent this event can be attached to an interrupt, when an
	//interrupt occurs the kernel will send a pulse message on the channel (identified by 
	//connectionID) with the pulse message code chosen(pulseMessageCode, valid range is 
	//_PULSE_CODE_MINAVAIL to _PULSE_CODE_MAXAVAIL). Use SIGEV_PULSE_PRIO_INHERIT if you
	//want the thread that receives the pulse message to inherit the priority of its process(?)
	SIGEV_PULSE_INIT(&event, connectionID, SIGEV_PULSE_PRIO_INHERIT,  
			pulseMessageCode, 0);									 
																	 
}

/**
 * Constructs an InterrupHandler.
 *
 * @param gpio - the GPIO module to be used as interrupt source
 * @param code - the code that will be transferred in the pulse messages
 */
InterruptHandler::InterruptHandler(int intrVecNum) :
		gpio(NULL), channelID(-1), connectionID(-1), pulseMessageCode(_PULSE_CODE_MINAVAIL),
		interruptHandlerID(-1), intrVecNum(intrVecNum), event { } {

	channelID = ChannelCreate(0); //create a channel to receive interrupts(pulse messages) on
	if (channelID < 0) {
		throw std::logic_error(
				"Failed to create channel - "s + std::strerror(errno));
	}
	connectionID = ConnectAttach(0, 0, channelID, 0, 0); //create a connection to the channel
	if (connectionID < 0) {
		ChannelDestroy(channelID); //cleanup channel
		throw std::logic_error(
				"Failed to connect to channel - "s + std::strerror(errno));
	}

	//initialize an event of type sigevent this event can be attached to an interrupt, when an
	//interrupt occurs the kernel will send a pulse message on the channel (identified by
	//connectionID) with the pulse message code chosen(pulseMessageCode, valid range is
	//_PULSE_CODE_MINAVAIL to _PULSE_CODE_MAXAVAIL). Use SIGEV_PULSE_PRIO_INHERIT if you
	//want the thread that receives the pulse message to inherit the priority of its process(?)
	SIGEV_PULSE_INIT(&event, connectionID, SIGEV_PULSE_PRIO_INHERIT,
			pulseMessageCode, 0);

}

/**
 * Destruct a GPIO abstraction.
 * This will release the memory owned by this module.
 */
InterruptHandler::~InterruptHandler() {
	ConnectDetach(connectionID);
	ChannelDestroy(channelID);
}

void InterruptHandler::enableInterrupt(uint32_t bits) {
	//set bits in GPIO-Registers regarding Interrupts
	if(gpio != NULL) {
		gpio->enableIRQ(bits);
	}

	// request IO priveliges
	ThreadCtl( _NTO_TCTL_IO, 0 ); 

	// Attach the initialized event to the interrupt(by interrupt vector number(GPIO0_INTB))
	interruptHandlerID = InterruptAttachEvent(intrVecNum, &event, 0);
	if (interruptHandlerID == -1) {
		throw std::logic_error(
				"Failed to attach to interrupt - "s + std::strerror(errno));
	}
}

void InterruptHandler::disableInterrupt() {
	if (interruptHandlerID != -1) {
		if(gpio != NULL) {
			gpio->disableIRQ(ALL_PINS);
			gpio->clearIRQ(ALL_PINS);
		}
		ThreadCtl( _NTO_TCTL_IO, 0 );
		InterruptDetach(interruptHandlerID); 
		interruptHandlerID = -1;
	}
}

/**
 * Wait for an interrupt (ie a pulse message) to arrive.
 *
 * @return the Signal that have that results from the interrupt
 * (e.g. IS_METAL, means that Metal Detector has detected Metal)
 */
Signal InterruptHandler::awaitInterruptGPIO() {
	if(interruptHandlerID == -1) {
		throw std::logic_error("Interrupt is not enabled!");
	}

	struct _pulse pulse { };
	
	//receive pulse message from the kernel(means an interrupt occured)
	//kernel already masked the interrupt
	int result = MsgReceivePulse(channelID, &pulse, sizeof(pulse), 0);

	if (result < 0) {
		throw std::logic_error(
				"Failed to receive pulse - "s + std::strerror(errno));
	}
	if (pulse.code < 0) {
		throw std::logic_error(
				"Received an error - "s + std::to_string(pulse.code));
	}

	//check which pins have pending IRQ(read IRQSTATUS_1)
	uint32_t pinsAffected = gpio->getIRQBits();

	//read the current value of the GPIO-pins(read DATA-IN)
	uint32_t currentPins = gpio->read();

	//clear IRQ by writing to IRQSTATUS_1
	gpio->clearIRQ(pinsAffected);

	unmaskInterrupt(intrVecNum);

	return determineSignal(pinsAffected, currentPins); //1..00000 pinsAffected //0..000000
}

void InterruptHandler::awaitInterruptHightSensor() {
	if(interruptHandlerID == -1) {
		throw std::logic_error("Interrupt is not enabled!");
	}

	struct _pulse pulse { };

	//receive pulse message from the kernel(means an interrupt occured)
	//kernel already masked the interrupt
	int result = MsgReceivePulse(channelID, &pulse, sizeof(pulse), 0);

	if (result < 0) {
		throw std::logic_error(
				"Failed to receive pulse - "s + std::strerror(errno));
	}
	if (pulse.code < 0) {
		throw std::logic_error(
				"Received an error - "s + std::to_string(pulse.code));
	}
}
/**
 *  unmasks interrupt
 */
void InterruptHandler::unmaskInterrupt(int intrVecNum){
	//request IO priveliges(also needed for in32, out32 etc..??? stated in qnx guide)
	ThreadCtl( _NTO_TCTL_IO, 0 );

	//unmask the interrupt for and because we have handled it(intr=interrupt vector number)
	InterruptUnmask(intrVecNum, interruptHandlerID);
}

/**
	Determines the pin that triggered an Interrupt and if it was triggered by a rising or a falling Edge,
	this Information is returned in form of an enum.
	@param affectedPins - the pins that were affected by the Interrupt
	@param currentPins - the state of the pins when the Interrupt was triggered
	
	@return the Signal that results from the Interrupt
*/
Signal InterruptHandler::determineSignal(const uint32_t affectedPins, const uint32_t currentPins) {
	Signal res;
	//The value of the pins that have been affected by the Interrupt, 
	//if 0 falling edge, if 1 rising edge(we are only edge sensitive)
	uint32_t affectedPinValue = affectedPins & currentPins; 
	switch(affectedPins) {
		case LB_ENTRY : //GPIO0 bit 2 low active
			//if falling edge
			if(affectedPinValue == 0) {
				res = Signal::LB_ENTRY_ENTERED;
			}
			//if rising edge
			else {
				res = Signal::LB_ENTRY_LEFT;
			}
			break;
		case LB_HIGHT : //GPIO0 bit 3 low active
			if(affectedPinValue == 0) {
				res = Signal::LB_HIGHT_ENTERED;
			}
			else {
				res = Signal::LB_HIGHT_LEFT;
			}
			break;
		case HIGHT_OK : //GPIO0 bit 4 high active
			if(affectedPinValue == 0) {
				res = Signal::HIGHT_OK;
			}
			else {
				res = Signal::HIGHT_NOT_OK;
			}
			break;
		case LB_GATE : //GPIO0 bit 5 low active
			if(affectedPinValue == 0) {
				res = Signal::LB_METAL_ENTERED;
			}
			else {
				res = Signal::LB_METAL_LEFT;
			}
			break;
		case METAL_DETECTOR : //GPIO0 bit 7 high active
			if(affectedPinValue == 0) {
				res = Signal::NOT_METAL;
			}
			else {
				res = Signal::IS_METAL;
			}
			break;
		case LB_SLIDE : //GPIO0 bit 15 low active
			if(affectedPinValue == 0) {
				res = Signal::LB_SLIDE_ENTERED;
			}
			else {
				res = Signal::LB_SLIDE_LEFT;
			}
			break;
		case LB_EXIT : //GPIO0 bit 20 low active
			if(affectedPinValue == 0) {
				res = Signal::LB_EXIT_ENTERED;
			}
			else {
				res = Signal::LB_EXIT_LEFT;
			}
			break;
		
		case BTN_START : //GPIO0 bit 22 high active
			if(affectedPinValue == 0) {
				res = Signal::BTN_START_RELEASED;
			}
			else {
				res = Signal::BTN_START_PRESSED;
			}
			break;
		case BTN_STOP : //GPIO0 bit 23 low active
			if(affectedPinValue == 0) {
				res = Signal::BTN_STOP_PRESSED;
			}
			else {
				res = Signal::BTN_STOP_RELEASED;
			}
			break;
		case BTN_RESET : //GPIO0 bit 26 high active	
			if(affectedPinValue == 0) {
				res = Signal::BTN_RESET_RELEASED;
			}
			else {
				res = Signal::BTN_RESET_PRESSED;
			}
			break;
		case E_STOP : //GPIO0 bit 27 high active
			if(affectedPinValue == 0) {
				res = Signal::ESTOP_PRESSED;
			}
			else {
				res = Signal::ESTOP_RELEASED;
			}
			break;
		default:
			res = Signal::NOT_DETERMINABLE;
			break;
	}	
	return res;
}
