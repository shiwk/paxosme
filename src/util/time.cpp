//
// Created by k s on 2021/8/14.
//

#include "time.hpp"

namespace paxosme {
    time_t Time::NowSinceEpochInMS() {
        auto now_time = std::chrono::system_clock::now();
        time_t now = (std::chrono::duration_cast<std::chrono::milliseconds>(now_time.time_since_epoch())).count();
        return now;
    }

    uint64_t Time::StopWatchMS(const SteadyTime begin, const SteadyTime end) {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        return duration.count();
    }

    std::chrono::milliseconds Time::MS(uint64_t ms) {
        return std::chrono::milliseconds(ms);
    };
}