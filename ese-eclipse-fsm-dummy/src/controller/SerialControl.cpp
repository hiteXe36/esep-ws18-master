#include "SerialControl.h"
#include <iostream>

//#ifdef __DUMMY_TEST
//#include "../hal/DummyHAL.h"
//#else
//#include "../hal/HAL.h"
//#endif
//#include "../servicetools/Logger.h"

SerialControl::SerialControl(Channel& disp2gate,Channel& serial2disp)
        : _2me(disp2gate), _2disp(serial2disp)
{

}

void SerialControl::operator()()
{
    while (true)
    {
        Message rcvMsg = _2me.dequeue();
//      LOG_WRITE("MotorControllerThread hat eine Message bekommen");
        switch (rcvMsg.dest)
        {
        case Component::MAIN:
            receive(rcvMsg);
            break;
        case Component::OTHER_MACHINE:
            send(rcvMsg);
            break;
        }
    }
}

void SerialControl::send(Message &m)
{
    std::cout << "[SERIAL] Sending to other machine: " << m.signal << std::endl;
}
void SerialControl::receive(Message &m)
{
    std::cout << "[SERIAL] Received from other machine: " << m.signal << " Forwarding to main..." << std::endl;
    _2disp.enqueue(m);
}
SerialControl::~SerialControl()
{

}

