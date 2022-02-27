//
// Created by shiwk on 2021/3/29.
//

#ifndef PAXOSME_EVENT_QUEUE_HPP
#define PAXOSME_EVENT_QUEUE_HPP


#include <chrono>
#include <vector>
#include "time.hpp"

using EventHandler = std::function<void()>;
using EventTimeStamp = SteadyTime;

namespace paxosme {

    enum EventType {
        kNULL = 0,
        kPrepareTimeout = 1,
        kProposeTimeout = 2,
        kNewValueTimeout = 3,
        kShallILearnTimeout = 4
    };

    using EventId = uint64_t;

    struct Event {
        Event() : eventType(kNULL), eventId(0) {};

        Event(EventHandler cb, const EventTimeStamp &time, EventType type, EventId event_id) : callback(std::move(cb)),
                                                                                               when(time),
                                                                                               eventType(type),
                                                                                               eventId(event_id) {}

        EventHandler callback;
        EventTimeStamp when;
        EventType eventType;
        EventId eventId;

        void operator()() const { callback(); }

        bool operator<(const Event &e) const {
            return when < e.when;
        }
    };

    class EventQueue {
    public:
        void Push(const Event &event);

        bool HasTimeout();

        void PopOne(Event &);

        bool NextEvent(Event &event);

    private:
        // no lock as event queue guarantees sequential
        std::vector<Event> events_;
    };
}

#endif //PAXOSME_EVENT_QUEUE_HPP
