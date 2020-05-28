#include "PUKTypeControl.h"
#include "../servicetools/enums.h"
#include <iostream>
#include <queue>

//#ifdef __DUMMY_TEST
//#include "../hal/DummyHAL.h"
//#else
//#include "../hal/HAL.h"
//#endif
//#include "../servicetools/Logger.h"

PUKTypeControl::PUKTypeControl(Channel& disp2puktc, Channel& toDisp)
        : _2me(disp2puktc), _2disp(toDisp)
{

}

void PUKTypeControl::operator()()
{
    std::queue<PUKType> messwerte;
    while(true)
    {
        Message rcvMsg = _2me.dequeue();
//      LOG_WRITE("MotorControllerThread hat eine Message bekommen");

        switch(rcvMsg.signal)
        {
        case Signal::GETPUKDATA:
        {

            Message sendmesg(Signal::PUKDATA,Component::MAIN,_thisComponent);
            sendmesg._idinfo = rcvMsg._idinfo;
            if(!messwerte.empty())
            {
                sendmesg._pukdata = messwerte.front();
                messwerte.pop();
            }
            else
            {
                sendmesg._pukdata = PUKType::TYPENODATA;
            }
            _2disp.enqueue(sendmesg);
            break;
        }
        case Signal::RECEIVEDATA:
            messwerte.push(rcvMsg._pukdata);
            break;
        default:
            std::cout << "[HEIGHT] Hight Controller got wrong signal" << std::endl;
            break;
        }

    }
}

PUKTypeControl::~PUKTypeControl()
{

}

