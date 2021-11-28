//
// Created by shiwk on 2020/8/16.
//

#include <proposer_state.hpp>

namespace paxosme {
    bool ProposerState::TryUpdateHighestProposalId(proposal_id_t proposal_id, node_id_t node_id) {

        /*
         * Leslie Lamport. Paxos Made Simple, 01 Nov 2001
         * "If the proposer receives a response to its prepare requests (numbered n) from a majority of acceptors,
         * then it sends an accept request to each of those acceptors for a proposal numbered n with a value v,
         * where v is the value of the highest-numbered proposal among the responses,
         * or is any value if the responses reported no proposals."
         * */

        if (proposal_id < highest_known_proposal_)
            return false;

        if (proposal_id > highest_known_proposal_ || node_id > highest_proposal_id_provider_) {
            highest_known_proposal_ = proposal_id;
            highest_proposal_id_provider_ = node_id;
            return true;
        }

        return false;
    }

    bool ProposerState::TryUpdateLogValue(proposal_id_t proposal_id, node_id_t node_id,
                                          const paxosme::LogValue &log_value) {
        if (TryUpdateHighestProposalId(proposal_id, node_id)) {
            log_value_ = log_value;
            return true;
        }

        return false;
    }

    const LogValue &ProposerState::GetLogValue() const {
        return log_value_;
    }

    void ProposerState::SetLogValue(LogValue &log_value) {
        log_value_ = log_value;
    }

    proposal_id_t ProposerState::NewProposalId() {
        my_proposal_Id_ = (my_proposal_Id_ > highest_known_proposal_ ? my_proposal_Id_ : highest_known_proposal_) + 1;
        return my_proposal_Id_;
    }

    proposal_id_t ProposerState::GetMyProposalId() const {
        return my_proposal_Id_;
    }

    void ProposerState::Init(proposal_id_t init_proposal_id) {
        my_proposal_Id_ = init_proposal_id;
    }

    void ProposerState::Reset() {

    }
}

