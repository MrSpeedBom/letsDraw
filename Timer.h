#pragma once
#include <chrono>
#include <cstdint>
#include <iostream>
uint64_t timeSinceEpochMillisec()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
class Timer
{
    uint64_t last_time=0,pause_time=0;
    public:
    void start()
    {
        last_time=timeSinceEpochMillisec();
    }
    void pause()
    {
        if(pause_time!=0)return;
        pause_time=timeSinceEpochMillisec();
    }
    void resume()
    {
        if(pause_time==0)return;
        last_time=timeSinceEpochMillisec()-(pause_time-last_time);
        pause_time=0;
    }
    float current_time()
    {
        if(pause_time==0)
        return float(timeSinceEpochMillisec()-last_time)/1000;
        return float(pause_time-last_time)/1000;
    }
};

