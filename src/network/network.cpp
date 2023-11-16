//
// Created by k s on 2022/3/27.
//

#include "network_server.hpp"
#include "network_client.hpp"
#include "messages.hpp"
#include "network.hpp"
#include <glog/logging.h>

#include <memory>
#include <unordered_map>

namespace paxosme
{

    inline static PodMsg PODMessage(const PaxMessage &message)
    {
        return PodMsg{};
    }

    class NetworkImpl : public Network
    {
        friend class Network;

        ~NetworkImpl()
        {
            for (auto &server : servers_)
            {
                server.second->Shutdown();
                server.second.reset();
            }

            servers_.clear();
        }

    private:
        std::unordered_map<size_t, std::shared_ptr<NetworkClient>> clients_;

        std::unordered_map<node_id_t, std::unique_ptr<NetworkServer>> servers_;
    };

    void Network::Quit(node_id_t node_id)
    {
        auto *impl = (NetworkImpl *)this;

        if (impl->servers_.find(node_id) == impl->servers_.end())
        {
            impl->servers_[node_id]->Shutdown();
            impl->servers_[node_id].reset();
            impl->servers_.erase(node_id);
        }
    }

    void Network::Start(Network::NetworkOptions &network_options)
    {
        auto *impl = (NetworkImpl *)this;
        for (const auto &peer : network_options.peers)
        {
            auto hash = PeerHash::peerHash(peer);
            impl->clients_.emplace(hash, NetworkClient::New(peer));
        }

        // self server
        //        auto msgCallback = [impl](PaxMessage message) { impl->paxController_->AddMessage(message); };
        auto msgCallback = network_options.msgCallback;

        for (auto &server : impl->servers_)
        {
            server.second->Start(msgCallback);
        }
    }

    Network *Network::New(const PeerList &peers)
    {
        LOG(INFO) << "Network::New()";
        auto nwImpl = new NetworkImpl;
        for (auto peer : peers)
        {
            nwImpl->servers_.emplace(PeerToNodeId(peer), std::move(std::unique_ptr<NetworkServer>(NetworkServer::New(peer))));
        }

        return nwImpl;
    }

    template <>
    void Network::Send<PaxMessage>(const paxosme::Peer &peer, const PaxMessage &message)
    {
        auto *impl = (NetworkImpl *)this;
        size_t hash = PeerHash::peerHash(peer);

        if (impl->clients_.find(hash) == impl->clients_.end())
        {
            std::shared_ptr<NetworkClient> client(NetworkClient::New(peer));
            impl->clients_.emplace(hash, client);
            client->Send(PODMessage(message));
            return;
        }

        impl->clients_[hash]->Send(PODMessage(message));
    }

    template <>
    void Network::Broadcast<PaxMessage>(const PaxMessage &message)
    {
        auto *impl = (NetworkImpl *)this;

        for (auto &x : impl->clients_)
        {
            x.second->Send(PODMessage(message));
        }
    }

    PeerHash PeerHash::peerHash = PeerHash();
}
