//
// Created by shiwk on 2021/3/11.
//

#include <grpcpp/create_channel.h>
#include <ares.h>
#include <libc.h>
#include "grpc_comm.hpp"

//namespace paxosme {
//
//
//
//    GrpcComm::GrpcComm(std::vector<node_id_t> &nodes) {
//        for (node_id_t node: nodes) {
//            auto host = ParseNodeId(node);
//            auto client = NewClient(host);
//            clientTable_[node] = client;
//        }
//    }
//
//    std::string GrpcComm::ParseNodeId(node_id_t node_id) {
//        std::string port = std::to_string(node_id & (0xffff)); // 2 bytes for port
//        in_addr addr{(uint32_t) node_id >> 2};
//        auto tar = std::string(inet_ntoa(addr)).append(port);
//        return tar;
//    }
//
//    std::shared_ptr<GrpcClient> GrpcComm::NewClient(std::string &host) {
//        return std::shared_ptr<GrpcClient>(
//                new GrpcClient(grpc::CreateChannel(host, grpc::InsecureChannelCredentials())));
//    }
//
//    void GrpcClient::Broadcast(const PodMsg &pod_msg) {
//        auto *impl = (GrpcComm *) this;
//
//        switch (pod_msg.messageType) {
//            case kMSG_PROPOSE_BROADCAST: {
//                paxos::ProposeRequest proposeRequest;
//                proposeRequest.set_sender_id(pod_msg.senderId);
//                proposeRequest.set_instance_id(pod_msg.instanceId);
//                proposeRequest.set_proposal_id(pod_msg.proposalId);
//                proposeRequest.set_proposing_node_id(pod_msg.proposingNodeId);
//
//                Broadcast<paxos::ProposeRequest, paxos::ProposeReply>(proposeRequest);
//                break;
//            }
//
//            case kMSG_ACCEPT_BROADCAST: {
//                paxos::AcceptRequest acceptRequest;
//                acceptRequest.set_sender_id(pod_msg.senderId);
//                acceptRequest.set_proposing_node_id(pod_msg.proposingNodeId);
//                acceptRequest.set_proposal_id(pod_msg.proposalId);
//                acceptRequest.set_instance_id(pod_msg.instanceId);
//                acceptRequest.set_proposed_log_value(pod_msg.proposedLogValue);
//
//                paxos::AcceptReply acceptReply;
//                impl->Broadcast<paxos::AcceptRequest, paxos::AcceptReply>(acceptRequest);
//                break;
//            }
//
//            case kMSG_VALUE_CHOSEN_BROADCAST: {
//                paxos::NewValueChosenRequest newValueChosenRequest;
//                newValueChosenRequest.set_sender_id(pod_msg.senderId);
//                newValueChosenRequest.set_proposing_node_id(pod_msg.proposingNodeId);
//                newValueChosenRequest.set_instance_id(pod_msg.instanceId);
//                newValueChosenRequest.set_chosen_value(pod_msg.chosenValue);
//                newValueChosenRequest.set_proposal_id(pod_msg.proposalId);
//
//                impl->Broadcast<paxos::NewValueChosenRequest, paxos::NewValueChosenReply>(newValueChosenRequest);
//                break;
//            }
//
//            case kMSG_SHALL_I_LEARN: {
//                paxos::ShallILearnRequest shallILearnRequest;
//                shallILearnRequest.set_sender_id(pod_msg.senderId);
//                shallILearnRequest.set_instance_id(pod_msg.instanceId);
//
//                impl->Broadcast<paxos::ShallILearnRequest, paxos::ShallILearnReply>(shallILearnRequest);
//                break;
//            }
//            default:
//                break;
//        }
//
//        return 1;
//
//        //todo I: more cases
//    }
//}