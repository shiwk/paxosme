//
// Created by shiwk on 2021/3/22.
//

#ifndef PAXOSME_PROVIDER_PAX_HPP
#define PAXOSME_PROVIDER_PAX_HPP

#include <log_value.hpp>
#include "common.hpp"
#include "lock.hpp"
#include "queue.hpp"
#include "time.hpp"

namespace paxosme {
    class PaxController;

    enum NewValueStatus {
        kPending = 1,
        kProposing = 2,
        kTimeout = 3,
        kSuccess = 4
    };
    
    enum NewValueResult {
        kValueNone = 1,
        kValueTimeout = 2,
        kValueAccepted = 3,
        kVALUE_ACCEPTED_WIN = 4,
        kVALUE_ACCEPTED_LOSE = 5, //todo I: not chosen by this instance, retry needed
        kTOO_MANY_VALUES = 6
    };

    struct PendingNewValue {
        LogValue* logValue;
        index_t index;
        NewValueResult newValueResult;
        instance_id_t instanceId;
    };


    class ProposalProv {

    public:
        // need waiting for the proposing value to be determined (success or fail)
        NewValueResult SubmitAndWait(const LogValue &value);

        bool Get(LogValue &);

        bool Flush(LogValue &, instance_id_t); // proposer NewValue needs, providing instance id to record
        bool WriteResult(const LogValue &, instance_id_t, NewValueResult); // consensus reached or my proposing timeout

        millisec GetNewValueTimeout();


    private:
        MyLock lock_;
        millisec timeout_ms_;
        instance_id_t instance_id_;
        index_t index_;
        index_t finish_index_;
        MyQueue<PendingNewValue> queue_;
        PaxController *paxController_;
    };
}


#endif //PAXOSME_PROVIDER_PAX_HPP
