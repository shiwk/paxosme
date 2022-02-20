//
// Created by shiwk on 2021/3/27.
//

#include <queue>
#include "event_queue.hpp"
#include "schedule.hpp"

namespace paxosme {

//    void Schedule::AddEvent(const EventHandler &cb, const time_t &when, EventType event_type) {
//        EventId event_id = ++eventId_;
//        auto real_when = std::chrono::system_clock::from_time_t(when);
//
//        eventQueue_.Push({cb, real_when, event_type, event_id});
//        eventTypeIdMap_[event_type] = event_id;
//    }
//
//    void Schedule::AddEvent(const EventHandler &cb, const timeval &when, EventType event_type) {
//        EventId event_id = ++eventId_;
//        auto real_when = std::chrono::system_clock::from_time_t(when.tv_sec) +
//                         std::chrono::microseconds(when.tv_usec);
//
//        eventQueue_.Push({cb, real_when, event_type, event_id});
//        eventTypeIdMap_[event_type] = event_id;
//    }

    void Schedule::AddEvent(const EventHandler &cb, const EventTimeStamp &when, EventType event_type) {
        EventId event_id = ++eventId_;
        eventQueue_.Push({cb, when, event_type, event_id});
        eventTypeIdMap_[event_type] = event_id;
    }

    bool Schedule::Dispatch(Event &event) {
        if (!eventQueue_.HasTimeout())
            return false;

        eventQueue_.PopOne(event);
        if (eventTypeIdMap_[event.eventType] != event.eventId)
            // already replaced event
            return false;

        return true;
    }
}