//
// Created by shiwk on 2020/8/2.
//

#ifndef PAXOSME_QUEUE_HPP
#define PAXOSME_QUEUE_HPP

#include <queue>
#include "lock.hpp"

template<class T>
class MyQueue {
public:
    explicit MyQueue(size_t limit) : limit_(limit){}

    bool TryAdd(const T &t) {
        lock_.Lock();
        bool res = false;
        if (queue_.size() < limit_) {
            queue_.push(t);
            res = true;
            lock_.Notify();
        }
        lock_.UnLock();
        return res;
    }

    // get and pop
    bool Take(T &t, std::chrono::milliseconds waitTime = std::chrono::milliseconds(1000)) {
        lock_.Lock();
        bool pred = lock_.WaitFor(waitTime, [&]() { return !queue_.empty(); });


        if (pred) {
            // queue_ is not empty
            t = queue_.front();
            queue_.pop();
            lock_.UnLock();
            return true;
        }

        // queue_ is empty and timeout
        lock_.UnLock();
        return false;
    }

    // pop
    void PopN(size_t);

private:
    std::queue<T> queue_;
    MyLock lock_;
    size_t limit_;
};


#endif //PAXOSME_QUEUE_HPP
