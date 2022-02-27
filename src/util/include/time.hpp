//
// Created by k s on 2021/8/14.
//

#ifndef PAXOSME_TIME_H
#define PAXOSME_TIME_H
#include "common.hpp"
#include <chrono>

#define STEADY_TIME_NOW std::chrono::steady_clock::now()
#define SYSTEM_TIME_NOW std::chrono::system_clock::now()

using SteadyTime = std::chrono::steady_clock::time_point;
using SystemTime = std::chrono::system_clock::time_point;
using millisec = uint64_t;


namespace paxosme {
    class Time {
    public:
        static uint64_t StopWatchMS(SteadyTime begin, SteadyTime end);
        static std::chrono::milliseconds DurationMS(SteadyTime begin, SteadyTime end);
        static std::chrono::milliseconds MS(uint64_t);
        static void MsSleep(const millisec iTimeMs);
    };
}
#endif //PAXOSME_TIMER_H
