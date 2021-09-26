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

    bool EventQueue::HasTimeout(Event &event) {
        if (events_.empty())
            return false;

        auto front = events_.front();
        auto when = event.when;
        event_time now = std::chrono::system_clock::now();
        if  (now < when)
            return false;
        event = front;
        return true;
    }

    void EventQueue::Pop() {
        std::pop_heap(events_.begin(), events_.end());
        events_.pop_back();
    }
}