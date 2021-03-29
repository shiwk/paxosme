//
// Created by shiwk on 2021/3/27.
//

#include <queue>
#include <sys/time.h>
#include <event.hpp>

namespace paxosme
{
    std::priority_queue<event, std::vector<event>, event_less> event_queue;

    void add(const event::callback_type &cb, const time_t &when)
    {
        auto real_when = std::chrono::system_clock::from_time_t(when);

        event_queue.emplace(cb, real_when);
    }

    void add(const event::callback_type &cb, const timeval &when)
    {
        auto real_when = std::chrono::system_clock::from_time_t(when.tv_sec) +
                         std::chrono::microseconds(when.tv_usec);

        event_queue.emplace(cb, real_when);
    }

    void add(const event::callback_type &cb,
             const std::chrono::time_point<std::chrono::system_clock> &when)
    {
        event_queue.emplace(cb, when);
    }

    void timer()
    {
        event::time_type now = std::chrono::system_clock::now();

        while (!event_queue.empty() &&
               (event_queue.top().when_ < now))
        {
            event_queue.top()();
            event_queue.pop();
        }
    }
}