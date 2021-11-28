//
// Created by shiwk on 2020/8/16.
//

#ifndef PAXOSME_PROPOSER_STATE_HPP
#define PAXOSME_PROPOSER_STATE_HPP


#include "messages_pax.hpp"
#include "config_pax.hpp"

namespace paxosme {
    class ProposerState {
        // todo I: initialization
        LogValue& log_value_;
        proposal_id_t highest_known_proposal_;
        proposal_id_t my_proposal_Id_;
        node_id_t highest_proposal_id_provider_;

    public:
        proposal_id_t GetMyProposalId() const;
        void Init(proposal_id_t);
        const LogValue &GetLogValue() const;

        void SetLogValue(LogValue &log_value);

        bool TryUpdateHighestProposalId(proposal_id_t proposal_id, node_id_t node_id);

        bool TryUpdateLogValue(proposal_id_t proposal_id, node_id_t node_id, const LogValue &log_value);

        proposal_id_t NewProposalId();
        void Reset();
    };
}


#endif //PAXOSME_PROPOSER_STATE_HPP
