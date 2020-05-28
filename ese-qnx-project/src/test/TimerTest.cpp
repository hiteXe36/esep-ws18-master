//#include "../servicetools/Timer.h"
//#include "../servicetools/timerinfo.h"
//#include "../servicetools/Logger.h"
//#include "../servicetools/Message.h"
//#include "../servicetools/enums.h"
//#include <string>
//#include <ctime>
//
//class TimerTest {
//
//public:
//	TimerTest() {
//
//		testTimerTimedOut();
//		testTimerCancelled();
//		testStartMultipleTimer();
//		testPauseContinue();
//		LOG_WRITE("End of TimerTest!");
//	}
//
//	virtual ~TimerTest() {
//
//	}
//
//	void testTimerTimedOut(){
//		LOG_WRITE("Start TimerTest testTimerTimedOut");
//		Channel channel(5);
//		Timerinfo info(3000, 1, 1);
//		Timer timer(channel, info);
//		std::thread tt(std::ref(timer));
//		LOG_WRITE("Timer1 started!");
//
//		/* wait for timed out message from Timer */
//		auto msg = channel.dequeue();
//
//		if (msg.signal == Signal::TIMED_OUT) {
//			LOG_WRITE("Got TIMED_OUT Signal. Correct!");
//		}
//		std::string id(msg.payload);
//		LOG_WRITE("ID: " + id);
//		tt.detach();
//	}
//
//	void testTimerCancelled(){
//		LOG_WRITE("Start TimerTest testTimerCancelled");
//		Channel channel(5);
//		Timerinfo info(1000, 2, 2);
//		Timer timer(channel, info);
//		std::thread tt(std::ref(timer));
//		LOG_WRITE("Timer2 started!");
//
//		timer.stopTime();
//		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//
//		if (channel.size() == 0) {
//			LOG_WRITE("Received no timeout. Correct!");
//		}
//		else {
//			LOG_WRITE("Received timeout. False!");
//		}
//		tt.detach();
//	}
//
//	/**
//	 * Start multiple timer. The first timer MultipleTimer1 have duration
//	 * of 10 seconds (10_000 ms). The second MultipleTimer2 have duration
//	 * 2 seconds.
//	 *
//	 * Expected: Timeout of second timer must arrived first, then first timer.
//	 */
//	void testStartMultipleTimer() {
//		LOG_WRITE("Start TimerTest testStartMultipleTimer");
//		Channel channel(5);
//		Timerinfo info3(2001, 3, 3);
//		Timerinfo info4(2000, 4, 4);
//		Timer timer3(channel, info3);
//		Timer timer4(channel, info4);
//		std::thread tt1(std::ref(timer3));
//		std::thread tt2(std::ref(timer4));
//
//		Message msg = channel.dequeue();
//		LOG_WRITE("First Timeout: " + std::to_string(msg._timerinfo._id) + " \n");
//
//		msg = channel.dequeue();
//		LOG_WRITE("Second Timeout: " + std::to_string(msg._timerinfo._id) + " \n");
//		tt1.detach();
//		tt2.detach();
//
//	}
//
//	void testPauseContinue(){
//		LOG_WRITE("Start Test for Pause and Continue");
//		Channel channel(5);
//		Timerinfo info(5000, 1, 1);
//		Timer timer(channel, info);
//		std::thread tt(std::ref(timer));
//		LOG_WRITE("TimerPauseContinue started!");
//		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//		timer.pauseTimer();
//		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
//		timer.continueTimer();
//		tt.detach();
//
//	}
//
//};
//
//
