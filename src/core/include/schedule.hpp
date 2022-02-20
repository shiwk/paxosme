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
        void AddEvent(const EventHandler &cb, const time_t &when, EventType event_type);
        void AddEvent(const EventHandler &cb, const timeval &when, EventType event_type);
        void AddEvent(const EventHandler &cb, const EventTimeStamp &when, EventType event_type);

        bool Dispatch(Event &);
        bool NextEventTime(EventTimeStamp &);

    private:
        std::unordered_map<EventType, EventId> eventTypeIdMap_;
        EventQueue eventQueue_;
        EventId eventId_;
    };
}

#endif //PAXOSME_SCHEDULE_HPP
