//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_PROPOSER_H
#define PAXOSME_PAX_PROPOSER_H


#include <log_value.hpp>
#include "player.h"
#include "types/proposal_id.hpp"

namespace paxosme {
    class PaxProposer : public PaxPlayer {

        void ProposeNew(LogValue log_value);

        void HandlePreReply(PaxMessage message);

        void HandleAcceptReply(PaxMessage message);

        void ProposeNoop();

    private:
        void PrePropose(PaxMessage message);

        void Propose(PaxMessage message);

        void OnChosenValue(ProposalID proposal_id);
    };

}
#endif //PAXOSME_PAX_PROPOSER_H
