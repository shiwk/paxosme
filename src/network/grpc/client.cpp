
//
// Created by shiwk on 2021/3/11.
//

#include <client.hpp>
#include "network.hpp"
#include "grpcpp/create_channel.h"
#include <glog/logging.h>

template <class TResponse>
using ResponseReader = std::unique_ptr<::grpc::ClientAsyncResponseReader<TResponse>>;

namespace paxosme
{

    bool GrpcClient::Send(const PodMsg &pod_msg)
    {
        bool result = false;
        switch (pod_msg.messageType)
        {
        case kPOD_MSG_PROPOSE_ACK:
        {
            paxos::ProposeAckRequest proposeAckRequest;
            proposeAckRequest.set_sender_id(pod_msg.senderId);
            proposeAckRequest.set_is_rejected(pod_msg.rejected);
            proposeAckRequest.set_instance_id(pod_msg.instanceId);
            proposeAckRequest.set_promised_proposal_id(pod_msg.promisedProposalId);
            proposeAckRequest.set_promised_node_id(pod_msg.promisedNodeId);
            proposeAckRequest.set_accepted_log_value(pod_msg.acceptedValue);

            result = Send<paxos::ProposeAckRequest, paxos::ProposeAckReply>(proposeAckRequest);
            break;
        }
        case kPOD_MSG_ACCEPT_ACK:
        {
            paxos::AcceptAckRequest acceptAckRequest;
            acceptAckRequest.set_sender_id(pod_msg.senderId);
            acceptAckRequest.set_instance_id(pod_msg.instanceId);
            acceptAckRequest.set_promised_node_id(pod_msg.promisedNodeId);
            acceptAckRequest.set_promised_proposal_id(pod_msg.promisedProposalId);
            acceptAckRequest.set_accepted_id(pod_msg.acceptedProposalId);
            acceptAckRequest.set_accepted_value(pod_msg.acceptedValue);
            acceptAckRequest.set_rejected(pod_msg.rejected);

            result = Send<paxos::AcceptAckRequest, paxos::AcceptAckReply>(acceptAckRequest);
            break;
        }

        case kPOD_MSG_LEARNER_VALUE_SYNC:
        {
            paxos::LearnerValueSyncRequest learnerValueSyncRequest;
            learnerValueSyncRequest.set_instance_id(pod_msg.instanceId);
            learnerValueSyncRequest.set_sender_id(pod_msg.senderId);
            learnerValueSyncRequest.set_value(pod_msg.chosenValue);
            learnerValueSyncRequest.set_proposing_node_id(pod_msg.proposingNodeId);
            learnerValueSyncRequest.set_proposal_id(pod_msg.proposalId);

            result = Send<paxos::LearnerValueSyncRequest, paxos::LearnerValueSyncReply>(
                learnerValueSyncRequest);
            break;
        }

        case kPOD_MSG_LEARNER_VALUE_SEND:
        {
            paxos::LearnerValueSendRequest learnerValueSendRequest;
            learnerValueSendRequest.set_instance_id(pod_msg.instanceId);
            learnerValueSendRequest.set_sender_id(pod_msg.senderId);
            learnerValueSendRequest.set_value(pod_msg.chosenValue);
            learnerValueSendRequest.set_proposing_node_id(pod_msg.proposingNodeId);
            learnerValueSendRequest.set_proposal_id(pod_msg.proposalId);

            result = Send<paxos::LearnerValueSendRequest, paxos::LearnerValueSendReply>(
                learnerValueSendRequest);
            break;
        }

        case kPOD_MSG_SYNC_VALUE_ACK:
        {
            paxos::AckSyncValueRequest ackSyncValueRequest;
            ackSyncValueRequest.set_instance_id(pod_msg.instanceId);
            ackSyncValueRequest.set_sender_id(pod_msg.senderId);

            result = Send<paxos::AckSyncValueRequest, paxos::AckSyncValueReply>(ackSyncValueRequest);
            break;
        };

        case kPOD_MSG_TELL_INSTANCE_ID:
        {
            paxos::TellInstanceIdRequest tellInstanceIdRequest;
            tellInstanceIdRequest.set_follower_instance_id(pod_msg.instanceId);
            tellInstanceIdRequest.set_leader_instance_id(pod_msg.leaderInstanceId);
            tellInstanceIdRequest.set_sender_id(pod_msg.senderId);

            result = Send<paxos::TellInstanceIdRequest, paxos::TellInstanceIdReply>(
                tellInstanceIdRequest);
            break;
        }

        case kPOD_MSG_CONFIRM_LEARN:
        {
            paxos::ConfirmLearnRequest confirmLearnRequest;
            confirmLearnRequest.set_instance_id(pod_msg.instanceId);
            confirmLearnRequest.set_sender_id(pod_msg.senderId);

            result = Send<paxos::ConfirmLearnRequest, paxos::ConfirmLearnReply>(confirmLearnRequest);
            break;
        }

        // todo I: more cases
        default:
            break;
        }

        return result;
    }

