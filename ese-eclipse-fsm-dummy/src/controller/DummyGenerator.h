#pragma once

#include "../servicetools/Channel.h"

class DummyGenerator
{
public:
    explicit DummyGenerator(Channel& toDisp);
    virtual ~DummyGenerator();

    void operator()();

private:

    Channel& _2disp;

//    static constexpr Component _thisComponent = Component::MOTOR;
};
