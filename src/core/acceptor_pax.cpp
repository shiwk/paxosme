//
// Created by shiwk on 2020/7/18.
//

#include <acceptor_pax.hpp>

/**
 * Acceptor processing when received prepare request
 * @param message
 */
void paxosme::PaxAcceptor::HandlePrepareRequest(const paxosme::PaxMessage &message) {

    /**
     * "An acceptor can accept a proposal numbered n iff
     * it has not responded to a prepare request having a number greater than n."
     */

    PaxAcceptorReplyMessage pax_reply_message{};
    pax_reply_message.SetProposerId(message.GetProposer());
    pax_reply_message.SetReplierId(GetNodeId());

    proposal_id_t proposal_id = message.GetProposalId();
    proposal_id_t proposer = message.GetProposer();


    if (IsHigherThanPromised(proposer, proposal_id)) {

        /**
         * "If an acceptor receives a prepare request with number n greater than that of any prepare request to
         * which it has already responded, then it responds to the request with a promise not to accept any
         * more proposals numbered less than n and with the highest-numbered proposal (if any) that it has accepted."
         */

        // higher than promised, then accept
        pax_reply_message.SetIsRejected(false);
        proposal_id_t accepted_proposal_id = acceptor_state_->GetAcceptedProposal();

        if (accepted_proposal_id != 0) {
            pax_reply_message.SetAcceptedId(accepted_proposal_id);
            pax_reply_message.SetAcceptedValue(GetAcceptedValue());
        }

        UpdatePromised(proposer, proposal_id);

    } else {

        // lower than promised, then reject
        pax_reply_message.SetIsRejected(true);
        proposal_id_t promised_id = acceptor_state_->GetPromisedProposal();
        node_id_t promised_node_id = acceptor_state_->GetPromisedProposer();

        pax_reply_message.SetPromisedId(promised_id);
        pax_reply_message.SetPromisedNodeId(promised_node_id);
    }

    // reply
    ReplyProposer(pax_reply_message, MessageType::kPrepareReply);
}

void paxosme::PaxAcceptor::HandleProposeRequest(const paxosme::PaxMessage &message) {
    PaxAcceptorReplyMessage pax_reply_message{};
    pax_reply_message.SetProposerId(message.GetProposer());
    pax_reply_message.SetReplierId(GetNodeId());

    proposal_id_t proposal_id = message.GetProposalId();
    proposal_id_t proposer = message.GetProposer();

    if (IsHigherThanPromised(proposer, proposal_id)) {

        /**
         * "For any v and n, if a proposal with value v and number n is issued,
         * then there is a set S consisting of a majority of acceptors such that either (a) no acceptor in S
         * has accepted any proposal numbered less than n, or (b) v is the value of the highest-numbered proposal
         * among all proposals numbered less than n accepted by the acceptors in S."
         */

        // update local
        acceptor_state_->SetPromisedProposal(proposal_id, proposer);
        acceptor_state_->SetAcceptedProposal(proposal_id, proposer);
        acceptor_state_->SetAcceptedValue(message.GetProposedLogValue());

        UpdatePromised(proposer, proposal_id);

    } else {
        pax_reply_message.SetIsRejected(true);
        pax_reply_message.SetPromisedId(acceptor_state_->GetPromisedProposal());
    }

    // reply
    ReplyProposer(pax_reply_message, MessageType::kProposeReply);
}

void paxosme::PaxAcceptor::ReplyProposer(const PaxAcceptorReplyMessage &reply, MessageType request_type) {
    PaxMessage pax_message(reply.GetReplierId(), request_type);
    pax_message.SetInstanceId(reply.GetInstanceId());
    pax_message.SetPromisedNodeId(reply.GetPromisedNodeId());
    pax_message.SetPromisedId(reply.GetPromisedId());
    pax_message.SetAcceptedId(reply.GetAcceptedId());
    pax_message.SetAcceptedValue(reply.GetAcceptedValue());
    pax_message.SetRejected(reply.IsRejected());

    SendMessage(pax_message, reply.GetProposerId());
}

bool paxosme::PaxAcceptor::IsAccepted() {
    return acceptor_state_->GetAcceptedNodeId() > 0;
}

bool paxosme::PaxAcceptor::IsHigherThanPromised(node_id_t proposer, proposal_id_t proposal_id) {
    proposal_id_t promised_proposal_id = acceptor_state_->GetPromisedProposal();
    node_id_t promised_proposer = acceptor_state_->GetPromisedProposer();
    return proposal_id > promised_proposal_id ||
           proposal_id == promised_proposal_id &&
           proposer > promised_proposer;
}

void paxosme::PaxAcceptor::UpdatePromised(node_id_t proposer, proposal_id_t proposal_id) {
    PaxosState paxos_state;

    acceptor_state_->SetPromisedProposal(proposal_id, proposer);

    paxos_state.set_instance_id(GetInstanceId());
    paxos_state.set_promised_node_id(proposer);
    paxos_state.set_promised_proposal_id(proposal_id);

    if (acceptor_state_->GetAcceptedProposal()) {
        paxos_state.set_accepted_node_id(proposer);
        paxos_state.set_accepted_proposal_id(acceptor_state_->GetAcceptedProposal());
        LogValue accepted_value = acceptor_state_->GetAcceptedValue();
        paxos_state.set_accepted_value(accepted_value.GetValue());
    }

    // an acceptor must remember this information even if it fails and then restarts.
    WriteState(paxos_state);
}

const paxosme::LogValue &paxosme::PaxAcceptor::GetAcceptedValue() {
    return acceptor_state_->GetAcceptedValue();
}

proposal_id_t paxosme::PaxAcceptor::GetAcceptedProposal() {
    return acceptor_state_->GetAcceptedProposal();
}

node_id_t paxosme::PaxAcceptor::GetAcceptedNodeId() {
    return acceptor_state_->GetAcceptedNodeId();
}

instance_id_t paxosme::PaxAcceptor::init() {
    PaxosState paxos_state = ReadState(-1);
    acceptor_state_->init(paxos_state);
    return paxos_state.instance_id();
}

void paxosme::AcceptorState::init(const PaxosState &state) {
    LogValue log_value{state.accepted_value()};
    SetAcceptedValue(log_value);
    SetAcceptedProposal(state.accepted_proposal_id(), state.accepted_node_id());
    SetPromisedProposal(state.promised_proposal_id(), state.promised_node_id());
    SetInstanceId(state.instance_id());
}

instance_id_t paxosme::AcceptorState::GetInstanceId() const {
    return instance_id_;
}

void paxosme::AcceptorState::SetInstanceId(instance_id_t instance_id) {
    instance_id_ = instance_id;
}