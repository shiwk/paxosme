//
// Created by shiwk on 2021/3/29.
//

#ifndef PAXOSME_EVENT_QUEUE_HPP
#define PAXOSME_EVENT_QUEUE_HPP


#include <chrono>
#include <vector>

using event_callback = std::function<void()>;
using event_time = std::chrono::time_point<std::chrono::system_clock>;

namespace paxosme {

    enum EventType {
        kNULL = 0,
        kPrepareTimeout = 1,
        kProposeTimeout = 2,
        kShallILearnTimeout = 3
    };

    using EventId = uint64_t;

    struct Event {
        Event() : eventType(kNULL), eventId(0) {};

        Event(event_callback cb, const event_time &time, EventType type, EventId event_id) : callback(std::move(cb)),
                                                                                             when(time),
                                                                                             eventType(type),
                                                                                             eventId(event_id) {}

        event_callback callback;
        event_time when;
        EventType eventType;
        EventId eventId;

        void operator()() const { callback(); }

        bool operator<(const Event &e) const {
            return when < e.when;
        }
    };

    class EventQueue {
    public:
        void Add(const Event &);

        bool HasTimeout(Event &);

        void Pop();

    private:
        std::vector<Event> events_;
    };
}

#endif //PAXOSME_EVENT_QUEUE_HPP
