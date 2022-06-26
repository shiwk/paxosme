//
// Created by k s on 2022/2/20.
//

#include "lock.hpp"

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
    // todo II
}

bool MyLock::WaitFor(std::chrono::milliseconds duration) {
    return cond_var_.wait_for(mtx_, duration) == std::cv_status::timeout;
}