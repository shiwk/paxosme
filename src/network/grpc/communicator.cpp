//
// Created by shiwk on 2021/3/11.
//

#include <grpcpp/create_channel.h>
#include <ares.h>
#include <libc.h>
#include <communicator.hpp>

namespace paxosme {

    int Communicator::Send(node_id_t node_id, const paxosme::PaxMessage &pax_message) {

        switch (pax_message.GetMessageType()) {
            case kMSG_PROPOSE_ACK: {
                paxos::ProposeAckRequest proposeAckRequest;
                proposeAckRequest.set_sender_id(pax_message.GetSender());
                proposeAckRequest.set_is_rejected(pax_message.IsRejected());
                proposeAckRequest.set_instance_id(pax_message.GetInstanceId());
                proposeAckRequest.set_promised_proposal_id(pax_message.GetPromisedProposalId());
                proposeAckRequest.set_promised_node_id(pax_message.GetPromisedNodeId());
                proposeAckRequest.set_accepted_log_value(pax_message.GetAcceptedValue());

                return Send<paxos::ProposeAckRequest, paxos::ProposeAckReply>(node_id, proposeAckRequest);
            }
            case kMSG_ACCEPT_ACK: {
                paxos::AcceptAckRequest acceptAckRequest;
                acceptAckRequest.set_sender_id(pax_message.GetSender());
                acceptAckRequest.set_instance_id(pax_message.GetInstanceId());
                acceptAckRequest.set_promised_node_id(pax_message.GetPromisedNodeId());
                acceptAckRequest.set_promised_proposal_id(pax_message.GetPromisedProposalId());
                acceptAckRequest.set_accepted_id(pax_message.GetAcceptedProposal());
                acceptAckRequest.set_accepted_value(pax_message.GetAcceptedValue());
                acceptAckRequest.set_rejected(pax_message.IsRejected());

                return Send<paxos::AcceptAckRequest, paxos::AcceptAckReply>(node_id, acceptAckRequest);
            }

            case kMSG_LEARNER_VALUE_SYNC: {
                paxos::LearnerValueSyncRequest learnerValueSyncRequest;
                learnerValueSyncRequest.set_instance_id(pax_message.GetInstanceId());
                learnerValueSyncRequest.set_sender_id(pax_message.GetSender());
                learnerValueSyncRequest.set_value(pax_message.GetChosenValue());
                learnerValueSyncRequest.set_proposing_node_id(pax_message.GetProposingNodeId());
                learnerValueSyncRequest.set_proposal_id(pax_message.GetProposalId());

                return Send<paxos::LearnerValueSyncRequest, paxos::LearnerValueSyncReply>(node_id,
                                                                                          learnerValueSyncRequest);
            }

            case kMSG_LEARNER_VALUE_SEND: {
                paxos::LearnerValueSendRequest learnerValueSendRequest;
                learnerValueSendRequest.set_instance_id(pax_message.GetInstanceId());
                learnerValueSendRequest.set_sender_id(pax_message.GetSender());
                learnerValueSendRequest.set_value(pax_message.GetChosenValue());
                learnerValueSendRequest.set_proposing_node_id(pax_message.GetProposingNodeId());
                learnerValueSendRequest.set_proposal_id(pax_message.GetProposalId());

                return Send<paxos::LearnerValueSendRequest, paxos::LearnerValueSendReply>(node_id,
                                                                                          learnerValueSendRequest);
            }

            case kMSG_SYNC_VALUE_ACK : {
                paxos::AckSyncValueRequest ackSyncValueRequest;
                ackSyncValueRequest.set_instance_id(pax_message.GetInstanceId());
                ackSyncValueRequest.set_sender_id(pax_message.GetSender());

                return Send<paxos::AckSyncValueRequest, paxos::AckSyncValueReply>(node_id, ackSyncValueRequest);
            };

            case kMSG_TELL_INSTANCE_ID: {
                paxos::TellInstanceIdRequest tellInstanceIdRequest;
                tellInstanceIdRequest.set_follower_instance_id(pax_message.GetInstanceId());
                tellInstanceIdRequest.set_leader_instance_id(pax_message.GetLeaderInstanceId());
                tellInstanceIdRequest.set_sender_id(pax_message.GetSender());

                return Send<paxos::TellInstanceIdRequest, paxos::TellInstanceIdReply>(node_id, tellInstanceIdRequest);
            }

            case kMSG_CONFIRM_LEARN:{
                paxos::ConfirmLearnRequest confirmLearnRequest;
                confirmLearnRequest.set_instance_id(pax_message.GetInstanceId());
                confirmLearnRequest.set_sender_id(pax_message.GetSender());

                return Send<paxos::ConfirmLearnRequest, paxos::ConfirmLearnReply>(node_id, confirmLearnRequest);
            }


                //todo I: more cases
            default:
                break;
        }


        return -1;

    }

