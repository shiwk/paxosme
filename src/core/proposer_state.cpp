//
// Created by shiwk on 2020/8/16.
//

#include <proposer_state.hpp>

/**
 *
 * @param proposal_id
 * @param node_id
 * @return
 */
bool paxosme::ProposerState::TryUpdateHighestProposalId(proposal_id_t proposal_id, node_id_t node_id) {

    /*
     * Leslie Lamport. Paxos Made Simple, 01 Nov 2001
     * "If the proposer receives a response to its prepare requests (numbered n) from a majority of acceptors,
     * then it sends an accept request to each of those acceptors for a proposal numbered n with a value v,
     * where v is the value of the highest-numbered proposal among the responses,
     * or is any value if the responses reported no proposals."
     * */

    if (proposal_id < highest_proposal_id_)
        return false;

    if (proposal_id > highest_proposal_id_ || node_id > highest_proposal_id_provider_) {
        highest_proposal_id_ = proposal_id;
        highest_proposal_id_provider_ = node_id;
        return true;
    }

    return false;
}

bool paxosme::ProposerState::TryUpdateLogValue(proposal_id_t proposal_id, node_id_t node_id,
                                               const paxosme::LogValue &log_value) {
    if (TryUpdateHighestProposalId(proposal_id, node_id)) {
        pending_message_.SetLogValue(log_value);
        return true;
    }

    return false;
}

