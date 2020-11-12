//
// Created by shiwk on 2020/7/18.
//

#include <pax_acceptor.hpp>

/**
 * Acceptor processing when received prepare request
 * @param message
 */
void paxosme::PaxAcceptor::HandlePreProposeRequest(const paxosme::PaxMessage& message) {

    /**
     * "An acceptor can accept a proposal numbered n iff
     * it has not responded to a prepare request having a number greater than n."
     */

    PaxAcceptorReplyMessage pax_reply_message;
    pax_reply_message.SetProposerId(message.GetProposerId());
    pax_reply_message.SetReplierId(GetNodeId());

    proposal_id_t proposal_id = message.GetProposalId();
    proposal_id_t proposer_id = message.GetProposerId();

    proposal_id_t promised_proposal_id = acceptor_state_->GetPromisedId();
    node_id_t promised_proposer = acceptor_state_->GetPromisedNodeId();

    if (proposal_id > promised_proposal_id ||
        proposal_id == promised_proposal_id &&
        proposer_id > promised_proposer) {

        /**
         * "If an acceptor receives a prepare request with number n greater than that of any prepare request to
         * which it has already responded, then it responds to the request with a promise not to accept any
         * more proposals numbered less than n and with the highest-numbered proposal (if any) that it has accepted."
         */

        // higher than promised, then accept
        pax_reply_message.SetIsRejected(false);
        proposal_id_t accepted_proposal_id = acceptor_state_->GetAcceptedProposalId();

        if (accepted_proposal_id != 0) {
            pax_reply_message.SetAcceptedId(accepted_proposal_id);
            pax_reply_message.SetAcceptedValue(acceptor_state_->GetAcceptedValue());
        }
        acceptor_state_->SetPromisedProposal(proposal_id, proposer_id);

        // an acceptor must remember this information even if it fails and then restarts.
        Persist(message.GetInstanceId(), acceptor_state_->GetAcceptedValue());

    } else {

        // lower than promised, then reject
        pax_reply_message.SetIsRejected(true);
        proposal_id_t promised_id = acceptor_state_->GetPromisedId();
        node_id_t promised_node_id = acceptor_state_->GetPromisedNodeId();

        pax_reply_message.SetPromisedId(promised_id);
        pax_reply_message.SetPromisedNodeId(promised_node_id);
    }

    // reply
    ReplyProposer(pax_reply_message, proposer_id, MessageType::PreProposeReply);
}

void paxosme::PaxAcceptor::HandleProposeRequest(const paxosme::PaxMessage& message) {
    PaxAcceptorReplyMessage pax_reply_message;
    pax_reply_message.SetProposerId(message.GetProposerId());
    pax_reply_message.SetReplierId(GetNodeId());

    proposal_id_t proposal_id = message.GetProposalId();
    proposal_id_t proposer_id = message.GetProposerId();

    proposal_id_t promised_proposal_id = acceptor_state_->GetPromisedId();
    node_id_t promised_proposer = acceptor_state_->GetPromisedNodeId();
    if (proposal_id > promised_proposal_id || proposal_id == promised_proposal_id && proposer_id > promised_proposer) {
        proposal_id_t accepted_proposal_id = acceptor_state_->GetAcceptedProposalId();

        {
            /**
             * "For any v and n, if a proposal with value v and number n is issued,
             * then there is a set S consisting of a majority of acceptors such that either (a) no acceptor in S
             * has accepted any proposal numbered less than n, or (b) v is the value of the highest-numbered proposal
             * among all proposals numbered less than n accepted by the acceptors in S."
             */
            if (accepted_proposal_id != 0) {
                // this should not happen with Non-Byzantium
            }
        }

        // update local
        acceptor_state_->SetPromisedProposal(proposer_id, proposer_id);
        acceptor_state_->SetAcceptedProposal(proposal_id, proposer_id);
        acceptor_state_->SetAcceptedValue(message.GetLogValue());

        // an acceptor must remember this information even if it fails and then restarts.
        Persist(message.GetInstanceId(), acceptor_state_->GetAcceptedValue());

    } else {
        pax_reply_message.SetIsRejected(true);
        pax_reply_message.SetPromisedId(promised_proposal_id);
    }

    // reply
    ReplyProposer(pax_reply_message, proposer_id, MessageType::ProposeReply);
}

void paxosme::PaxAcceptor::ReplyProposer(const PaxAcceptorReplyMessage &pax_acceptor_reply_message,
                                         node_id_t proposer_id, MessageType request_type) {
    Send(pax_acceptor_reply_message, proposer_id, request_type);
}

bool paxosme::PaxAcceptor::IsAccepted() {
    return acceptor_state_->GetAcceptedNodeId() > 0;
}
