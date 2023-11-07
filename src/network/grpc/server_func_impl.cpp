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
        service_->RequestPropose(&ctx_, &request_, &responder_, cq_.get(), cq_.get(), this);
    }

    template<>
    void CallData<paxos::ProposeRequest, paxos::ProposeReply>::HandleRequest(paxos::ProposeReply &reply) {
        // generate message
//        PaxMessage paxMessage(request_.sender_id(), kMSG_PROPOSE_BROADCAST);
//        paxMessage.SetInstanceId(request_.instance_id());
//        paxMessage.SetProposingNodeId(request_.proposing_node_id());
//        paxMessage.SetProposalId(request_.proposal_id());

        msgCallback_(request_.SerializeAsString());

        // set reply
        reply.set_ack(true);
    }

    /*
     * ProposeAck
     */
    template<>
    void CallData<paxos::ProposeAckRequest, paxos::ProposeAckReply>::InitRequestState() {
        service_->RequestProposeAck(&ctx_, &request_, &responder_, cq_.get(), cq_.get(), this);
    }

    template<>
    void CallData<paxos::ProposeAckRequest, paxos::ProposeAckReply>::HandleRequest(paxos::ProposeAckReply &reply) {
//        PaxMessage paxMessage(request_.sender_id(), kMSG_PROPOSE_ACK);
//
//        paxMessage.SetInstanceId(request_.instance_id());
//        paxMessage.SetProposingNodeId(request_.reply_to());
//        paxMessage.SetProposalId(request_.proposal_id());
//        paxMessage.SetRejected(request_.is_rejected());
//        paxMessage.SetPromisedProposalId(request_.promised_proposal_id());
//        paxMessage.SetPromisedNodeId(request_.promised_node_id());

        msgCallback_(request_.SerializeAsString());
        reply.set_ack(true);
    }


    /*
     * Accept
     */
    template<>
    void CallData<paxos::AcceptRequest, paxos::AcceptReply>::InitRequestState() {
        service_->RequestAccept(&ctx_, &request_, &responder_, cq_.get(), cq_.get(), this);
    }

    template<>
    void CallData<paxos::AcceptRequest, paxos::AcceptReply>::HandleRequest(paxos::AcceptReply &reply) {
//        PaxMessage paxMessage(request_.sender_id(), kMSG_ACCEPT_BROADCAST);
//
//        paxMessage.SetInstanceId(request_.instance_id());
//        paxMessage.SetProposingNodeId(request_.proposing_node_id());
//        paxMessage.SetProposalId(request_.proposal_id());
//        paxMessage.SetProposedLogValue(request_.proposed_log_value());

        msgCallback_(request_.SerializeAsString());
        reply.set_ack(true);
    }

    /*
     * AcceptAck
     */
    template<>
    void CallData<paxos::AcceptAckRequest, paxos::AcceptAckReply>::InitRequestState() {
        service_->RequestAcceptAck(&ctx_, &request_, &responder_, cq_.get(), cq_.get(), this);
    }

    template<>
    void CallData<paxos::AcceptAckRequest, paxos::AcceptAckReply>::HandleRequest(paxos::AcceptAckReply &reply) {
//        PaxMessage paxMessage(request_.sender_id(), kMSG_ACCEPT_ACK);
//
//        paxMessage.SetInstanceId(request_.instance_id());
//        paxMessage.SetProposalId(request_.proposal_id());
//        paxMessage.SetProposingNodeId(request_.proposing_node_id());
//        paxMessage.SetRejected(request_.rejected());
//        paxMessage.SetPromisedNodeId(request_.promised_node_id());
//        paxMessage.SetPromisedProposalId(request_.promised_proposal_id());

        msgCallback_(request_.SerializeAsString());
        reply.set_ack(true);
    }

    template<>
    void CallData<paxos::NewValueChosenRequest, paxos::NewValueChosenReply>::InitRequestState() {
        service_->RequestNewValueChosen(&ctx_, &request_, &responder_, cq_.get(),cq_.get(),this);
    }

    template<>
    void CallData<paxos::NewValueChosenRequest, paxos::NewValueChosenReply>::HandleRequest(paxos::NewValueChosenReply &reply) {
//        PaxMessage paxMessage(request_.sender_id(), kMSG_VALUE_CHOSEN_BROADCAST);
//
//        paxMessage.SetInstanceId(request_.instance_id());
//        paxMessage.SetChosenValue(request_.chosen_value());
//        paxMessage.SetProposalId(request_.proposal_id());
//        paxMessage.SetProposingNodeId(request_.proposing_node_id());

        msgCallback_(request_.SerializeAsString());
        reply.set_ack(true);
    }

    template<>
    void CallData<paxos::ShallILearnRequest, paxos::ShallILearnReply>::HandleRequest(paxos::ShallILearnReply &reply) {
//        PaxMessage paxMessage(request_.sender_id(), kMSG_SHALL_I_LEARN);
//        paxMessage.SetInstanceId(request_.instance_id());

        msgCallback_(request_.SerializeAsString());
        reply.set_ack(true);
    }

    template<>
    void CallData<paxos::LearnerValueSyncRequest, paxos::LearnerValueSyncReply>::HandleRequest(paxos::LearnerValueSyncReply &reply) {
//        PaxMessage paxMessage(request_.sender_id(), kMSG_LEARNER_VALUE_SYNC);
//        paxMessage.SetInstanceId(request_.instance_id());
//        paxMessage.SetChosenValue(request_.value());
//        paxMessage.SetProposalId(request_.proposal_id());
//        paxMessage.SetProposingNodeId(request_.proposing_node_id());

        msgCallback_(request_.SerializeAsString());
        reply.set_ack(true);
    }

    template<>
    void CallData<paxos::LearnerValueSendRequest, paxos::LearnerValueSendReply>::HandleRequest(
            paxos::LearnerValueSendReply &reply) {
//        PaxMessage paxMessage(request_.sender_id(), kMSG_LEARNER_VALUE_SEND);
//
//        paxMessage.SetInstanceId(request_.instance_id());
//        paxMessage.SetChosenValue(request_.value());
//        paxMessage.SetProposalId(request_.proposal_id());
//        paxMessage.SetProposingNodeId(request_.proposing_node_id());

        msgCallback_(request_.SerializeAsString());
        reply.set_ack(true);
    }

    template<>
    void CallData<paxos::AckSyncValueRequest, paxos::AckSyncValueReply>::HandleRequest(paxos::AckSyncValueReply &reply) {
//        PaxMessage paxMessage(request_.sender_id(), kMSG_SYNC_VALUE_ACK);
//        paxMessage.SetInstanceId(request_.instance_id());

        msgCallback_(request_.SerializeAsString());
        reply.set_ack(true);
    }

    template<>
    void CallData<paxos::TellInstanceIdRequest, paxos::TellInstanceIdReply>::HandleRequest(paxos::TellInstanceIdReply &reply) {
//        PaxMessage paxMessage(request_.sender_id(), kMSG_TELL_INSTANCE_ID);
//        paxMessage.SetInstanceId(request_.follower_instance_id());
//        paxMessage.SetLeaderInstanceId(request_.leader_instance_id());

        msgCallback_(request_.SerializeAsString());
        reply.set_ack(true);
    }

    template<>
    void CallData<paxos::ConfirmLearnRequest, paxos::ConfirmLearnReply>::HandleRequest(paxos::ConfirmLearnReply &reply) {
//        PaxMessage paxMessage(request_.sender_id(), kMSG_TELL_INSTANCE_ID);
//        paxMessage.SetInstanceId(request_.instance_id());

        msgCallback_(request_.SerializeAsString());
        reply.set_ack(true);
    }
}