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
    explicit MyQueue(size_t limit) : limit_(limit) {}

    bool TryAdd(const T &t) {
        std::unique_lock<std::mutex> lock(mtx_);
        bool res = false;
        if (queue_.size() < limit_) {
            queue_.push(t);
            res = true;
            cv_.notify_one();
        }
        return res;
    }

    // get and pop
    bool Take(T &t, std::chrono::milliseconds waitTime = std::chrono::milliseconds(1000)) {
        std::unique_lock<std::mutex> lock(mtx_);
        bool pred = cv_.wait_for(lock, waitTime, [&]() { return !queue_.empty(); });

        if (pred) {
            // queue_ is not empty
            t = queue_.front();
            queue_.pop();
            return true;
        }

        // queue_ is empty and timeout
        return false;
    }

    // pop
    void PopN(size_t t) {
        std::unique_lock<std::mutex> lock(mtx_);
        size_t i =0;
        while (i++ < t)
            queue_.pop();
    }

    size_t Size() {
        std::unique_lock<std::mutex> lock(mtx_);
        return queue_.size();
    }

    size_t Empty() {
        std::unique_lock<std::mutex> lock(mtx_);
        return queue_.empty();
    }

    T &Front() {
        std::unique_lock<std::mutex> lock(mtx_);
        return queue_.front();
    }

private:
    std::queue<T> queue_;
    std::mutex mtx_;
    std::condition_variable cv_;
    size_t limit_;
};


#endif //PAXOSME_QUEUE_HPP
