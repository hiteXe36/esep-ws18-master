/**
 * Tutorium ESEP - WS18
 *
 * Author: Moritz Höwer (Moritz.Hoewer@haw-hamburg.de)
 */

#ifndef GPIO_H
#define GPIO_H

#include <cstdint>

static constexpr uint64_t GPIO0_BASE = 0x44E07000;
// spruhl73.pdf - Page 180 - 183
static constexpr size_t GPIO_SIZE = 0x1000;
// spruhl73.pdf - Page 467
static constexpr int GPIO0_INTB = 97; //(interrupt vector number)
// spruhl73.pdf - Page 4877

//REGISTER
static constexpr int IRQSTATUS_1 = 0x30;
static constexpr int IRQSTATUS_SET_1 = 0x38;
static constexpr int IRQSTATUS_CLR_1 = 0x40;
static constexpr int OE = 0x134;
static constexpr int DATAIN = 0x138;
static constexpr int LEVELDETECT0 = 0x140;
static constexpr int LEVELDETECT1 = 0x144;
static constexpr int RISINGDETECT = 0x148;
static constexpr int FALLINGDETECT = 0x14C;
static constexpr int DEBOUNCENABLE = 0x150;
static constexpr int DEBOUNCINGTIME = 0x154;
static constexpr int CLEARDATAOUT = 0x190;
static constexpr int SETDATAOUT = 0x194;

//GPIO0 BITS
static constexpr int GPIO_NUMBER_OF_BITS = 32;
static constexpr int LB_ENTRY = 1 << 2;
static constexpr int LB_HIGHT = 1 << 3;
static constexpr int HIGHT_OK = 1 << 4;
static constexpr int LB_GATE = 1 << 5;
static constexpr int METAL_DETECTOR = 1 << 7;
static constexpr int LB_SLIDE = 1 << 15;
static constexpr int LB_EXIT = 1 << 20;
static constexpr int BTN_START = 1 << 22;
static constexpr int BTN_STOP = 1 << 23;
static constexpr int BTN_RESET = 1 << 26;
static constexpr int E_STOP = 1 << 27;
static constexpr int PINS_FOR_IRQ_GENERATION = LB_ENTRY | LB_HIGHT | HIGHT_OK | LB_GATE | METAL_DETECTOR |
											   LB_SLIDE | LB_EXIT | BTN_START | BTN_STOP | BTN_RESET | E_STOP;

static constexpr uint32_t ALL_PINS = 0xFFFFFFFF;

/**
 * Abstraction for a GPIO Bank.
 * @author Moritz Höwer (Moritz.Hoewer@haw-hamburg.de)
 */
class GPIO {
private:
  uintptr_t _baseAddress;

public:
  GPIO(const uint64_t baseAddress);
  GPIO(const GPIO&) = delete; // prevent copy
  GPIO(GPIO&& other) = delete; // prevent move
  virtual ~GPIO();

  GPIO& operator= (const GPIO&) = delete; // prevent copy
  GPIO& operator= (GPIO&& other) = delete; // prevent move

  void setInput(uint32_t pins);
  void setOutput(uint32_t pins);

  void setBits(uint32_t bits);
  void clearBits(uint32_t bits);

  void enableIRQ(uint32_t bits);
  void disableIRQ(uint32_t bits);

  uint32_t getIRQBits();
  void clearIRQ(uint32_t bits);

  uint32_t read();

private:
  void unmapMemory();
  void writeRegister(int offset, uint32_t value);
  uint32_t readRegister(int offset);
};

#endif /* GPIO_H */
