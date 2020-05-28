#pragma once

#include "enums.h"
#include "timerinfo.h"
//#include "Logger.h"

struct Message
{
        Message(Signal signal, Component dest, Component src)
        : _timerinfo(Timerinfo(0,0,0))
        {
            this->signal = signal;
            this->dest = dest;
            this->src = src;
            this->_pukdata = PUKType::TYPENULL;
//            this->_timerinfo = Timerinfo(0,0,0);
            this->_idinfo = -1;
        }

        void setTimerData(int duration, int id, int logic_id)
        {
            _timerinfo._duration = duration;
            _timerinfo._id = id;
            _timerinfo._logic_id = logic_id;
        }

        Signal signal;
        Component dest;
        Component src;

        PUKType _pukdata;
        int _idinfo;
        Timerinfo _timerinfo;
};
