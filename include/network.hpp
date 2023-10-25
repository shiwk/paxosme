//
// Created by k s on 2022/5/22.
//

#ifndef PAXOSME_NETWORK_HPP
#define PAXOSME_NETWORK_HPP

#include <string>
#include <unordered_set>
#include <vector>
#include "common.hpp"

using node_id_vector = std::vector<node_id_t>;

namespace paxosme
{
    struct Peer
    {
        std::string ip;
        int port;

        std::string ToString() const
        {
            return ip + ":" + std::to_string(port);
        }

        bool operator==(const Peer &endpoint) const
        {
            return this->port == endpoint.port && this->ip == endpoint.ip;
        }
    };

    struct PeerHash
    {
        size_t operator()(const Peer &endpoint) const
        {
            const int PRIME_CONST = 31;
            size_t hashCode = 0;
            auto st = endpoint.ToString();
            for (int i = 0; i < st.length(); i++)
            {
                hashCode += st[i] * pow(PRIME_CONST, i);
            }
            return hashCode;
        }

        static PeerHash peerHash;
    };

    struct PeerList
    {
        PeerList(std::vector<Peer> nodeList) : peers(nodeList.begin(), nodeList.end())
        {
        }
        void Add(const Peer &endpoint)
        {
            peers.insert(endpoint);
        }

        bool Contains(const Peer &endpoint)
        {
            return peers.find(endpoint) != peers.end();
        }

        bool Remove(const Peer &endpoint)
        {
            return peers.erase(endpoint);
        }

        auto begin() { return peers.begin(); }
        auto end() { return peers.end(); }

    private:
        std::unordered_set<Peer, PeerHash> peers;
    };

    class Network
    {
    public:
        using MsgCallback = std::function<void(std::string)>;

        struct NetworkOptions
        {
            PeerList peers;
            node_id_t self;
            MsgCallback msgCallback;
        };
        Network() = default;

        static Peer NodeIdToPeer(node_id_t node_id)
        {
            // todo I: convert node id to peer
            return Peer{"127.0.0.1", 9999};
        }

        static node_id_t PeerToNodeId(Peer)
        {
            // todo I: convert peer to node id
            return 0;
        }

        static Network *New();
        virtual ~Network() = default;

        //        void Start(const node_id_vector &, const node_id_t &self);
        void Start(NetworkOptions &);

        template <class M>
        void Send(const Peer &, const M &message);

        template <class M>
        void Broadcast(const M &message);

        void Quit(node_id_t);
    };

    class Comm
    {
    };

    template <class M>
    class Communicator : public Comm
    {
    public:
        int SendMessage(node_id_t node_id, const M &message);

        int BroadcastMessage(const M &message);

    private:
        Network *network_;
    };
}

#endif
// PAXOSME_GRPC_NETWORK_HPP
