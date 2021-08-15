//
// Created by k s on 2021/8/14.
//

#include "Time.hpp"

namespace paxosme{
    const uint64_t Time :: NowSinceEpochInMS()
    {
        auto now_time = chrono::system_clock::now();
        uint64_t now = (chrono::duration_cast<chrono::milliseconds>(now_time.time_since_epoch())).count();
        return now;
    }

    const std::chrono:milliseconds MS(uint64_t ms){
        return std::chrono::milliseconds(ms);
    };
}