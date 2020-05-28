/**
 * Tutorium ESEP - WS18
 *
 * Author: Moritz Höwer (Moritz.Hoewer@haw-hamburg.de)
 */

#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

#include <cstdint>
#include <sys/siginfo.h>
#include "../servicetools/enums.h"
class GPIO;

/**
 * Abstraction for an interrupt handler
 * @author Moritz Höwer (Moritz.Hoewer@haw-hamburg.de)
 */
class InterruptHandler {
private:
  GPIO* gpio;
  int channelID;
  int connectionID;
  int8_t pulseMessageCode;
  int interruptHandlerID;
  int intrVecNum;
  struct sigevent event;

  Signal determineSignal(const uint32_t affectedPins, uint32_t currentPins);
public:
  InterruptHandler(const int intVecNum);
  InterruptHandler(GPIO* gpio, const int intrVecNum);
  InterruptHandler(const InterruptHandler&) = delete; // prevent copy
  InterruptHandler(InterruptHandler&& other) = delete; // prevent move
  virtual ~InterruptHandler();

  void enableInterrupt(uint32_t bits);
  void disableInterrupt();
  Signal awaitInterruptGPIO();
  void awaitInterruptHightSensor();
  void unmaskInterrupt(int intrVecNum);

  InterruptHandler& operator= (const InterruptHandler&) = delete; // prevent copy
  InterruptHandler& operator= (InterruptHandler&& other) = delete; // prevent move
};

#endif /* INTERRUPT_HANDLER_H */
