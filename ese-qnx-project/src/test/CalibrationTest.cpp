/*
 * CalibrationTest.cpp
 *
 *  Created on: 13.12.2018
 *      Author: acf139
 */
#include "../servicetools/CalibrationValues.h"
#include "../servicetools/Logger.h"
#include "../controller/CalibrationControl.h"
#include "../controller/Dispatcher.h"
#include "../controller/SensorControl.h"
#include "../controller/SerialController.h"

class CalibrationTest {

public:
	CalibrationTest() {
		LOG_ENABLE_CONSOLE_OUTPUT
		LOG_WRITE("Start of CalibrationTest!");
		testCalibration();
		//TODO test stopCalibration
		LOG_WRITE("End of CalibrationTest!");
	}

	virtual ~CalibrationTest() {

	}
	void printCalibrationValues(struct CalibrationValues caliValues) {
		std::cout << "Delta: " << caliValues.hightValues.delta << std::endl;
		std::cout << "Flat Puck: " << caliValues.hightValues.hightOfFlatPuck << std::endl;
		std::cout << "High Puck: " << caliValues.hightValues.hightOfHighPuck << std::endl;
		std::cout << "Zero: " << caliValues.hightValues.hightOfZero << std::endl;
		std::cout << "One: " << caliValues.hightValues.hightOfOne << std::endl;
	}

	void testCalibration(){
		Channel disp2gate(5);
		Channel disp2motor(5);
		Channel disp2main(5);
		Channel disp2lamp(5);
		Channel disp2emrec(5);
		Channel disp2sens(5);
		Channel disp2hight(5);
		Channel disp2cali(5);
		Channel disp2serial(5);
		Channel disp2remote(10);
		Channel disp2TimerControl(30);
		Channel omniChannel(30);

		Channel sensToTest(5);
		Channel testToSens(5);
		ADC adc;
		Dispatcher dc(disp2gate, disp2motor, disp2lamp, disp2emrec, disp2sens, disp2hight, disp2remote,disp2TimerControl, disp2cali, disp2serial, disp2main, omniChannel, EXECMODE::MAIN);
		CalibrationControl cc(omniChannel, disp2cali, adc);
		SensorControl sc(omniChannel, disp2sens);
		SerialController serc(disp2serial, omniChannel);

		std::thread serct(std::ref(serc));
		std::thread sct(std::ref(sc));
		std::thread dct(std::ref(dc));
		std::thread cct(std::ref(cc));

		omniChannel.enqueue(*(std::make_unique<Message>(Signal::STARTCALIBRATION, Component::CALIBRATION, Component::MAIN)));
		std::cout << "startcalibration from test sent " << std::endl;
		//TODO send StopCalibrationMessage

		sct.join();
		dct.join();
	}
};




