//
// Created by k s on 2022/4/16.
//
#include <server.hpp>

namespace paxosme {
    /*
     * Propose
     */
    template<>
    void CallData<paxos::ProposeRequest, paxos::ProposeReply>::InitRequestState() {
        service_->RequestPropose(&ctx_, &request_, &responder_, cq_, cq_, this);
    }

    template<>
    void CallData<paxos::ProposeRequest, paxos::ProposeReply>::HandleRequest(paxos::ProposeReply &reply) {
        // generate message
        PaxMessage paxMessage(request_.sender_id(), kMSG_PROPOSE_BROADCAST);
        paxMessage.SetInstanceId(request_.instance_id());
        paxMessage.SetProposer(request_.proposing_node_id());
        paxMessage.SetProposalId(request_.proposal_id());

        controller_->AddMessage(paxMessage);

        // set reply
        reply.set_ack(true);
    }

    /*
     * ProposeAck
     */
    template<>
    void CallData<paxos::ProposeAckRequest, paxos::ProposeAckReply>::InitRequestState() {
        service_->RequestProposeAck(&ctx_, &request_, &responder_, cq_, cq_, this);
    }

    template<>
    void CallData<paxos::ProposeAckRequest, paxos::ProposeAckReply>::HandleRequest(paxos::ProposeAckReply &reply) {
        PaxMessage paxMessage(request_.sender_id(), kMSG_PROPOSE_ACK);

        paxMessage.SetInstanceId(request_.instance_id());
        paxMessage.SetProposer(request_.reply_to());
        paxMessage.SetProposalId(request_.proposal_id());
        paxMessage.SetRejected(request_.is_rejected());
        paxMessage.SetPromisedProposalId(request_.promised_proposal_id());
        paxMessage.SetPromisedNodeId(request_.promised_node_id());

        controller_->AddMessage(paxMessage);
        reply.set_ack(true);
    }


    /*
     * Accept
     */
    template<>
    void CallData<paxos::AcceptRequest, paxos::AcceptReply>::InitRequestState() {
        service_->RequestAccept(&ctx_, &request_, &responder_, cq_, cq_, this);
    }

    template<>
    void CallData<paxos::AcceptRequest, paxos::AcceptReply>::HandleRequest(paxos::AcceptReply &reply) {
        PaxMessage paxMessage(request_.sender_id(), kMSG_ACCEPT_BROADCAST);

        paxMessage.SetInstanceId(request_.instance_id());
        paxMessage.SetProposer(request_.proposing_node_id());
        paxMessage.SetProposalId(request_.proposal_id());
        paxMessage.SetProposedLogValue(request_.proposed_log_value());

        controller_->AddMessage(paxMessage);
        reply.set_ack(true);
    }

    /*
     * AcceptAck
     */
    template<>
    void CallData<paxos::AcceptAckRequest, paxos::AcceptAckReply>::InitRequestState() {
        service_->RequestAcceptAck(&ctx_, &request_, &responder_, cq_, cq_, this);
    }

    template<>
    void CallData<paxos::AcceptAckRequest, paxos::AcceptAckReply>::HandleRequest(paxos::AcceptAckReply &reply) {
        PaxMessage paxMessage(request_.sender_id(), kMSG_ACCEPT_ACK);

        paxMessage.SetInstanceId(request_.instance_id());
        paxMessage.SetProposalId(request_.proposal_id());
        paxMessage.SetProposer(request_.proposing_node_id());
        paxMessage.SetRejected(request_.rejected());
        paxMessage.SetPromisedNodeId(request_.promised_node_id());
        paxMessage.SetPromisedProposalId(request_.promised_proposal_id());

        controller_->AddMessage(paxMessage);
        reply.set_ack(true);
    }
}