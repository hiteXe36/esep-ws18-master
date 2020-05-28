#pragma once

#include "../servicetools/Channel.h"
#include "../servicetools/Message.h"
#include <iostream>

class SerialControl
{
public:
    explicit SerialControl(Channel& disp2gate, Channel& serial2disp);
    virtual ~SerialControl();

    void operator()();

    void send(Message &m);
    void receive(Message &m);

private:

    Channel& _2me;
    Channel& _2disp;

    static constexpr Component _thisComponent = Component::MOTOR;
};
