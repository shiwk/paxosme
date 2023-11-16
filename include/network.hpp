//
// Created by k s on 2022/5/22.
//

#ifndef PAXOSME_NETWORK_HPP
#define PAXOSME_NETWORK_HPP

#include <string>
#include <unordered_set>
#include <vector>
#include "common.hpp"
#include <netinet/in.h> // in_addr
#include <arpa/inet.h>  // inet_ntoa

using node_id_vector = std::vector<node_id_t>;

namespace paxosme
{
    struct Peer
    {
        std::string ip;
        uint32_t port;

        std::string ToString() const
        {
            return ip + ":" + std::to_string(port);
        }

        bool operator==(const Peer &peer) const
        {
            return this->port == peer.port && this->ip == peer.ip;
        }
    };

    struct PeerHash
    {
        size_t operator()(const Peer &peer) const
        {
            const int PRIME_CONST = 31;
            size_t hashCode = 0;
            auto st = peer.ToString();
            for (int i = 0; i < st.length(); i++)
            {
                hashCode += st[i] * pow(PRIME_CONST, i);
            }
            return hashCode;
        }

        static PeerHash peerHash;
    };

    using PeerList = std::vector<Peer>;

    struct PeerSet
    {
        PeerSet(std::vector<Peer> nodeList) : peers(nodeList.begin(), nodeList.end())
        {
        }
        void Add(const Peer &peer)
        {
            peers.insert(peer);
        }

        bool Contains(const Peer &peer)
        {
            return peers.find(peer) != peers.end();
        }

        bool Remove(const Peer &peer)
        {
            return peers.erase(peer);
        }

        auto begin() { return peers.begin(); }
        auto end() { return peers.end(); }

    private:
        std::unordered_set<Peer, PeerHash> peers;
    };

    class Network
    {
    public:
        using MsgCallback = std::function<void(const std::string)>;

        struct NetworkOptions
        {
            PeerSet peers;
            node_id_t self;
            MsgCallback msgCallback;
        };
        Network() = default;

        static Peer NodeIdToPeer(node_id_t node_id)
        {
            in_addr addr{(uint32_t)(node_id >> 32)};
            return Peer{std::string(inet_ntoa(addr)), (uint32_t)(node_id & 0xffffffff)};
        }

        static node_id_t PeerToNodeId(const Peer &peer)
        {
            in_addr_t addr = inet_addr(peer.ip.c_str());
            assert(addr != (uint32_t)-1);
            return (((uint64_t)addr) << 32) | peer.port;
        }

        static Network *New(const PeerList &peers);
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
