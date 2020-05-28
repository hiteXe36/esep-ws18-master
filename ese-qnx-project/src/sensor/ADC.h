/**
 * Tutorium ESEP - WS18
 *
 * Author: Moritz Höwer (Moritz.Hoewer@haw-hamburg.de)
 */
#ifndef HAL_ADC_H_
#define HAL_ADC_H_

#include <mutex>
#include "../sensor/InterruptHandler.h"
#include "../sensor/lib/TSCADC.h"

class ADC {
private:
	TSCADC tscadc;
	InterruptHandler interruptHandler;
	std::mutex mtxAccess;

public:
	ADC();
	ADC(const ADC&) = delete; // prevent copy
	ADC(ADC&&) = delete; // prevent move
	virtual ~ADC();

	uint16_t read();

	ADC& operator=(const ADC&) = delete; // prevent copy
	ADC& operator=(ADC&& other) = delete; // prevent move

private:
	void init();
	void stepConfigure(unsigned int stepSel, Fifo fifo,
			PositiveInput positiveInpChannel, AverageSamples average);

	void enableInterrupt();
	void disableInterrupt();

	void cleanUpInterrupts();

	void startConversion();
};

#endif /* HAL_ADC_H_ */
/** @} */
