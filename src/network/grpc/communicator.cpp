//
// Created by shiwk on 2021/3/11.
//

#include <grpcpp/create_channel.h>
#include <ares.h>
#include <libc.h>
#include <communicator.hpp>

namespace paxosme {

    int Communicator::Send(node_id_t node_id, const paxosme::PaxMessage &pax_message) {
        if (clientTable_.find(node_id) == clientTable_.end())
            return -1;


        if (pax_message.GetMessageType() == kMSG_PROPOSE_BROADCAST) {
            bool request = clientTable_[node_id]->Propose(pax_message);
            return request ? 0 : -1;
        }
        else if (pax_message.GetMessageType() == kMSG_ACCEPT_BROADCAST){
            bool request = clientTable_[node_id]->Accept(pax_message);
            return request ? 0 : -1;
        }

        return -1;

    }

//    int Communicator::Receive(const paxosme::PaxMessage &pax_message) {
//        return 0;
//    }

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
        return 0;
    }

}