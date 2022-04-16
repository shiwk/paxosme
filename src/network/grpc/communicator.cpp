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
                proposeAckRequest.set_is_rejected(pax_message.IsRejected());
                proposeAckRequest.set_instance_id(pax_message.GetInstanceId());
                proposeAckRequest.set_promised_id(pax_message.GetPromisedId());
                proposeAckRequest.set_promised_node_id(pax_message.GetPromisedNodeId());
                proposeAckRequest.set_accepted_log_value(pax_message.GetAcceptedValue());

                paxos::ProposeAckReply proposeAckReply;
                return Send(node_id, proposeAckRequest, proposeAckReply);
            }
            case kMSG_ACCEPT_ACK: {
                paxos::AcceptAckRequest acceptAckRequest;
                acceptAckRequest.set_instance_id(pax_message.GetInstanceId());
                acceptAckRequest.set_promised_node_id(pax_message.GetPromisedNodeId());
                acceptAckRequest.set_promised_id(pax_message.GetPromisedId());
                acceptAckRequest.set_accepted_id(pax_message.GetAcceptedId());
                acceptAckRequest.set_accepted_value(pax_message.GetAcceptedValue());
                acceptAckRequest.set_rejected(pax_message.IsRejected());

                paxos::AcceptAckReply acceptAckReply;
                return Send(node_id, acceptAckRequest, acceptAckReply);
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
                proposeRequest.set_instance_id(pax_message.GetInstanceId());
                proposeRequest.set_proposal_id(pax_message.GetProposalId());
                proposeRequest.set_proposer_id(pax_message.GetProposer());

                Broadcast<paxos::ProposeRequest, paxos::ProposeReply>(proposeRequest);
                break;
            }

            case kMSG_ACCEPT_BROADCAST: {
                paxos::AcceptRequest acceptRequest;
                acceptRequest.set_proposer_id(pax_message.GetProposer());
                acceptRequest.set_proposal_id(pax_message.GetProposalId());
                acceptRequest.set_instance_id(pax_message.GetInstanceId());
                acceptRequest.set_proposed_log_value(pax_message.GetProposedLogValue());

                paxos::AcceptReply acceptReply;
                Broadcast<paxos::AcceptRequest, paxos::AcceptReply>(acceptRequest);
                break;
            }

            default:
                break;
        }

        return 1;

        //todo I: more cases
    }
}