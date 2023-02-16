//
// Created by k s on 2022/3/27.
//

#include "network_server.hpp"
#include "controller.hpp"
#include "messages_pax.hpp"
#include <memory>

namespace paxosme {

    class NetworkImpl : public Network {
        friend class Network;

    private:
        PeerList peers_;
        std::unique_ptr<NetworkServer> server_;
    protected:
        PaxController *paxController_;
    };

    // todo I: another way for network interface

    void Network::Quit(node_id_t node_id) {
        auto *impl = (NetworkImpl *) this;
        auto peer = NodeIdToEndpoint(node_id);
        impl->peers_.Remove(peer);
    }

    void Network::Start(const node_id_vector& node_id_list, const node_id_t &self) {
        auto *impl = (NetworkImpl *) this;
        for (node_id_t nodeId: node_id_list) {
            auto peer = NodeIdToEndpoint(nodeId);
            impl->peers_.Add(peer);
        }

        // self server
        auto msgCallback = [impl](PaxMessage message) { impl->paxController_->AddMessage(message); };
        impl->server_->Start(NodeIdToEndpoint(self), msgCallback);
    }

    Network *Network::New() {
        auto nwImpl = new NetworkImpl;
        nwImpl->server_ = std::unique_ptr<NetworkServer>((NetworkServer *) NetworkServer::New());

        return nwImpl;
    }

    void Network::Delete(Network *network) {
        delete network;
    }
}


