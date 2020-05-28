#pragma once

struct Timerinfo
{
        Timerinfo(int duration, int id, int logic_id)
        :_duration(duration), _id(id), _logic_id(logic_id){}

        int _duration;
        int _id;
        int _logic_id;
};
