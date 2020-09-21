//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_LEARNER_HPP
#define PAXOSME_PAX_LEARNER_HPP

#include <log_value.hpp>
#include "pax_player.hpp"
#include "pax_config.hpp"

namespace paxosme {
    class PaxLearnerState{
        LogValue log_value_;
    public:
        void LearnNewValue(const LogValue& log_value){
            log_value_ = log_value;
        }
        const LogValue &GetLearnedValue() {
            return log_value_;
        }
    };
    class PaxLearner : public PaxPlayer {
    public:
        void HandleNewValueProposed(const PaxMessage& pax_message);
        void HandleNewValueRequestReply(const PaxMessage& pax_message);
        const LogValue & GetLearnedValue();

    private:
        void RequestNewValue(); // LearnNewValue
        void ReplyLearnedValue(node_id_t node_id, LogValue log_value); // HandleLearnRequest
        void BroadCastLearnedValue(LogValue log_value); // if chosen my local proposer

        PaxLearnerState* pax_learner_state_;
    };
}


#endif //PAXOSME_PAX_LEARNER_HPP
