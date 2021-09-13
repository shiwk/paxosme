//
// Created by shiwk on 2021/3/30.
//

#ifndef PAXOSME_SCHEDULE_HPP
#define PAXOSME_SCHEDULE_HPP

#include <event_queue.hpp>
#include <queue>
#include <unordered_map>
namespace paxosme{
    class Schedule{
    public:
        void add(const event_callback&, const time_t&, EventType);
        void add(const event_callback&, const timeval&, EventType);
        void add(const event_callback&, const std::chrono::time_point<std::chrono::system_clock>&, EventType);

        [[noreturn]] void run();

    private:
        std::unordered_map<EventType, EventId> eventTypeIdMap_;
        EventQueue eventQueue_;
        EventId eventId_;
    };
}

#endif //PAXOSME_SCHEDULE_HPP
