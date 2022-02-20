//
// Created by shiwk on 2021/3/28.
//

#include <chrono>
#include <event_queue.hpp>

namespace paxosme {
    void EventQueue::Push(const Event& event) {
        events_.emplace_back(event);
        std::push_heap(events_.begin(), events_.end());
    }

    bool EventQueue::HasTimeout() {
        if (events_.empty())
            return false;

        auto front = events_.front();
        auto when = front.when;

        if  (STEADY_TIME_NOW < when)
            return false;
        return true;
    }

    void EventQueue::PopOne(Event &e) {
        std::pop_heap(events_.begin(), events_.end());
        e = events_.back();
        events_.pop_back();
    }
}