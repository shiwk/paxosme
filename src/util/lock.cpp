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

void MyLock::Notify() {
    cond_var_.notify_one();
}




