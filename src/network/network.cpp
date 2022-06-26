//
// Created by k s on 2022/3/27.
//

#include "network_impl.hpp"
#include "server.hpp"
#include <memory>

namespace paxosme {

    // todo I: another way for network inteface

    void Network::Join(node_id_t node_id) {
        auto *impl = (NetworkImpl *) this;
        auto peer = NodeIdToEndpoint(node_id);
        impl->peers_.Add(peer);
    }

    void Network::Quit(node_id_t node_id) {
        auto *impl = (NetworkImpl *) this;
        auto peer = NodeIdToEndpoint(node_id);
        impl->peers_.Remove(peer);
    }

    void Network::Start(NodeIdList *node_id_list, const Endpoint &self) {
        auto *impl = (NetworkImpl *) this;
        for (node_id_t nodeId: node_id_list->idList) {
            auto peer = NodeIdToEndpoint(nodeId);
            impl->peers_.Add(peer);
        }

        // self server
        auto msgCallback = [impl](PaxMessage message) { impl->paxController_->AddMessage(message); };
        impl->server_->Start(self, msgCallback);
    }

//    Network *Network::New() {
//        auto nwImpl = new NetworkImpl;
//        nwImpl->server_ = std::unique_ptr<ServerImpl>((ServerImpl *) new GrpcServer);
//
//        return nwImpl;
//    }

    void Network::Delete(Network *network) {
        delete network;
    }
}


