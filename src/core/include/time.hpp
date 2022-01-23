//
// Created by k s on 2021/8/14.
//

#ifndef PAXOSME_TIME_H
#define PAXOSME_TIME_H
#include "common.hpp"
#include <chrono>

namespace paxosme {
    class Time {
    public:
        static time_t NowSinceEpochInMS();
        static std::chrono::milliseconds MS(uint64_t);
        static void MsSleep(const int iTimeMs);
    };
}
#endif //PAXOSME_TIMER_H