    Communicator::Communicator(std::vector<node_id_t> &nodes) {
        for (node_id_t node: nodes) {
            auto host = ParseNodeId(node);
            auto client = NewClient(host);
            clientTable_[node] = client;
        }
    }

    std::string Communicator::ParseNodeId(node_id_t node_id) {
        std::string port = std::to_string(node_id & (0xffff)); // 2 bytes for port
        in_addr addr{(uint32_t) node_id >> 2};
        auto tar = std::string(inet_ntoa(addr)).append(port);
        return tar;
    }

    std::shared_ptr<GrpcClient> Communicator::NewClient(std::string &host) {
        return std::shared_ptr<GrpcClient>(
                new GrpcClient(grpc::CreateChannel(host, grpc::InsecureChannelCredentials())));
    }

    int Communicator::Broadcast(const PaxMessage &pax_message) {
        switch (pax_message.GetMessageType()) {
            case kMSG_PROPOSE_BROADCAST: {
                paxos::ProposeRequest proposeRequest;
                proposeRequest.set_sender_id(pax_message.GetSender());
                proposeRequest.set_instance_id(pax_message.GetInstanceId());
                proposeRequest.set_proposal_id(pax_message.GetProposalId());
                proposeRequest.set_proposing_node_id(pax_message.GetProposingNodeId());

                Broadcast<paxos::ProposeRequest, paxos::ProposeReply>(proposeRequest);
                break;
            }

            case kMSG_ACCEPT_BROADCAST: {
                paxos::AcceptRequest acceptRequest;
                acceptRequest.set_sender_id(pax_message.GetSender());
                acceptRequest.set_proposing_node_id(pax_message.GetProposingNodeId());
                acceptRequest.set_proposal_id(pax_message.GetProposalId());
                acceptRequest.set_instance_id(pax_message.GetInstanceId());
                acceptRequest.set_proposed_log_value(pax_message.GetProposedLogValue());

                paxos::AcceptReply acceptReply;
                Broadcast<paxos::AcceptRequest, paxos::AcceptReply>(acceptRequest);
                break;
            }

            case kMSG_VALUE_CHOSEN_BROADCAST: {
                paxos::NewValueChosenRequest newValueChosenRequest;
                newValueChosenRequest.set_sender_id(pax_message.GetSender());
                newValueChosenRequest.set_proposing_node_id(pax_message.GetProposingNodeId());
                newValueChosenRequest.set_instance_id(pax_message.GetInstanceId());
                newValueChosenRequest.set_chosen_value(pax_message.GetChosenValue());
                newValueChosenRequest.set_proposal_id(pax_message.GetProposalId());

                Broadcast<paxos::NewValueChosenRequest, paxos::NewValueChosenReply>(newValueChosenRequest);
                break;
            }

            case kMSG_SHALL_I_LEARN: {
                paxos::ShallILearnRequest shallILearnRequest;
                shallILearnRequest.set_sender_id(pax_message.GetSender());
                shallILearnRequest.set_instance_id(pax_message.GetInstanceId());

                Broadcast<paxos::ShallILearnRequest, paxos::ShallILearnReply>(shallILearnRequest);
                break;
            }
            default:
                break;
        }

        return 1;

        //todo I: more cases
    }
}