    template <>
    ResponseReader<paxos::ProposeReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq, paxos::ProposeRequest &request)
    {
        return stub_->PrepareAsyncPropose(c, request, cq);
    }

    template <>
    ResponseReader<paxos::ProposeAckReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq,
                               paxos::ProposeAckRequest &request)
    {
        return stub_->PrepareAsyncProposeAck(c, request, cq);
    }

    template <>
    ResponseReader<paxos::AcceptReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq, paxos::AcceptRequest &request)
    {
        return stub_->PrepareAsyncAccept(c, request, cq);
    }

    template <>
    ResponseReader<paxos::AcceptAckReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq,
                               paxos::AcceptAckRequest &request)
    {
        return stub_->PrepareAsyncAcceptAck(c, request, cq);
    }

    template <>
    ResponseReader<paxos::NewValueChosenReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq,
                               paxos::NewValueChosenRequest &request)
    {
        return stub_->PrepareAsyncNewValueChosen(c, request, cq);
    }

    template <>
    ResponseReader<paxos::ShallILearnReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq,
                               paxos::ShallILearnRequest &request)
    {
        return stub_->PrepareAsyncShallILearn(c, request, cq);
    }

    template <>
    ResponseReader<paxos::LearnerValueSyncReply>
    GrpcClient::PrepareRequest(::grpc::ClientContext *c, ::grpc::CompletionQueue *cq,
                               paxos::LearnerValueSyncRequest &request)
    {
        return stub_->PrepareAsyncLearnerValueSync(c, request, cq);
    }

    template <>
    ResponseReader<paxos::AckSyncValueReply>
    GrpcClient::PrepareRequest(grpc::ClientContext *c, grpc::CompletionQueue *cq, paxos::AckSyncValueRequest &request)
    {
        return stub_->PrepareAsyncAckSyncValue(c, request, cq);
    }

    template <>
    ResponseReader<paxos::LearnerValueSendReply> GrpcClient::PrepareRequest(::grpc::ClientContext
                                                                                *c,
                                                                            ::grpc::CompletionQueue *cq,
                                                                            paxos::LearnerValueSendRequest &request)
    {
        return stub_->PrepareAsyncLearnerValueSend(c, request, cq);
    }

    template <>
    ResponseReader<paxos::TellInstanceIdReply> GrpcClient::PrepareRequest(::grpc::ClientContext
                                                                              *c,
                                                                          ::grpc::CompletionQueue *cq,
                                                                          paxos::TellInstanceIdRequest &request)
    {
        return stub_->PrepareAsyncTellInstanceId(c, request, cq);
    }

    template <>
    ResponseReader<paxos::ConfirmLearnReply> GrpcClient::PrepareRequest(::grpc::ClientContext
                                                                            *c,
                                                                        ::grpc::CompletionQueue *cq,
                                                                        paxos::ConfirmLearnRequest
                                                                            &request)
    {
        return stub_->PrepareAsyncConfirmLearn(c, request, cq);
    }

    GrpcClient *GrpcClient::NewClient(const std::string &host)
    {
        return new GrpcClient(grpc::CreateChannel(host, grpc::InsecureChannelCredentials()));
    }

    NetworkClient *NetworkClient::New(const Peer &peer)
    {
        return GrpcClient::NewClient(peer.ToString());
    }
}
