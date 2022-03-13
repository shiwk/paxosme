//
// Created by k s on 2022/2/20.
//

#ifndef PAXOSME_LOCK_HPP
#define PAXOSME_LOCK_HPP

#include <mutex>
#include <chrono>
#include <condition_variable>

//using Pred = std::function<bool()>;

class MyLock {
public:
    void Lock();

    void UnLock();

    void NotifyOne();
    void NotifyAll();

    template<class Pred>
    bool WaitFor(std::chrono::milliseconds duration, Pred &&);

    template<class Pred>
    void Wait(Pred &&);

    template<class Pred>
    bool WaitUntil(std::chrono::steady_clock::time_point, Pred &&);

private:
    std::mutex mtx_;
    std::condition_variable_any cond_var_;
};

template<class Pred>
bool MyLock::WaitFor(std::chrono::milliseconds duration, Pred &&pred) {
    return cond_var_.wait_for(mtx_, duration, pred);
}


template<class Pred>
bool MyLock::WaitUntil(std::chrono::steady_clock::time_point tp, Pred &&pred) {
    return cond_var_.wait_until(mtx_, tp, pred);
}

template<class Pred>
void MyLock::Wait(Pred &&pred) {
    cond_var_.wait(mtx_, pred);
}

void MyLock::Lock() {
    mtx_.lock();
}

void MyLock::UnLock() {
    mtx_.unlock();
}

void MyLock::NotifyOne() {
    cond_var_.notify_one();
}

void MyLock::NotifyAll() {

}


#endif //PAXOSME_LOCK_HPP
