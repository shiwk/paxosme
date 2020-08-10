//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_LEARNER_HPP
#define PAXOSME_PAX_LEARNER_HPP

#include <log_value.hpp>
#include "pax_player.hpp"
#include "pax_config.hpp"

namespace paxosme {
    class PaxLearner : public PaxPlayer {
    public:
        void LearnNewValue();
        void HandleLearnRequest(PaxMessage message);

        void ReceiveChosenValue(PaxMessage message);

        bool GetLearnedValue(LogValue & log_value);
        instance_id_t GetLastInstanceId();

    private:
        void RequestNewValue(); // LearnNewValue
        void ReplyLearnedValue(node_id_t node_id, LogValue log_value); // HandleLearnRequest
        void BroadCastLearnedValue(LogValue log_value); // if chosen my local proposer

        PaxConfig pax_config_;
        LogValue log_value_;
    };
}


#endif //PAXOSME_PAX_LEARNER_HPP
