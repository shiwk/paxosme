//
// Created by shiwk on 2020/7/18.
//

#include <pax_acceptor.hpp>

/**
 * Acceptor processing when received prepare request
 * @param message
 */
void paxosme::PaxAcceptor::HandlePreProposeRequest(paxosme::PaxMessage message) {
    PaxReplyMessage pax_reply_message;
    pax_reply_message.SetProposerId(message.GetProposerId());
    pax_reply_message.SetReplierId(GetNodeId());

    proposal_id_t proposal_id = message.GetProposalId();
    proposal_id_t proposer_id = message.GetProposerId();

    if (proposal_id > acceptor_state_->GetPromisedId() ||
        proposal_id == acceptor_state_->GetPromisedId() &&
        proposer_id > acceptor_state_->GetPromisedNodeId()) {

        // higher than promised, then accept
        pax_reply_message.SetIsRejected(false);
        proposal_id_t accepted_id = acceptor_state_->GetAcceptedId();

        /*
         * "If an acceptor receives a prepare request with number n greater than that of any prepare request to
         * which it has already responded, then it responds to the request with a promise not to accept any
         * more proposals numbered less than n and with the highest-numbered pro- posal (if any) that it has accepted."
         * */

        if (accepted_id != 0) {
            pax_reply_message.SetAcceptedId(accepted_id);
            pax_reply_message.SetAcceptedValue(acceptor_state_->GetAcceptedValue());
        }
        acceptor_state_->SetPromisedProposal(proposal_id, proposer_id);

    } else {

        // lower than promised, then reject
        pax_reply_message.SetIsRejected(true);
        proposal_id_t promised_id = acceptor_state_->GetPromisedId();
        node_id_t promised_node_id = acceptor_state_->GetPromisedNodeId();

        /*
         * "An acceptor can accept a proposal numbered n iff it has not
         * responded to a prepare request having a number greater than n."
         * */

        pax_reply_message.SetPromisedId(promised_id);
        pax_reply_message.SetPromisedNodeId(promised_node_id);
    }

    // todo: send pax_reply_message to proposer
}

void paxosme::PaxAcceptor::HandleProposeRequest(paxosme::PaxMessage message) {

}
