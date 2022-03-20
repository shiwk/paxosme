//
// Created by k s on 2022/2/19.
//

#include "proposal_prov.hpp"
#include "controller.hpp"
#define HARDCODE_VALUE_LIMIT 10

namespace paxosme {
    NewValueResult ProposalProv::SubmitAndWait(const LogValue &value) {
        lock_.Lock();

        index_t i = index_++;
        auto *logValue = const_cast<LogValue *>(&value);

        // instance = 0 means new value, not yet flushed
        PendingNewValue pendingNewValue{logValue, i, kValueNone, 0};

        if (!queue_.TryAdd(pendingNewValue)) {
            lock_.UnLock();
            // todo I: committer should retry in this case
            return kTOO_MANY_VALUES;
        }

        // tell main loop "here is a NewValue msg", in case waits for network msg coming
        PaxMessage message{0, kPLACEHOLDER_NEW_VALUE_COMMITTED};
        paxController_->AddMessage(message);

        lock_.Wait([&]() { return finish_index_ == i; }); // waiting for result

        PendingNewValue front = queue_.Front();
        NewValueResult res = front.newValueResult;

        delete logValue;
        front.logValue = nullptr;

        queue_.PopN(1); // pop my value

        lock_.UnLock();
        return res;
    }

    bool ProposalProv::Flush(LogValue &log_value, instance_id_t instance_id) {
        lock_.Lock();

        if (queue_.Empty()) {
            // there is no local NewValue
            lock_.UnLock();
            return false;
        }

        PendingNewValue pendingNewValue = queue_.Front();

        if (pendingNewValue.instanceId != 0) {
            // the old NewValue still not cleaned
            lock_.UnLock();
            return false;
        }

        log_value = *pendingNewValue.logValue;
        pendingNewValue.instanceId = instance_id; // means flushed

        lock_.UnLock();
        return true;
    }

    bool ProposalProv::WriteResult(const LogValue &log_value, instance_id_t instance_id, NewValueResult result) {
        lock_.Lock();

        if (queue_.Empty()) {
            // there is no local NewValue
            lock_.UnLock();
            return true;
        }

        PendingNewValue pendingNewValue = queue_.Front();

        if (pendingNewValue.instanceId == 0) {
            // the local NewValue still not flushed
            lock_.UnLock();
            return true;
        }

        if (result != kValueAccepted) {
            pendingNewValue.newValueResult = result;
        } else {
            pendingNewValue.newValueResult =
                    *pendingNewValue.logValue == log_value ? kVALUE_ACCEPTED_WIN : kVALUE_ACCEPTED_LOSE;
        }

        finish_index_ = pendingNewValue.index;

        lock_.NotifyAll(); // tell
        lock_.UnLock();
        return true;
    }
}


