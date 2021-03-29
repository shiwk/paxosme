//
// Created by shiwk on 2021/3/29.
//

#ifndef PAXOSME_EVENT_HPP
#define PAXOSME_EVENT_HPP

#endif //PAXOSME_EVENT_HPP
#include <chrono>

namespace paxosme {
    struct event
    {
        typedef std::function<void()> callback_type;
        typedef std::chrono::time_point<std::chrono::system_clock> time_type;

        event(callback_type cb, const time_type &when): callback_(std::move(cb)), when_(when)
        { }

        void operator()() const
        { callback_(); }

        callback_type callback_;
        time_type     when_;
    };

    struct event_less : public std::less<event> {
        bool operator()(const event &e1, const event &e2) const {
            return (e2.when_ < e1.when_);
        }
    };
}