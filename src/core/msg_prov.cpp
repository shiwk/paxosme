//
// Created by k s on 2022/2/20.
//

#include "msg_prov.hpp"

void paxosme::MsgProv::Add(PaxMessage *pax_message) {
    lock_.Lock();
    queue_.push(pax_message);
    lock_.Notify();
    lock_.UnLock();
}

bool paxosme::MsgProv::Take(paxosme::PaxMessage *message, std::chrono::milliseconds waitTime) {
    lock_.Lock();
    bool pred = lock_.WaitFor(waitTime, [&]() { return !queue_.empty(); });

    // queue_.empty() is true
    if (pred) {
        message = queue_.front();
        queue_.pop();
        lock_.UnLock();
        return true;
    }
    lock_.UnLock();

    return false;
}
