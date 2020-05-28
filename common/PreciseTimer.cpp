#include "PreciseTimer.h"
#include <iostream>
#include <string.h>

void PreciseTimer::start()
{
    startTime = std::chrono::system_clock::now();
}

std::string PreciseTimer::stop()
{
    // Снимаем показания таймера
    std::chrono::system_clock::time_point stopTime = std::chrono::high_resolution_clock::now();

    // Вычисляем разницу времени и преобразуем в double
    std::chrono::duration<double, std::milli> elapsed = stopTime - startTime;
    double seconds = elapsed.count()/1000.0;
    int minutes = (int)(seconds / 60.0);
    seconds -= minutes*60;

    std::string result = "";
    if (minutes)
        result += (std::to_string(minutes)+" minutes ");
    result += std::to_string(seconds)+" seconds";

    return result;
}
