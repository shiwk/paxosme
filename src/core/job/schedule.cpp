//
// Created by shiwk on 2021/3/27.
//

#include <queue>
#include <event.hpp>
#include <schedule.hpp>

namespace paxosme
{

    void Schedule::add(const event_callback &cb, const time_t &when, EventType event_type)
    {
        auto real_when = std::chrono::system_clock::from_time_t(when);

        event_queue.emplace(cb, real_when, event_type);
    }

    void Schedule::add(const event_callback &cb, const timeval &when, EventType event_type)
    {
        auto real_when = std::chrono::system_clock::from_time_t(when.tv_sec) +
                         std::chrono::microseconds(when.tv_usec);

        event_queue.emplace(cb, real_when, event_type);
    }

    void Schedule::add(const event_callback &cb,
             const event_time &when, EventType event_type)
    {
        event_queue.emplace(cb, when, event_type);
    }

    void Schedule::run()
    {
        while (true)
        {
            event_time now = std::chrono::system_clock::now();

            if(event_queue.empty() ||  now < event_queue.top().when )
                continue;

            event_callback callback = event_queue.top();
            callback();
            event_queue.pop();
        }
    }
}