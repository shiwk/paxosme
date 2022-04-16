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
        PaxMessage paxMessage(request_.proposer_id(), kMSG_PROPOSE_BROADCAST);
        paxMessage.SetInstanceId(request_.instance_id());
        paxMessage.SetProposer(request_.proposer_id());
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
        paxMessage.SetPromisedId(request_.promised_id());
        paxMessage.SetPromisedNodeId(request_.promised_node_id());

        controller_->AddMessage(paxMessage);
        reply.set_ack(true);
    }
}