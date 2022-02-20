//
// Created by k s on 2021/8/14.
//

#include "time.hpp"

namespace paxosme {

    uint64_t Time::StopWatchMS(const SteadyTime begin, const SteadyTime end) {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        return duration.count();
    }

    std::chrono::milliseconds Time::MS(uint64_t ms) {
        return std::chrono::milliseconds(ms);
    }

    std::chrono::milliseconds Time::DurationMS(SteadyTime begin, SteadyTime end) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    };
}