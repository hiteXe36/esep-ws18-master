/**
 * Tutorium ESEP - WS18
 *
 * Author: Moritz Höwer (Moritz.Hoewer@haw-hamburg.de)
 */
#include "../sensor/ADC.h"

#include <sys/neutrino.h>
#include <string>

// spruh73l - p. 465
static constexpr int ADC_TSC_GENINT = 16;

// configuration
static constexpr AverageSamples ADC_AVG_SAMPLES = SIXTEEN_SAMPLES_AVG;
static constexpr Fifo ADC_FIFO = Fifo::FIFO_0; 
static constexpr unsigned int ADC_STEP = 1;
static constexpr unsigned int ALL_INTERRUPTS = 0x7FF; //4+4+3 = 11 bits set

ADC::ADC() :
    tscadc(), interruptHandler(ADC_TSC_GENINT), mtxAccess() {
	init();
	enableInterrupt();
	tscadc.moduleStateSet(true);
}

ADC::~ADC() {
	disableInterrupt();
	tscadc.moduleStateSet(false);
}

void ADC::enableInterrupt() {
	cleanUpInterrupts();
	tscadc.eventInterruptEnable(END_OF_SEQUENCE_INT);
	interruptHandler.enableInterrupt(0x0); //parameter has no purpose in this case
}

void ADC::disableInterrupt() {
	tscadc.eventInterruptDisable(END_OF_SEQUENCE_INT);
	cleanUpInterrupts();

	interruptHandler.disableInterrupt();
}

void ADC::init() {
	/* Configures ADC to 3Mhz */ //adc clock is 24MHz
	tscadc.configureAFEClock(24000000, 3000000); //-> gets divided by 8

	/* General purpose inputs */
	tscadc.tsModeConfig(GENERAL_PURPOSE_MODE);

	/* Disable Touchscreen Transistors */
	tscadc.tsTransistorConfig(false);

	/* Disable data tagging */
	tscadc.stepIDTagConfig(false);

	/* Disable Write Protection of Step Configuration regs in order to do step configuration */
	tscadc.stepConfigProtectionDisable();

	/* Configure step 1 for channel 1(AN0)*/
	stepConfigure(ADC_STEP, ADC_FIFO, POSITIVE_INP_CHANNEL1, ADC_AVG_SAMPLES); //use fifo0
}

void ADC::stepConfigure(unsigned int stepSel, Fifo fifo,
		PositiveInput positiveInpChannel, AverageSamples average) {
	unsigned int step = (stepSel - 1); // compatibility to configureStepEnable

	/* Configure ADC to Single ended operation mode */
	tscadc.tsStepOperationModeControl(SINGLE_ENDED_OPER_MODE, step);

	/* Configure ADC to one shot mode */
	tscadc.tsStepModeConfig(step, ONE_SHOT_SOFTWARE_ENABLED);

	/* Configure step to select Channel, reference voltages */
	tscadc.tsStepConfig(step, NEGATIVE_REF_VSSA, positiveInpChannel,
			NEGATIVE_INP_CHANNEL1, POSITIVE_REF_VDDA);

	/* XPPSW Pin is on, Which pull up the AN0 line*/
	tscadc.tsStepAnalogSupplyConfig(true, false, false, step);

	/* XNNSW Pin is on, Which pull down the AN1 line*/
	tscadc.tsStepAnalogGroundConfig(true, false, false, false, step);

	/* select fifo for storing the data */
	tscadc.tsStepFIFOSelConfig(step, fifo);

	/* Configure averaging */
	tscadc.tsStepAverageConfig(step, average);
}

void ADC::startConversion() {
	/* Enable step (will get disabled by the ADC module after completion spruhl p. 1740 12.3.4(adc is configured in one-shot mode)*/
	tscadc.configureStepEnable(ADC_STEP, true); //enables step 1 in this case(step 0 is touchscreen charge)
}

void ADC::cleanUpInterrupts(void) {
	tscadc.intStatusClear(ALL_INTERRUPTS);
}

uint16_t ADC::read() {
	std::lock_guard<std::mutex> lock(mtxAccess); //automatically unlocks at the end of functions scope

	//enables step(16 configurable steps, fsm will go through all enabled steps,
	//each step can be customly configured(e.g which channel to sample, one-shot mode usw..))
	startConversion();
	unsigned int irqStatus;
	do {
		interruptHandler.awaitInterruptHightSensor(); //wait for interrupt
		irqStatus = tscadc.intStatus(); //read status of interrupt
		tscadc.intStatusClear(irqStatus); //clear source of interrupt

		interruptHandler.unmaskInterrupt(ADC_TSC_GENINT);
	} while (!(irqStatus & END_OF_SEQUENCE_INT)); // Wait for end of sequence
	//An END_OF_SEQUENCE interrupt is generated after the last active step is completed before going back
	//to the IDLE state. The END_OF_SEQUENCE interrupt does not mean data is in the FIFO (should use the
	//FIFO interrupts and FIFOxCOUNT register).(???)
	return tscadc.fifoADCDataRead(ADC_FIFO);
}
