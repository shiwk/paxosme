//
// Created by shiwk on 2021/3/30.
//

#ifndef PAXOSME_SCHEDULE_HPP
#define PAXOSME_SCHEDULE_HPP

#include <event.hpp>
namespace paxosme{
    class Schedule{
    public:
        void add(const event_callback&, const time_t&, EventType);
        void add(const event_callback&, const timeval&, EventType);
        void add(const event_callback&,
                 const std::chrono::time_point<std::chrono::system_clock>&, EventType);
        void run();
    private:
        std::priority_queue<Event, std::vector<Event>, std::less<>> event_queue;

    };
}

#endif //PAXOSME_SCHEDULE_HPP
