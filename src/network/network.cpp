//
// Created by k s on 2022/3/27.
//

#include "network_server.hpp"
#include <memory>

namespace paxosme {

    class NetworkImpl : public Network {
        friend class Network;
        ~NetworkImpl() override = default;

    private:
        PeerList peers_;
        std::unique_ptr<NetworkServer> server_;
    };


    void Network::Quit(node_id_t node_id) {
        auto *impl = (NetworkImpl *) this;
        auto peer = NodeIdToEndpoint(node_id);
        impl->peers_.Remove(peer);
    }

    void Network::Start(Network::NetworkOptions &network_options) {
        auto *impl = (NetworkImpl *) this;
        for (node_id_t nodeId: network_options.peers) {
            auto peer = NodeIdToEndpoint(nodeId);
            impl->peers_.Add(peer);
        }

        // self server
//        auto msgCallback = [impl](PaxMessage message) { impl->paxController_->AddMessage(message); };
        auto msgCallback = network_options.msgCallback;
        impl->server_->Start(NodeIdToEndpoint(network_options.self), msgCallback);
    }

    Network *Network::New() {
        auto nwImpl = new NetworkImpl;
        nwImpl->server_ = std::unique_ptr<NetworkServer>((NetworkServer *) NetworkServer::New());

        return nwImpl;
    }
}


