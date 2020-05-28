#pragma once

#include "../servicetools/Channel.h"
#include "../servicetools/Message.h"
#include <iostream>

class PUKTypeControl
{
public:
    explicit PUKTypeControl(Channel& disp2puktc, Channel& toDisp);
    virtual ~PUKTypeControl();

    void operator()();

private:

    Channel& _2me;
    Channel& _2disp;

    static constexpr Component _thisComponent = Component::HIGHT;
};
