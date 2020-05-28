#include "TimerControl.h"
#include <iostream>

//#ifdef __DUMMY_TEST
//#include "../hal/DummyHAL.h"
//#else
//#include "../hal/HAL.h"
//#endif
//#include "../servicetools/Logger.h"

TimerControl::TimerControl(Channel& disp2timer)
        : _2me(disp2timer)
{

}

void TimerControl::operator()()
{
    while(true)
    {
        Message rcvMsg = _2me.dequeue();
//      LOG_WRITE("MotorControllerThread hat eine Message bekommen");
        switch(rcvMsg.signal)
        {
        case Signal::STARTTIMER:

            std::cout << "[TIMER] New Timer Request. Duration: " << rcvMsg._timerinfo._duration << ". ID: " << rcvMsg._timerinfo._id << ". logic_id: "
                    << rcvMsg._timerinfo._logic_id << std::endl;
            break;
        case Signal::STOPALLTIMERS:
            std::cout << "[TIMER] Stopping all timers." << std::endl;
            break;
        case Signal::CONTINUEALLTIMERS:
            std::cout << "[TIMER] Continuing all timers." << std::endl;
            break;
        default:
            std::cout << "[TIMER] TimerControl got wrong signal" << std::endl;
            break;
        }
    }
}

TimerControl::~TimerControl()
{

}

