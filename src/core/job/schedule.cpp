//
// Created by shiwk on 2021/3/27.
//

#include <queue>
#include "event_queue.hpp"
#include "schedule.hpp"

namespace paxosme {

    void Schedule::add(const event_callback &cb, const time_t &when, EventType event_type) {
        EventId event_id = ++eventId_;
        auto real_when = std::chrono::system_clock::from_time_t(when);

        eventQueue_.Add({cb, real_when, event_type, event_id});
        eventTypeIdMap_[event_type] = event_id;
    }

    void Schedule::add(const event_callback &cb, const timeval &when, EventType event_type) {
        EventId event_id = ++eventId_;
        auto real_when = std::chrono::system_clock::from_time_t(when.tv_sec) +
                         std::chrono::microseconds(when.tv_usec);

        eventQueue_.Add({cb, real_when, event_type, event_id});
        eventTypeIdMap_[event_type] = event_id;
    }

    void Schedule::add(const event_callback &cb, const event_time &when, EventType event_type) {
        EventId event_id = ++eventId_;
        eventQueue_.Add({cb, when, event_type, event_id});
        eventTypeIdMap_[event_type] = event_id;
    }

    [[noreturn]] void Schedule::run() {
        while (true) {
            event_time now = std::chrono::system_clock::now();

            Event timeOutEvent;
            if (!eventQueue_.HasTimeout(timeOutEvent) ||
                eventTypeIdMap_[timeOutEvent.eventType] != timeOutEvent.eventId)
                continue;

            timeOutEvent();
            eventQueue_.Pop();
        }
    }
}