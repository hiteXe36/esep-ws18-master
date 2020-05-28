#pragma once

#include "../servicetools/Channel.h"
#include "../servicetools/Message.h"
#include <iostream>

class TimerControl
{
public:
    explicit TimerControl(Channel& disp2timer);
    virtual ~TimerControl();

    void operator()();

    void send(Message &m);
    void receive(Message &m);

private:

    Channel& _2me;

    static constexpr Component _thisComponent = Component::TIMER;
};
