#include "PreciseTimer.h"

void PreciseTimer::startMeasure()
{
    startTime = std::chrono::system_clock::now();
}

double PreciseTimer::stopMeasure()
{
    // Снимаем показания таймера
    std::chrono::system_clock::time_point stopTime = std::chrono::high_resolution_clock::now();

    // Вычисляем разницу времени и преобразуем в double
    std::chrono::duration<double, std::milli> elapsed = stopTime - startTime;
    return elapsed.count()/1000.0;
}
