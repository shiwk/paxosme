//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_LEARNER_H
#define PAXOSME_PAX_LEARNER_H

#include "player.h"
#include "types/proposal_id.hpp"
#include "types/node_id.hpp"

namespace paxosme {
    class PaxLearner : public PaxPlayer {
        void LearnNewValue();
        void HandleLearnRequest(PaxMessage message);
    private:
        void ReplyLearnedValue(ProposalID proposal_id, NodeID node_id);
    };
}


#endif //PAXOSME_PAX_LEARNER_H
