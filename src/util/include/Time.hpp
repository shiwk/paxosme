//
// Created by k s on 2021/8/14.
//

#ifndef PAXOSME_TIME_H
#define PAXOSME_TIME_H
namespace paxosme {
    class Time {
    public:
        static const uint64_t NowSinceEpochInMS();
        static const std::chrono::milliseconds MS(uint64_t);
        static void MsSleep(const int iTimeMs);
    };
}
#endif //PAXOSME_TIMER_H
