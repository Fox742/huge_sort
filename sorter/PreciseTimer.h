#ifndef EXT_PRETIMER_H
#define EXT_PRETIMER_H
#include <chrono>


class PreciseTimer
{
private:
    std::chrono::system_clock::time_point startTime;

public:
    void startMeasure();
    double stopMeasure();

};

#endif // _EXT_PRETIMER_H
