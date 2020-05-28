#include "../servicetools/Channel.h"
#include "../servicetools/enums.h"
#include "../controller/TimerControl.h"

#include <thread>
#include <chrono>

class TimerControlTest {
private:
	Channel *meChannel;
	Channel *timerControlChannel;
public:
	TimerControlTest() {
		meChannel = new Channel(5);
		timerControlChannel = new Channel(5);

		TimerControl tc(*meChannel, *timerControlChannel);
		std::thread tct(std::ref(tc));
		tct.detach();

		testTimerStop();
		testTimerTimedOut();
		testTimerStartMultiple();
	}

	virtual ~TimerControlTest() {

	}

	void testTimerTimedOut() {
		LOG_WRITE("TestTimerTimedOut start");

		Timerinfo info(2000, 5, 5);
		timerControlChannel->enqueue(
				Message(Signal::STARTTIMER, Component::TIMER_CONTROL,
						Component::TIMER_CONTROL, info));

		Message msg = meChannel->dequeue();
		LOG_WRITE(
				"Received signal: " + Enums::signalToString(msg.signal)
						+ " from : " + Enums::componentToString(msg.src));

		LOG_WRITE("TestTimerTimedOut end");
	}

	void testTimerStop() {
		LOG_WRITE("TestTimerStop start");

		Timerinfo info(2000, 5, 5);
		timerControlChannel->enqueue(
				Message(Signal::STARTTIMER, Component::TIMER_CONTROL,
						Component::TIMER_CONTROL, info));
		timerControlChannel->enqueue(
				Message(Signal::STOPTIMER, Component::TIMER_CONTROL,
						Component::TIMER_CONTROL, info));
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		if (meChannel->size() == 0) {
			LOG_WRITE("No Time Out received");

		}
		if (timerControlChannel->size() == 0) {
			LOG_WRITE("Timer Started and Stopped");
		}

		LOG_WRITE("TestTimerStop end");
	}

	void testTimerStartMultiple() {
		LOG_WRITE("TestTimerStartMultiple start");

		Timerinfo info(5000, 5, 5);
		timerControlChannel->enqueue(
				Message(Signal::STARTTIMER, Component::TIMER_CONTROL,
						Component::TIMER_CONTROL, info));
		Timerinfo info2(2000, 10, 10);
		timerControlChannel->enqueue(
				Message(Signal::STARTTIMER, Component::TIMER_CONTROL,
						Component::TIMER_CONTROL, info2));

		Message msg = meChannel->dequeue();
		LOG_WRITE(
				"Received signal: " + Enums::signalToString(msg.signal)
						+ " from : " + Enums::componentToString(msg.src)
						+ " with ID: " + std::to_string(msg._timerinfo._id));

		msg = meChannel->dequeue();
				LOG_WRITE(
						"Received signal: " + Enums::signalToString(msg.signal)
								+ " from : " + Enums::componentToString(msg.src)
								+ " with ID: " + std::to_string(msg._timerinfo._id));

		LOG_WRITE("TestTimerStartMultiple end");
	}
};

