//
// Created by shiwk on 2021/3/29.
//

#ifndef PAXOSME_EVENT_HPP
#define PAXOSME_EVENT_HPP

#endif //PAXOSME_EVENT_HPP

#include <chrono>

using event_callback = std::function<void()>;
using event_time = std::chrono::time_point<std::chrono::system_clock>;

namespace paxosme {

    enum EventType {
        kPrepare,
        kRequestLearning
    };

    struct Event {

        Event(event_callback cb, const event_time &time, EventType type) : callback(std::move(cb)), when(time),
                                                                           event_type(type) {}

        event_callback callback;
        event_time when;
        EventType event_type;

        void operator()() const { callback(); }

        bool operator<(const Event &e) const {
            return when < e.when;
        }
    };
}