#ifndef EXT_PRETIMER_H
#define EXT_PRETIMER_H
#include <chrono>
#include <iostream>
#include <string.h>

class PreciseTimer
{
private:
    std::chrono::system_clock::time_point startTime;

public:
    void start();
    std::string stop();

};

#endif // _EXT_PRETIMER_H
