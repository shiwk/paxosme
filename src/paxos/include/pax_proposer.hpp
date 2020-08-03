//
// Created by shiwk on 2020/7/18.
//

#ifndef PAXOSME_PAX_PROPOSER_HPP
#define PAXOSME_PAX_PROPOSER_HPP


#include <log_value.hpp>
#include "pax_player.hpp"
#include "pax_config.hpp"

namespace paxosme {

    enum class PaxProposerStatus {
        kNone = 0,
        kPrepare = 1,
        kAccept = 2
    };

    class PaxProposer : public PaxPlayer {

        void ProposeNew(LogValue log_value);

        void HandlePreReply(PaxMessage message);

        void HandleAcceptReply(PaxMessage message);

        void ProposeNoop();

    private:
        void PrePropose(PaxMessage message);

        void Propose(PaxMessage message);

        void OnChosenValue(ProposalId proposal_id);

        void SetProposerStatus(paxosme::PaxProposerStatus proposer_status);

        PaxConfig pax_config_;

    public:
        PaxProposer(const PaxConfig &pax_config);

    private:

        PaxProposerStatus proposer_status_;
    };
}
#endif //PAXOSME_PAX_PROPOSER_HPP
