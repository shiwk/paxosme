//
// Created by shiwk on 2021/3/29.
//

#ifndef PAXOSME_EVENT_QUEUE_HPP
#define PAXOSME_EVENT_QUEUE_HPP

#include <chrono>
#include <vector>
#include "time.hpp"

using EventHandler = std::function<void()>;
using EventTimeStamp = std::chrono::steady_clock::time_point;

enum EventType
{
    kNULL = 0,

    // timeout events
    kEVENT_PROPOSE_TIMEOUT = 1,
    kEVENT_ACCEPT_TIMEOUT = 2,
    kEVENT_NEWVALUE_TIMEOUT = 3,

    // schedule events
    kEVENT_SHALL_I_LEARN = 4,

    kEVENT_CLEAN_SEGMENT = 0x01 >> 3
};

using EventId = uint64_t;

struct Event
{
    Event() : eventType(kNULL), eventId(0){};

    Event(EventHandler cb, const EventTimeStamp &time, EventType type, EventId event_id) : callback(std::move(cb)),
                                                                                           when(time),
                                                                                           eventType(type),
                                                                                           eventId(event_id) {}

    EventHandler callback;
    EventTimeStamp when;
    EventType eventType;
    EventId eventId;

    void operator()() const { callback(); }

    bool operator<(const Event &e) const
    {
        return when < e.when;
    }
};

class EventQueue
{
    // no lock as event queue guarantees sequential

public:
    bool Empty();

    void Push(const Event &event);

    bool HasTimeout();

    bool PopOne(Event &);

    Event &Next();

private:
    std::vector<Event> events_;
};

#endif // PAXOSME_EVENT_QUEUE_HPP
