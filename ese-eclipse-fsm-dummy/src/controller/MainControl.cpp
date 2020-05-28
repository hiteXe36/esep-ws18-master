#include "MainControl.h"

//#ifdef __DUMMY_TEST
//#include "../hal/DummyHAL.h"
//#else
//#include "../hal/HAL.h"
//#endif
//#include "../servicetools/Logger.h"

#include <iostream>

using namespace std;

//		this_thread::sleep_for(chrono::milliseconds(5000));

MainControl::MainControl(Channel& main2disp, Channel& disp2main)
        : _2disp(main2disp), _2me(disp2main)
{

}

void MainControl::operator()()
{
    while(true)
    {
        Message rcvmsg = _2me.dequeue();
        switch(rcvmsg.signal)
        {
        //-------------------------------------------------------------------------------------------------------
        //----------------------------------------LB-SIGNALS-----------------------------------------------------
        //-------------------------------------------------------------------------------------------------------
        case (Signal::LB_ENTRY_ENTERED):
            std::cout << "[MAIN] Entry-Barrier Rising" << std::endl;
            break;
        case (Signal::LB_ENTRY_LEFT):
            std::cout << "[MAIN] Entry-Barrier Falling" << std::endl;
            break;
        case (Signal::LB_HIGHT_ENTERED):
            std::cout << "[MAIN] Height-Barrier Rising" << std::endl;
            break;
        case (Signal::LB_HIGHT_LEFT):
            std::cout << "[MAIN] Height-Barrier Falling" << std::endl;
            break;
        case (Signal::LB_METAL_ENTERED):
            std::cout << "[MAIN] Gate-Barrier Rising" << std::endl;
            break;
        case (Signal::LB_METAL_LEFT):
            std::cout << "[MAIN] Gate-Barrier Falling" << std::endl;
            break;
        case (Signal::LB_EXIT_ENTERED):
            std::cout << "[MAIN] Exit-Barrier Rising" << std::endl;
            break;
        case (Signal::LB_EXIT_LEFT):
            std::cout << "[MAIN] Exit-Barrier Falling" << std::endl;
            break;
        case (Signal::LB_SLIDE_ENTERED):
            std::cout << "[MAIN] Slide-Barrier Rising" << std::endl;
            break;
        case (Signal::LB_SLIDE_LEFT):
            std::cout << "[MAIN] Height-Barrier Falling" << std::endl;
            break;

            //-------------------------------------------------------------------------------------------------------
            //----------------------------------------COMM-SIGNALS-----------------------------------------------------
            //-------------------------------------------------------------------------------------------------------
        case (Signal::MACHINE2BUSY):
            std::cout << "[MAIN] Got notification from other machine; Now busy." << std::endl;
            break;
        case (Signal::MACHINE2IDLE):
            std::cout << "[MAIN] Got notification from other machine; Now idle." << std::endl;
            break;
        case (Signal::PASS_CONFIRMED):
            std::cout << "[MAIN] Got notification from other machine; Pass succesful." << std::endl;
            break;

            //-------------------------------------------------------------------------------------------------------
            //----------------------------------------HIGHT-SIGNALS-----------------------------------------------------
            //-------------------------------------------------------------------------------------------------------
        case (Signal::PUKDATA):
            std::cout << "[MAIN] Received PUKData for ID " << rcvmsg._idinfo << " PUKType: " << rcvmsg._pukdata << std::endl;
            break;
            //-------------------------------------------------------------------------------------------------------
            //----------------------------------------TIMER-SIGNALS-----------------------------------------------------
            //-------------------------------------------------------------------------------------------------------
        case (Signal::TIMEOUT):
            std::cout << "[MAIN] Received Timeout for FSM ID " << rcvmsg._timerinfo._id << ". Logic ID: " << rcvmsg._timerinfo._logic_id << std::endl;
            break;
        }
    }
    {
//        Message timerrequest(Signal::STARTTIMER, Component::TIMER, _thisComponent);
//        timerrequest.setTimerData(20, 35, 4);
//        _2disp.enqueue(timerrequest);
//
//        Message datarequest(Signal::GETPUKDATA, Component::HIGHT, _thisComponent);
//        datarequest._idinfo = 35;
//        _2disp.enqueue(datarequest);
//
//        Message rcvdata = _2me.dequeue();
//        std::cout << "PUKType: " << rcvdata._pukdata << std::endl;
//
//        Message send2other(Signal::NOTIFY_MACHINE2, Component::OTHER_MACHINE, _thisComponent);
//        _2disp.enqueue(send2other);

    }
}

//void MainControl::sendMessage(Signal s, Component c)
//{
//	Message newmsg(s,c,_thisComponent);
//	_2disp.enqueue(newmsg);
//}

MainControl::~MainControl()
{
//NULL;
}

