//
// Created by shiwk on 2021/3/22.
//

#ifndef PAXOSME_PROVIDER_PAX_HPP
#define PAXOSME_PROVIDER_PAX_HPP

#include <log_value.hpp>
#include "common.hpp"
#include "lock.hpp"

namespace paxosme {

    enum LogValueStatus {
        kPending = 1,
        kProposing = 2,
        kTimeout = 3,
        kSuccess = 4
    };

    struct PendingLogValue {
        LogValue logValue;
        instance_id_t instanceId;
        LogValueStatus logValueStatus;
    };

    class ProposalProv {

    public:
        // todo I: cache for new proposing value and push the "NewValue" msg to message queue.
        // need waiting for the proposing value to be determined (success or fail)
        void Push(const LogValue &value);

        bool Get(LogValue &);

        bool FirstGet(LogValue &, instance_id_t); // proposer NewValue needs, providing instance id to record
        bool Flush(const LogValue &, instance_id_t); // consensus reached or my proposing timeout

        millisec GetNewValueTimeout();


    private:
        MyLock lock_;
        millisec timeout_ms_;
        std::queue<PendingLogValue> queue_;
    };
}


#endif //PAXOSME_PROVIDER_PAX_HPP
