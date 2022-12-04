//
// Created by k s on 2022/5/22.
//

#ifndef PAXOSME_NETWORK_HPP
#define PAXOSME_NETWORK_HPP

#include <string>
#include <unordered_set>
#include "common.hpp"

using node_id_vector = std::vector<node_id_t>;

namespace paxosme {
    struct Endpoint {
        std::string ip;
        int port;

        std::string ToString() const{
            return ip + ":" + std::to_string(port);
        }

        bool operator== (const Endpoint &endpoint) const {
            return this->port == endpoint.port && this->ip == endpoint.ip;
        }
    };

    struct EndpointHash {
        size_t operator()(const Endpoint &endpoint) const {
            const int PRIME_CONST = 31;
            size_t hashCode = 0;
            auto st = endpoint.ToString();
            for (int i = 0; i < st.length(); i++) {
                hashCode += st[i] * pow(PRIME_CONST, i);
            }
            return hashCode;
        }
    };

    struct PeerList {
        void Add(const Endpoint &endpoint) {
            peers.insert(endpoint);
        }

        bool Contains(const Endpoint &endpoint) {
            return peers.find(endpoint) != peers.end();
        }

        bool Remove(const Endpoint &endpoint) {
            return peers.erase(endpoint);
        }

    private:
        std::unordered_set<Endpoint, EndpointHash> peers;
    };

    class Comm {
    };

    template<class M>
    class Communicator : public Comm {
    public:
        int Send(node_id_t node_id, const M &message);

        int Broadcast(const M &message);
    };

    class Network {
    public:
        Network()=default;

        Endpoint NodeIdToEndpoint(node_id_t) {
            // todo I: convert node id to peer
        }

        static Network *New();

        static void Delete(Network *);

//        Network(PeerList *);
        virtual ~Network() = default;

        void Start(const node_id_vector &, const node_id_t &self);

        void Quit(node_id_t);

    };
}

#endif
//PAXOSME_GRPC_NETWORK_HPP
