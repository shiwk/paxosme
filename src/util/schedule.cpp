//
// Created by shiwk on 2021/3/27.
//

#include "event.hpp"
#include "schedule.hpp"

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

void Scheduler::AddEvent(const EventHandler &cb, const EventTimeStamp &when, EventType event_type)
{
    EventId event_id = ++eventId_;
    eventQueue_.Push({cb, when, event_type, event_id});
    eventTypeIdMap_[event_type] = event_id;
}

bool Scheduler::Dispatch(Event &event)
{
    if (!eventQueue_.HasTimeout())
        return false;

    while (eventQueue_.PopOne(event))
    {
        if (eventTypeIdMap_[event.eventType] != event.eventId)
            // already replaced event
            continue;
        return true;
    }

    // this should not happen as indeed some events are not out-of-date
    return false;
}

bool Scheduler::NextEventTime(EventTimeStamp &event_time_stamp)
{
    if (eventQueue_.Empty())
        return false;

    Event e = eventQueue_.Next();
    while (eventTypeIdMap_[e.eventType] != e.eventId)
    {
        // already replaced event
        eventQueue_.PopOne(e);
        e = eventQueue_.Next();
    }

    event_time_stamp = e.when;

    return true;
}

void Scheduler::Remove(EventType event_type)
{
    eventTypeIdMap_.erase(event_type);
}