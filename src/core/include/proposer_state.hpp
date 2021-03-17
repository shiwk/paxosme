//
// Created by shiwk on 2020/8/16.
//

#ifndef PAXOSME_PROPOSER_STATE_HPP
#define PAXOSME_PROPOSER_STATE_HPP


#include "messages_pax.hpp"
#include "pax_config.hpp"

namespace paxosme {
    class ProposerState {
        LogValue *log_value_;
        proposal_id_t highest_known_proposal_;
        proposal_id_t my_proposal_;


    private:
        node_id_t highest_proposal_id_provider_;

    public:
        proposal_id_t GetMyProposal() const;

        void SetMyProposal(proposal_id_t my_proposal);


        LogValue *GetLogValue() const;

        void SetLogValue(LogValue *log_value);

        bool TryUpdateHighestProposalId(proposal_id_t proposal_id, node_id_t node_id);

        bool TryUpdateLogValue(proposal_id_t proposal_id, node_id_t node_id, const LogValue &log_value);

        proposal_id_t GetNewProposalId();


    };
}


#endif //PAXOSME_PROPOSER_STATE_HPP
