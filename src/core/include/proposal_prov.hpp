//
// Created by shiwk on 2021/3/22.
//

#ifndef PAXOSME_PROVIDER_PAX_HPP
#define PAXOSME_PROVIDER_PAX_HPP

#include <log_value.hpp>
#include "common.hpp"
#include "lock.hpp"

namespace paxosme {

    enum NewValueStatus {
        kPending = 1,
        kProposing = 2,
        kTimeout = 3,
        kSuccess = 4
    };

    struct PendingNewValue {
        LogValue logValue;
        instance_id_t instanceId;
        NewValueStatus newValueStatus;
    };

    enum NewValueResult {
        kValueNone = 1,
        kValueTimeout = 2,
        kValueAccepted = 3,
        kValueLose = 4
    };

    class ProposalProv {

    public:
        // todo I: cache for new proposing value and push the "NewValue" msg to message queue.
        // need waiting for the proposing value to be determined (success or fail)
        void Push(const LogValue &value);

        bool Get(LogValue &);

        bool FirstGet(LogValue &, instance_id_t); // proposer NewValue needs, providing instance id to record
        bool Flush(const LogValue &, instance_id_t, NewValueResult); // consensus reached or my proposing timeout

        millisec GetNewValueTimeout();


    private:
        MyLock lock_;
        millisec timeout_ms_;
        std::queue<PendingNewValue> queue_;
    };
}


#endif //PAXOSME_PROVIDER_PAX_HPP
